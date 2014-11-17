/*
 * PROJECT: GEMMapper
 * FILE: mapper_cuda.c
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 */

#include "mapper_cuda.h"

/*
 * Mapper-CUDA Search
 */
typedef struct {
  /* Thread Info */
  uint64_t thread_id;
  pthread_t* thread_data;
  /* I/O */
  buffered_input_file_t* buffered_fasta_input;
  buffered_input_file_t* buffered_fasta_input_end1;
  buffered_input_file_t* buffered_fasta_input_end2;
  /* Mapper parameters */
  mapper_parameters_t* mapper_parameters;
  /* Archive-Search group */
  archive_search_group_t* search_group;
  /* Ticker */
  ticker_t* ticker;
} mapper_cuda_search_t;

/*
 * Error report
 */
mapper_cuda_search_t* g_mapper_cuda_search; // Global searches on going
void mapper_cuda_error_report(FILE* stream) {
//  // Display thread info
//  const uint64_t threads_id = gem_thread_get_thread_id();
//  if (threads_id==0) {
//    fprintf(stream,"GEM::Running-Thread (threadID = MASTER)\n");
//  } else {
//    uint64_t i;
//    // Display Generating threads
//    const uint64_t num_generating_threads = g_mapper_cuda_search->cuda_parameters->num_generating_threads;
//    for (i=0;i<num_generating_threads;++i) {
//      mapper_cuda_search_t* const mapper_cuda_search = g_mapper_searches + i;
//      // Generating Thread
//      fprintf(stream,"GEM::Generating-Thread (threadID = %lu)\n",mapper_cuda_search->thread_id);
//      // Display Input State
//      const sequence_t* const sequence = archive_search_get_sequence(mapper_cuda_search->archive_search);
//      tab_global_inc();
//      mapper_display_input_state(stream,mapper_cuda_search->buffered_fasta_input,sequence);
//      tab_global_dec();
//    }
//    // Display Selecting threads
//    const uint64_t num_selecting_threads = g_mapper_cuda_search->cuda_parameters->num_selecting_threads;
//    for (i=0;i<num_selecting_threads;++i) {
//      mapper_cuda_search_t* const mapper_cuda_search = g_mapper_searches + num_generating_threads + i;
//      // Selecting Thread
//      fprintf(stream,"GEM::Selecting-Thread (threadID = %lu)\n",mapper_cuda_search->thread_id);
//      // Display Input State
//      const sequence_t* const sequence = archive_search_get_sequence(mapper_cuda_search->archive_search);
//      tab_global_inc();
//      mapper_display_input_state(stream,mapper_cuda_search->buffered_fasta_input,sequence);
//      tab_global_dec();
//    }
//  }
}
/*
 * SE CUDA Mapper
 */
void* mapper_SE_CUDA_thread(mapper_cuda_search_t* const mapper_search) {
  // GEM-thread error handler
  gem_thread_register_id(mapper_search->thread_id+1);

  // Create new buffered reader/writer
  const mapper_parameters_t* const parameters = mapper_search->mapper_parameters;
  const mapper_parameters_cuda_t* const cuda_parameters = &parameters->cuda;
  archive_search_group_t* const search_group = mapper_search->search_group;
  mapper_search->buffered_fasta_input = buffered_input_file_new(parameters->input_file,cuda_parameters->input_buffer_lines);
  buffered_output_file_t* const buffered_output_file = buffered_output_file_new(parameters->output_file);
  buffered_input_file_attach_buffered_output(mapper_search->buffered_fasta_input,buffered_output_file);
  matches_t* const matches = matches_new();

  archive_search_t* archive_search_generate = NULL;

  // FASTA/FASTQ reading loop
  uint64_t reads_processed = 0;
  while (true) {
    // Check the end_of_block (Reload input-buffer if needed)
    if (buffered_input_file_eob(mapper_search->buffered_fasta_input)) {
      // We cannot reload input-buffer until all the searches of the previous block are solved
      if (archive_search_group_is_empty(search_group)) {
        if (buffered_input_file_reload__dump_attached(mapper_search->buffered_fasta_input)==INPUT_STATUS_EOF) break;
      }
    }
    // Keep processing the current input-block
    while (!buffered_input_file_eob(mapper_search->buffered_fasta_input)) {
      // Request a clean archive-search
      archive_search_generate = archive_search_group_alloc(search_group);

      // Parse Sequence
      const error_code_t error_code = input_fasta_parse_sequence(
          mapper_search->buffered_fasta_input,archive_search_get_sequence(archive_search_generate),
          parameters->io.fastq_strictly_normalized,parameters->io.fastq_try_recovery,false);
      gem_cond_fatal_error(error_code==INPUT_STATUS_FAIL,MAPPER_CUDA_ERROR_PARSING);

      // Generate Candidates (Search into the archive)
      archive_search_generate_candidates(archive_search_generate);

      // Add archive-search to group (Put candidates in buffer)
      if (!archive_search_group_add_search(search_group,archive_search_generate)) break; // Go to select-candidates
      archive_search_generate = NULL; // Last archive-search is in BPM-buffer
    }
    // Start retrieving
    archive_search_group_retrieve_begin(search_group);
    // Process all search-groups generated
    bpm_gpu_buffer_t* bpm_gpu_buffer;
    archive_search_t* archive_search_select;
    while (archive_search_group_get_search(search_group,&archive_search_select,&bpm_gpu_buffer)) {
      // Init
      matches_configure(matches,archive_search_select->text_collection);
      matches_clear(matches);
      // Select candidates
      archive_search_retrieve_candidates(archive_search_select,bpm_gpu_buffer,matches);
      // Select matches
      archive_select_matches(archive_search_select,matches);
      // Score matches // TODO
      // archive_search_score_matches(archive_search,matches); // TODO
      // Output matches
      mapper_SE_output_matches(parameters,buffered_output_file,archive_search_get_sequence(archive_search_select),matches);
      // Update processed
      if (++reads_processed == MAPPER_TICKER_STEP) {
        ticker_update_mutex(mapper_search->ticker,reads_processed);
        reads_processed=0;
      }
    }
    archive_search_group_clear(search_group); // Reset search-group
    // Check if the last archive-search couldn't fit into the BPM-buffer (Put candidates in buffer if not)
    if (archive_search_generate!=NULL) {
      archive_search_group_add_search(search_group,archive_search_generate);
      archive_search_generate = NULL;
    }
  }
  // Update processed
  ticker_update_mutex(mapper_search->ticker,reads_processed);

  // Clean up
  buffered_input_file_close(mapper_search->buffered_fasta_input);
  buffered_output_file_close(buffered_output_file);
  matches_delete(matches);

  pthread_exit(0);
}
/*
 * SE-CUDA runnable
 */
GEM_INLINE void mapper_SE_CUDA_run(mapper_parameters_t* const mapper_parameters) {
  // Check CUDA-Support & parameters compliance
  if (!bpm_gpu_support()) GEM_CUDA_NOT_SUPPORTED();
  mapper_parameters_cuda_t* const cuda_parameters = &mapper_parameters->cuda;
  const uint64_t num_threads = mapper_parameters->system.num_threads;
  const uint64_t num_search_groups_per_thread = cuda_parameters->num_search_groups_per_thread;
  const uint64_t num_search_groups = num_search_groups_per_thread * num_threads;
  // Prepare BPM-GPU Buffer
  bpm_gpu_buffer_collection_t* const bpm_gpu_buffer_collection =
      bpm_gpu_init(mapper_parameters->archive->enc_text,num_search_groups,cuda_parameters->bpm_buffer_size,
          mapper_parameters->hints.avg_read_length,mapper_parameters->hints.candidates_per_query,
          mapper_parameters->misc.verbose_dev);
  // Prepare output file (SAM headers)
  if (mapper_parameters->io.output_format==SAM) {
    output_sam_print_header(mapper_parameters->output_file,
        mapper_parameters->archive,mapper_parameters->argc,mapper_parameters->argv);
  }
  // Ticker
  ticker_t ticker;
  ticker_count_reset(&ticker,mapper_parameters->misc.verbose_user,"SE-CUDA::Mapping Sequences",0,MAPPER_TICKER_STEP,false);
  ticker_add_process_label(&ticker,"#","sequences processed");
  ticker_add_finish_label(&ticker,"Total","sequences processed");
  ticker_mutex_enable(&ticker);
  // Prepare Mapper searches
  mapper_cuda_search_t* const mapper_search = mm_malloc(num_threads*sizeof(mapper_cuda_search_t));
  // Set error-report function
  g_mapper_cuda_search = mapper_search;
  gem_error_set_report_function(mapper_cuda_error_report);
  /*
   * Launch 'Generating Candidates' threads
   */
  bpm_gpu_buffer_t* const bpm_gpu_buffers = bpm_gpu_buffer_collection->bpm_gpu_buffers;
  uint64_t i, bpm_gpu_buffer_pos=0;
  for (i=0;i<num_threads;++i) {
    // Setup Thread
    mapper_search[i].thread_id = i;
    mapper_search[i].thread_data = mm_alloc(pthread_t);
    mapper_search[i].mapper_parameters = mapper_parameters;
    mapper_search[i].search_group =
        archive_search_group_new(mapper_parameters,bpm_gpu_buffers+bpm_gpu_buffer_pos,num_search_groups_per_thread);
    bpm_gpu_buffer_pos += num_search_groups_per_thread;
    mapper_search[i].ticker = &ticker;
    // Launch thread
    gem_cond_fatal_error__perror(
        pthread_create(mapper_search[i].thread_data,0,
            (void* (*)(void*) )mapper_SE_CUDA_thread,(void* )(mapper_search + i)),SYS_THREAD_CREATE);
  }
  // Join all threads
  for (i=0;i<num_threads;++i) {
    gem_cond_fatal_error__perror(pthread_join(*(mapper_search[i].thread_data),0),SYS_THREAD_JOIN);
    archive_search_group_delete(mapper_search[i].search_group);
    mm_free(mapper_search[i].thread_data);
  }
  // Clean up
  ticker_finish(&ticker);
  ticker_mutex_cleanup(&ticker);
  mm_free(mapper_search); // Delete mapper-CUDA searches
}

/*
 * PROJECT: GEMMapper
 * FILE: archive_search_pe_stepwise.c
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 */

#include "archive/archive_search_pe_stepwise.h"
#include "archive/archive_search_se_stepwise.h"
#include "archive/archive_search.h"
#include "archive/archive_search_parameters.h"
#include "archive/archive_search_pe.h"
#include "archive/archive_select.h"
#include "archive/archive_score_se.h"
#include "archive/archive_score_pe.h"
#include "archive/archive_check.h"
#include "approximate_search/approximate_search_stepwise.h"

/*
 * Debug
 */
#define DEBUG_ARCHIVE_SEARCH_PE_STEPWISE GEM_DEEP_DEBUG

/*
 * Profile
 */
#define PROFILE_LEVEL PHIGH

/*
 * Debug
 */
void archive_search_pe_stepwise_debug_preface(
    archive_search_t* const archive_search,
    const char* const label) {
  // DEBUG
  gem_cond_debug_block(DEBUG_ARCHIVE_SEARCH_PE_STEPWISE) {
    tab_fprintf(stderr,
        "[GEM]>ArchiveSearch.STEPWISE.PE :: %s (Tag=%s)\n",
        label,archive_search->sequence.tag.buffer);
    tab_global_inc();
  }
}
void archive_search_pe_stepwise_debug_epilogue() {
  gem_cond_debug_block(DEBUG_ARCHIVE_SEARCH_PE_STEPWISE) { tab_global_dec(); }
}

/*
 * Stepwise: Init Search
 */
void archive_search_pe_stepwise_init_search(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_INIT,PROFILE_LEVEL);
  // DEBUG
  gem_cond_debug_block(DEBUG_ARCHIVE_SEARCH_PE_STEPWISE) {
    tab_fprintf(stderr,"[GEM]>ArchiveSearch.STEPWISE.PE :: Region.Profile.Generate\n");
    tab_fprintf(gem_log_get_stream(),"  => Tag %s\n",archive_search_end1->sequence.tag.buffer);
    tab_fprintf(gem_log_get_stream(),"  => Sequence.End1 %s\n",archive_search_end1->sequence.read.buffer);
    tab_fprintf(gem_log_get_stream(),"  => Sequence.End2 %s\n",archive_search_end2->sequence.read.buffer);
  }
  // Init
  archive_search_end1->pair_searched = false;
  archive_search_end1->pair_extended = false;
  archive_search_end1->pair_extended_shortcut = false;
  archive_search_end2->pair_searched = false;
  archive_search_end2->pair_extended = false;
  archive_search_reset(archive_search_end1);
  archive_search_reset(archive_search_end2);
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_INIT,PROFILE_LEVEL);
}
/*
 * Stepwise: Region-Profile
 */
void archive_search_pe_stepwise_region_profile_static_generate(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_GENERATE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Generate");
  // Region-Profile Generate
  approximate_search_stepwise_region_profile_static_generate(&archive_search_end1->approximate_search);
  approximate_search_stepwise_region_profile_static_generate(&archive_search_end2->approximate_search);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_GENERATE,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_region_profile_static_copy(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_ssearch_t* const gpu_buffer_fmi_ssearch) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_COPY,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Copy");
  // Region-Profile Copy
  approximate_search_stepwise_region_profile_static_copy(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_ssearch);
  approximate_search_stepwise_region_profile_static_copy(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_ssearch);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_COPY,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_region_profile_static_retrieve(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_ssearch_t* const gpu_buffer_fmi_ssearch) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_RETRIEVE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Retrieve");
  // Region-Profile Retrieve
  approximate_search_stepwise_region_profile_static_retrieve(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_ssearch);
  approximate_search_stepwise_region_profile_static_retrieve(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_ssearch);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_RETRIEVE,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_region_profile_adaptive_generate(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_GENERATE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Generate");
  // Region-Profile Generate
  approximate_search_stepwise_region_profile_adaptive_generate(&archive_search_end1->approximate_search);
  approximate_search_stepwise_region_profile_adaptive_generate(&archive_search_end2->approximate_search);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_GENERATE,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_region_profile_adaptive_copy(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_asearch_t* const gpu_buffer_fmi_asearch) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_COPY,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Copy");
  // Region-Profile Copy
  approximate_search_stepwise_region_profile_adaptive_copy(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_asearch);
  approximate_search_stepwise_region_profile_adaptive_copy(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_asearch);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_COPY,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_region_profile_adaptive_retrieve(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_asearch_t* const gpu_buffer_fmi_asearch) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_RETRIEVE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Region.Profile.Retrieve");
  // Region-Profile Retrieve
  approximate_search_stepwise_region_profile_adaptive_retrieve(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_asearch);
  approximate_search_stepwise_region_profile_adaptive_retrieve(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_asearch);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_REGION_PROFILE_RETRIEVE,PROFILE_LEVEL);
}
/*
 * Stepwise: Decode-Candidates
 */
void archive_search_pe_stepwise_decode_candidates_copy(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_DECODE_CANDIDATES_COPY,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Decode.Candidates.Copy");
  // Decode-Candidates Copy
  approximate_search_stepwise_decode_candidates_copy(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_decode);
  approximate_search_stepwise_decode_candidates_copy(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_decode);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_DECODE_CANDIDATES_COPY,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_decode_candidates_retrieve(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_DECODE_CANDIDATES_RETRIEVE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Decode.Candidates.Retrieve");
  // Decode-Candidates Retrieve
  approximate_search_stepwise_decode_candidates_retrieve(
      &archive_search_end1->approximate_search,gpu_buffer_fmi_decode);
  approximate_search_stepwise_decode_candidates_retrieve(
      &archive_search_end2->approximate_search,gpu_buffer_fmi_decode);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_DECODE_CANDIDATES_RETRIEVE,PROFILE_LEVEL);
}
/*
 * Stepwise: Verify-Candidates
 */
void archive_search_pe_stepwise_verify_candidates_copy(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_align_bpm_t* const gpu_buffer_align_bpm) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_VERIFY_CANDIDATES_COPY,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Verify.Candidates.Copy");
  // Verify-Candidates Copy
  approximate_search_stepwise_verify_candidates_copy(
      &archive_search_end1->approximate_search,gpu_buffer_align_bpm);
  approximate_search_stepwise_verify_candidates_copy(
      &archive_search_end2->approximate_search,gpu_buffer_align_bpm);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_VERIFY_CANDIDATES_COPY,PROFILE_LEVEL);
}
void archive_search_pe_stepwise_verify_candidates_retrieve(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    gpu_buffer_align_bpm_t* const gpu_buffer_align_bpm,
    matches_t* const matches) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_VERIFY_CANDIDATES_RETRIEVE,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Verify.Candidates.Retrieve");
  // Verify-Candidates Retrieve
  approximate_search_stepwise_verify_candidates_retrieve(
      &archive_search_end1->approximate_search,gpu_buffer_align_bpm,matches);
  approximate_search_stepwise_verify_candidates_retrieve(
      &archive_search_end2->approximate_search,gpu_buffer_align_bpm,matches);
  // DEBUG
  archive_search_pe_stepwise_debug_epilogue();
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_VERIFY_CANDIDATES_RETRIEVE,PROFILE_LEVEL);
}

/*
 * Stepwise: Finish Search
 */
void archive_search_pe_stepwise_finish_search(
    archive_search_t* const archive_search_end1,
    archive_search_t* const archive_search_end2,
    paired_matches_t* const paired_matches) {
  PROFILE_START(GP_ARCHIVE_SEARCH_PE_FINISH_SEARCH,PROFILE_LEVEL);
  // DEBUG
  archive_search_pe_stepwise_debug_preface(archive_search_end1,"Finish");
  // Finish SE-Search
  search_parameters_t* const search_parameters = &archive_search_end1->search_parameters;
  archive_search_se_stepwise_finish_search(archive_search_end1,paired_matches->matches_end1,true);
  archive_search_se_stepwise_finish_search(archive_search_end2,paired_matches->matches_end2,true);
  archive_search_end1->pe_search_state = archive_search_pe_state_recovery;
  archive_search_end1->pair_searched = true;
  archive_search_end2->pair_searched = true;
  archive_search_pe_continue(archive_search_end1,archive_search_end2,paired_matches);
  // PE Score (Select alignment-Model and process accordingly)
  archive_score_matches_pe(archive_search_end1,archive_search_end2,paired_matches);
  // PE Check matches
  if (search_parameters->check_type!=archive_check_nothing) {
    archive_check_pe_matches(
        archive_search_end1->archive,search_parameters->alignment_model,
        &search_parameters->swg_penalties,&archive_search_end1->sequence,
        &archive_search_end2->sequence,paired_matches,
        search_parameters->check_type,archive_search_end1->mm_stack);
  }
  // DEBUG
  gem_cond_debug_block(DEBUG_ARCHIVE_SEARCH_PE_STEPWISE) {
    tab_global_inc();
    archive_search_pe_print(gem_log_get_stream(),archive_search_end1,archive_search_end2,paired_matches);
    tab_global_dec();
    tab_global_dec();
  }
  PROFILE_STOP(GP_ARCHIVE_SEARCH_PE_FINISH_SEARCH,PROFILE_LEVEL);
}


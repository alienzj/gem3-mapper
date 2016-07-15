/*
 * PROJECT: GEMMapper
 * FILE: match_align.c
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#include "matches/match_align.h"
#include "matches/match_align_swg.h"
#include "matches/match_align_swg_local.h"
#include "matches/match_align_normalize.h"
#include "matches/match_align_dto.h"
#include "matches/matches_cigar.h"
#include "align/align.h"
#include "align/align_swg.h"
#include "align/align_bpm.h"
#include "archive/archive_text_rl.h"

/*
 * Profile
 */
#define PROFILE_LEVEL PLOW

/*
 * Match Clipping
 */
void match_aling_add_clipping(
    match_trace_t* const match_trace,
    vector_t* const cigar_vector,
    const uint64_t sequence_clip_left,
    const uint64_t sequence_clip_right) {
  // Left trim
  if (sequence_clip_left > 0) {
    // Parameters
    match_alignment_t* const match_alignment = &match_trace->match_alignment;
    const uint64_t cigar_offset = match_alignment->cigar_offset;
    cigar_element_t* cigar_buffer = vector_get_elm(cigar_vector,cigar_offset,cigar_element_t);
    if (cigar_buffer[0].type==cigar_del) {
      cigar_buffer[0].length += sequence_clip_left;
      cigar_buffer[0].attributes = cigar_attr_trim;
    } else {
      // Reserve additional
      vector_reserve_additional(cigar_vector,1);
      vector_inc_used(cigar_vector); // Increment used
      cigar_buffer = vector_get_elm(cigar_vector,cigar_offset,cigar_element_t);
      // Shift CIGAR one position right
      uint64_t i;
      for (i=match_alignment->cigar_length;i>0;--i) {
        cigar_buffer[i] = cigar_buffer[i-1];
      }
      // Add clip
      cigar_buffer[0].type = cigar_del;
      cigar_buffer[0].attributes = cigar_attr_trim;
      cigar_buffer[0].length = sequence_clip_left;
      ++(match_alignment->cigar_length);
    }
  }
  // Right trim
  if (sequence_clip_right > 0) {
    // Parameters
    match_alignment_t* const match_alignment = &match_trace->match_alignment;
    const uint64_t cigar_offset = match_alignment->cigar_offset;
    cigar_element_t* cigar_buffer = vector_get_elm(cigar_vector,cigar_offset,cigar_element_t);
    const uint64_t cigar_length = match_alignment->cigar_length;
    if (cigar_buffer[cigar_length-1].type==cigar_del) {
      cigar_buffer[cigar_length-1].length += sequence_clip_right;
      cigar_buffer[cigar_length-1].attributes = cigar_attr_trim;
    } else {
      // Reserve additional
      vector_reserve_additional(cigar_vector,1);
      vector_inc_used(cigar_vector); // Increment used
      cigar_buffer = vector_get_elm(cigar_vector,cigar_offset,cigar_element_t);
      // Add clip
      cigar_buffer[cigar_length].type = cigar_del;
      cigar_buffer[cigar_length].attributes = cigar_attr_trim;
      cigar_buffer[cigar_length].length = sequence_clip_right;
      ++(match_alignment->cigar_length);
    }
  }
}
/*
 * Exact Alignment
 *   @align_input->key_length
 *   @align_input->text_position
 *   @align_input->text_trace_offset
 *   @align_input->text
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_parameters->swg_penalties
 *   @match_trace->match_alignment.score
 */
void match_align_exact(
    matches_t* const matches,
    match_trace_t* const match_trace,
    match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters) {
  PROFILE_START(GP_MATCHES_ALIGN_EXACT,PROFILE_LEVEL);
  // Parameters
  const uint64_t key_length = align_input->key_length;
  match_alignment_t* const match_alignment = &match_trace->match_alignment;
  // Configure match-trace
  match_trace->type = match_type_regular;
  match_trace->text_trace_offset = align_input->text_trace_offset;
  match_trace->text = NULL;
  match_trace->text_length = align_input->key_length;
  match_trace->sequence_name = NULL;
  match_trace->text_position = UINT64_MAX;
  match_trace->distance = 0;
  match_trace->edit_distance = 0;
  match_trace->swg_score = align_swg_score_match(align_parameters->swg_penalties,(int32_t)key_length);
  // Set position/distance
  region_alignment_t* const region_alignment = align_input->region_alignment;
  match_alignment->match_text_offset = region_alignment->alignment_tiles->text_begin_offset;
  match_alignment->match_position = align_input->text_position + match_alignment->match_text_offset; // Adjust position
  match_alignment->cigar_offset = vector_get_used(matches->cigar_vector);
  match_alignment->cigar_length = 0;
  match_alignment->score = 0;
  match_alignment->effective_length = key_length;
  // Insert exact-match CIGAR
  matches_cigar_vector_append_match(
      matches->cigar_vector,&match_alignment->cigar_length,key_length,cigar_attr_none);
  PROFILE_STOP(GP_MATCHES_ALIGN_EXACT,PROFILE_LEVEL);
}
/*
 * Hamming Alignment (Only mismatches)
 *   @align_input->key
 *   @align_input->key_length
 *   @align_input->text_position
 *   @align_input->text_trace_offset
 *   @align_input->text
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_parameters->allowed_enc
 */
void match_align_hamming(
    matches_t* const matches,
    match_trace_t* const match_trace,
    match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters) {
  PROFILE_START(GP_MATCHES_ALIGN_HAMMING,PROFILE_LEVEL);
  // Parameters
  region_alignment_t* const region_alignment = align_input->region_alignment;
  const uint64_t text_offset_begin = region_alignment->alignment_tiles->text_begin_offset;
  const uint8_t* const key = align_input->key;
  const uint64_t key_length = align_input->key_length;
  uint8_t* const text = align_input->text + text_offset_begin;
  const bool* const allowed_enc = align_parameters->allowed_enc;
  // Configure match-trace
  match_trace->type = match_type_regular;
  match_trace->text_trace_offset = align_input->text_trace_offset;
  match_trace->text = text;
  match_trace->text_length = key_length;
  match_trace->sequence_name = NULL;
  match_trace->text_position = UINT64_MAX;
  match_alignment_t* const match_alignment = &match_trace->match_alignment;
  match_alignment->match_text_offset = text_offset_begin;
  match_alignment->match_position = align_input->text_position + text_offset_begin;
  match_alignment->cigar_offset = vector_get_used(matches->cigar_vector);
  match_alignment->cigar_length = 0;
  // Hamming Alignment
  uint64_t i, mismatches;
  for (i=0,mismatches=0;i<key_length;++i) {
    const uint8_t candidate_enc = text[i];
    // Check Mismatch
    if (!allowed_enc[candidate_enc] || candidate_enc != key[i]) {
      ++mismatches;
      matches_cigar_vector_append_mismatch(matches->cigar_vector,
          &match_alignment->cigar_length,candidate_enc,cigar_attr_none);
    } else {
      matches_cigar_vector_append_match(matches->cigar_vector,
          &match_alignment->cigar_length,1,cigar_attr_none);
    }
  }
  match_alignment->effective_length = key_length;
  // Set distances
  match_trace->distance = mismatches;
  match_trace->edit_distance = mismatches;
  match_trace->swg_score = align_swg_score_cigar(align_parameters->swg_penalties,
      matches->cigar_vector,match_alignment->cigar_offset,match_alignment->cigar_length);
  match_trace->match_scaffold = NULL;
  PROFILE_STOP(GP_MATCHES_ALIGN_HAMMING,PROFILE_LEVEL);
}
/*
 * Levenshtein
 *   @align_input->key
 *   @align_input->key_length
 *   @align_input->bpm_pattern
 *   @align_input->text_trace_offset
 *   @align_input->text_position
 *   @align_input->text
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_input->text_length
 *   @align_parameters->swg_penalties
 *   @align_parameters->max_error
 *   @align_parameters->left_gap_alignment
 */
void match_align_levenshtein(
    matches_t* const matches,
    match_trace_t* const match_trace,
    match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters,
    mm_stack_t* const mm_stack) {
  PROFILE_START(GP_MATCHES_ALIGN_LEVENSHTEIN,PROFILE_LEVEL);
  // Configure match-trace
  match_trace->type = match_type_regular;
  match_trace->text_trace_offset = align_input->text_trace_offset;
  match_trace->sequence_name = NULL;
  match_trace->text_position = UINT64_MAX;
  // Levenshtein Align
  match_alignment_t* const match_alignment = &match_trace->match_alignment;
  match_alignment->match_position = align_input->text_position;
  align_bpm_match(align_input,align_parameters->max_error,
      align_parameters->left_gap_alignment,match_alignment,matches->cigar_vector,mm_stack);
  // Set distances
  if (match_alignment->score==-1) {
    PROF_INC_COUNTER(GP_ALIGNED_DISCARDED_SWG);
    match_trace->distance = ALIGN_DISTANCE_INF;
    match_trace->edit_distance = ALIGN_DISTANCE_INF;
    match_trace->swg_score = SWG_SCORE_MIN;
  } else {
    match_trace->distance = match_alignment->score;
    match_trace->edit_distance = match_alignment->score;
    match_trace->swg_score = align_swg_score_cigar(align_parameters->swg_penalties,
        matches->cigar_vector,match_alignment->cigar_offset,match_alignment->cigar_length);
    // Store matching text
    match_trace->match_scaffold = NULL;
    match_alignment->match_text_offset = match_alignment->match_position - align_input->text_position;
    match_trace->text = align_input->text + match_alignment->match_text_offset;
    match_trace->text_length = match_alignment->effective_length;
  }
  PROFILE_STOP(GP_MATCHES_ALIGN_LEVENSHTEIN,PROFILE_LEVEL);
}
/*
 * Smith-Waterman-Gotoh Alignment (Gap-affine)
 *   @align_input->key
 *   @align_input->key_length
 *   @align_input->text_trace_offset
 *   @align_input->text_position
 *   @align_input->text
 *   @align_input->text_length
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_parameters->swg_penalties
 *   @align_parameters->swg_threshold
 *   @align_parameters->min_identity
 *   @align_parameters->left_gap_alignment
 *   @align_parameters->allowed_enc
 *   @align_parameters->max_bandwidth
 *   @align_parameters->cigar_curation
 *   @align_parameters->cigar_curation_min_end_context
 *   @match_scaffold->scaffold_regions
 *   @match_scaffold->num_scaffold_regions
 */
void match_align_smith_waterman_gotoh(
    matches_t* const matches,
    match_trace_t* const match_trace,
    match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters,
    match_scaffold_t* const match_scaffold,
    mm_stack_t* const mm_stack) {
  PROFILE_START(GP_MATCHES_ALIGN_SWG,PROFILE_LEVEL);
  // Configure match-trace
  match_trace->text_trace_offset = align_input->text_trace_offset;
  match_trace->sequence_name = NULL;
  match_trace->text_position = UINT64_MAX;
  // Scaffold the alignment
  if (!align_parameters->force_full_swg) {
    PROFILE_PAUSE(GP_MATCHES_ALIGN_SWG,PROFILE_LEVEL);
    match_scaffold_adaptive(match_scaffold,align_input,align_parameters,matches,mm_stack);
    PROFILE_CONTINUE(GP_MATCHES_ALIGN_SWG,PROFILE_LEVEL);
    if (match_scaffold->num_scaffold_regions==0) {
      PROF_INC_COUNTER(GP_ALIGNED_DISCARDED_SCAFFOLD);
      match_trace->swg_score = SWG_SCORE_MIN;
      match_trace->distance = ALIGN_DISTANCE_INF;
      return;
    }
  }
  match_trace->match_scaffold = match_scaffold;
  // Align SWG
  match_align_swg(matches,match_trace,align_input,align_parameters,match_scaffold,mm_stack);
  // Check for bad alignments (discarded)
  match_alignment_t* const match_alignment = &match_trace->match_alignment;
  if (match_alignment->score == SWG_SCORE_MIN) { // Input trims not computed
    PROF_INC_COUNTER(GP_ALIGNED_DISCARDED_SWG);
    match_trace->swg_score = SWG_SCORE_MIN;
    match_trace->distance = ALIGN_DISTANCE_INF;
    return;
  }
  // Normalize CIGAR & Adjust Position (Translate RL if required)
  match_align_normalize(matches,match_trace,align_input,align_parameters,mm_stack);
  // Compute matching bases (identity) + score
  match_align_swg_compute_alignment_type(matches,match_trace,align_parameters);
  if (match_trace->type == match_type_local) {
    // Compute Local Alignment
    match_align_swg_local_alignment(matches,match_trace,align_input,align_parameters);
    if (match_trace->swg_score == 0) {
      match_trace->swg_score = SWG_SCORE_MIN;
      match_trace->distance = ALIGN_DISTANCE_INF;
      return;
    }
  }
  // Compute distance + edit distance + effective-length
  vector_t* const cigar_vector = matches->cigar_vector;
  const uint64_t cigar_offset = match_alignment->cigar_offset;
  const uint64_t cigar_length = match_alignment->cigar_length;
  match_trace->distance = matches_cigar_compute_event_distance(cigar_vector,cigar_offset,cigar_length);
  match_trace->edit_distance = matches_cigar_compute_edit_distance(cigar_vector,cigar_offset,cigar_length);
  match_alignment->effective_length = matches_cigar_effective_length(cigar_vector,cigar_offset,cigar_length);
  match_trace->text_length = match_alignment->effective_length;
  PROFILE_STOP(GP_MATCHES_ALIGN_SWG,PROFILE_LEVEL);
}


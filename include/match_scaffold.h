/*
 * PROJECT: GEMMapper
 * FILE: match_scaffold.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef MATCH_SCAFFOLD_H_
#define MATCH_SCAFFOLD_H_

#include "essentials.h"
#include "bpm_align.h"
#include "swg_align.h"
#include "matches.h"

/*
 * Match Scaffold
 */
typedef struct {
  /* Scaffold matching regions */
  region_matching_t* scaffold_regions;
  uint64_t num_scaffold_regions;
  uint64_t scaffolding_coverage;
  /* Underlying Alignment */
  match_alignment_t match_alignment;
} match_scaffold_t;

/*
 * Setup
 */
GEM_INLINE void match_scaffold_init(match_scaffold_t* const match_scaffold);

/*
 * Scaffold the alignment (based on levenshtein alignment)
 *   @align_input->key
 *   @align_input->key_length
 *   @align_input->bpm_pattern
 *   @align_input->text_position
 *   @align_input->text
 *   @align_input->text_length
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_parameters->left_gap_alignment
 *   @align_parameters->min_matching_length
 *   @align_parameters->min_context_length
 */
GEM_INLINE bool match_scaffold_levenshtein(
    matches_t* const matches,match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters,
    match_scaffold_t* const match_scaffold,mm_stack_t* const mm_stack);

/*
 * Compute an scaffold for the alignment
 *   @align_input->key
 *   @align_input->key_length
 *   @align_input->bpm_pattern
 *   @align_input->text_position
 *   @align_input->text
 *   @align_input->text_length
 *   @align_input->text_offset_begin
 *   @align_input->text_offset_end
 *   @align_parameters->max_error
 *   @align_parameters->left_gap_alignment
 *   @align_parameters->min_coverage
 *   @align_parameters->min_matching_length
 *   @align_parameters->min_context_length
 *   @match_scaffold->num_scaffold_regions
 *   @match_scaffold->scaffold_regions
 */
GEM_INLINE void match_scaffold_alignment(
    matches_t* const matches,match_align_input_t* const align_input,
    match_align_parameters_t* const align_parameters,
    match_scaffold_t* const match_scaffold,mm_stack_t* const mm_stack);

/*
 * Sorting
 */
GEM_INLINE void match_scaffold_sort_regions_matching(match_scaffold_t* const match_scaffold);

/*
 * Display
 */
GEM_INLINE void match_scaffold_print(
    FILE* const stream,matches_t* const matches,match_scaffold_t* const match_scaffold);

#endif /* MATCH_SCAFFOLD_H_ */
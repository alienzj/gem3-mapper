/*
 * PROJECT: GEMMapper
 * FILE: filtering_region.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef FILTERING_REGION_H_
#define FILTERING_REGION_H_

#include "essentials.h"

#include "archive_text.h"
#include "text_collection.h"
#include "approximate_search_parameters.h"
#include "matches.h"
#include "match_elements.h"
#include "match_scaffold.h"
#include "pattern.h"

/*
 * Debug
 */
#define DEBUG_ALIGN_CANDIDATES  false

/*
 * Filtering Region
 */
typedef enum {
  filtering_region_pending,               // On-hold state (unverified)
  filtering_region_unverified,            // Region still unverified
  filtering_region_accepted,              // Region accepted by pre-filters
  filtering_region_accepted_subdominant,  // Region accepted by pre-filters but sub-dominant
  filtering_region_aligned,               // Region aligned
  filtering_region_aligned_subdominant,   // Region aligned but sub-dominant in the end
  filtering_region_discarded              // Region discarded
} filtering_region_status_t;
typedef struct {
  /* State */
  filtering_region_status_t status;
  /* Text-trace */
  uint64_t text_trace_offset;
  /* Location */
  uint64_t begin_position;             // Region effective begin position (adjusted to error boundaries)
  uint64_t end_position;               // Region effective end position   (adjusted to error boundaries)
  uint64_t base_position_offset;       // Offset to base filtering position (Begin position without error boundary correction)
  /* Regions Matching */
  match_scaffold_t match_scaffold;     // Matching regions supporting the filtering region (Scaffolding)
  /* Alignment distance */
  uint64_t align_distance;             // Distance (In case of approximate: max-bound)
  uint64_t align_distance_min_bound;   // Approximated distance (min-bound)
  uint64_t align_match_begin_column;   // Match begin column (inclusive)
  uint64_t align_match_end_column;     // Matching column (inclusive)
} filtering_region_t;
typedef struct {
  /* Filtering region */
  filtering_region_t* filtering_region;
  /* Location */
  sequence_end_t sequence_end;
  strand_t strand;
  uint64_t position;
} filtering_region_locator_t;
typedef struct {
  /* Location */
  uint64_t begin_position;
  uint64_t end_position;
} verified_region_t;

/*
 * Sorting
 */
GEM_INLINE void filtering_region_sort_regions_matching(const filtering_region_t* const filtering_region);
GEM_INLINE void filtering_region_locator_sort_positions(vector_t* const filtering_region_locators);

/*
 * Matching regions
 */
GEM_INLINE void filtering_region_exact_extend(
    filtering_region_t* const filtering_region,
    const uint8_t* const key,const uint64_t key_length,
    const uint8_t* const text,const bool* const allowed_enc);
GEM_INLINE void filtering_region_chain_matching_regions(
    filtering_region_t* const filtering_region,
    const uint8_t* const key,const uint64_t key_length,
    const uint8_t* const text,const bool* const allowed_enc,
    const uint64_t max_error,mm_stack_t* const stack);

/*
 * (Re)Align
 */
GEM_INLINE bool filtering_region_align(
    filtering_region_t* const filtering_region,archive_text_t* const archive_text,
    const text_collection_t* const text_collection,const as_parameters_t* const as_parameters,
    const bool emulated_rc_search,const bool left_gap_alignment,
    pattern_t* const pattern,matches_t* const matches,
    match_trace_t* const match_trace,mm_stack_t* const mm_stack);
GEM_INLINE bool filtering_region_local_align(
    filtering_region_t* const filtering_region,archive_text_t* const archive_text,
    const text_collection_t* const text_collection,const as_parameters_t* const as_parameters,
    const bool emulated_rc_search,const bool left_gap_alignment,
    pattern_t* const pattern,matches_t* const matches,
    match_trace_t* const match_trace,mm_stack_t* const mm_stack);

/*
 * Verify
 */
GEM_INLINE bool filtering_region_verify(
    filtering_region_t* const filtering_region,
    const text_collection_t* const text_collection,
    search_parameters_t* const search_parameters,const pattern_t* const pattern);
GEM_INLINE uint64_t filtering_region_verify_multiple_hits(
    vector_t* const filtering_regions,filtering_region_t* const filtering_region,
    const text_collection_t* const text_collection,search_parameters_t* const search_parameters,
    const pattern_t* const pattern);

GEM_INLINE uint64_t filtering_region_verify_extension(
    vector_t* const filtering_regions,vector_t* const verified_regions,
    const text_collection_t* const text_collection,
    const uint64_t const text_trace_offset,const uint64_t index_position,
    search_parameters_t* const search_parameters,const pattern_t* const pattern);

#endif /* FILTERING_REGION_H_ */
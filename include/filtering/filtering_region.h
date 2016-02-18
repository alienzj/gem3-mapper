/*
 * PROJECT: GEMMapper
 * FILE: filtering_region.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef FILTERING_REGION_H_
#define FILTERING_REGION_H_

#include "utils/essentials.h"
#include "data_structures/sequence.h"
#include "data_structures/text_collection.h"
#include "archive/archive_text.h"
#include "matches/match_scaffold.h"

/*
 * Filtering Region
 */
typedef enum {
  filtering_region_pending=0,               // On-hold state (unverified)
  filtering_region_unverified=1,            // Region still unverified
  filtering_region_verified_discarded=2,    // Region verified but too distant
  filtering_region_accepted=3,              // Region accepted by pre-filters
  filtering_region_accepted_subdominant=4,  // Region accepted by pre-filters but sub-dominant (by ranking)
  filtering_region_aligned=5,               // Region aligned
} filtering_region_status_t;
extern const char* filtering_region_status_label[6];
typedef struct {
  /* State */
  filtering_region_status_t status;         // Filtering Region Status
  /* Location */
  uint64_t text_trace_offset;               // Text-trace
  uint64_t begin_position_translated;       // Translated begin-position from RL-Space (In case of RL-text)
  uint64_t begin_position;                  // Region effective begin position (adjusted to error boundaries)
  uint64_t end_position;                    // Region effective end position (adjusted to error boundaries)
  uint64_t base_begin_position_offset;      // Offset to base begin filtering position (no error boundary correction)
  uint64_t base_end_position_offset;        // Offset to base begin filtering position (no error boundary correction)
  /* Trimmed Pattern */
  uint64_t key_trim_left;                   // Key left trim  (due to position correction wrt the reference limits)
  uint64_t key_trim_right;                  // Key right trim (due to position correction wrt the reference limits)
  bool key_trimmed;                         // Key has been trimmed (due to dangling ends; projected to the text-candidate)
  bpm_pattern_t* bpm_pattern_trimmed;       // BPM-Pattern Trimmed
  bpm_pattern_t* bpm_pattern_trimmed_tiles; // BPM-Pattern Trimmed & Tiled
  /* Matching Regions */
  match_scaffold_t match_scaffold;          // Matching regions supporting the filtering region (Scaffolding)
  /* Alignment distance */
  uint64_t align_distance;                  // Distance (In case of approximate: max-bound)
  uint64_t align_distance_min_bound;        // Approximated distance (min-bound)
  uint64_t align_match_begin_column;        // Match begin column (inclusive)
  uint64_t align_match_end_column;          // Matching column (inclusive)
  /* Footprint */
  uint64_t footprint;                       // Footprint checksum
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
 * Accessors
 */
void filtering_region_add(
    vector_t* const filtering_regions,
    const uint64_t text_trace_offset,
    const uint64_t begin_position,
    const uint64_t end_position,
    const uint64_t align_distance,
    const uint64_t align_match_end_column);

/*
 * Retrieve filtering region text-candidate
 */
void filtering_region_retrieve_text(
    filtering_region_t* const filtering_region,
    archive_text_t* const archive_text,
    text_collection_t* const text_collection,
    mm_stack_t* const mm_stack);

/*
 * Sorting
 */
void filtering_region_sort_regions_matching(const filtering_region_t* const filtering_region);
void filtering_region_locator_sort_positions(vector_t* const filtering_region_locators);

/*
 * Display
 */
void filtering_region_print(
    FILE* const stream,
    filtering_region_t* const region,
    const text_collection_t* const text_collection,
    const bool print_matching_regions);

#endif /* FILTERING_REGION_H_ */

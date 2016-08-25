/*
 * PROJECT: GEMMapper
 * FILE: approximate_search_filtering_stepwise.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#include <approximate_search/approximate_search_stages.h>
#include "approximate_search/approximate_search_stepwise.h"
#include "approximate_search/approximate_search_region_profile.h"
#include "approximate_search/approximate_search_verify_candidates.h"
#include "approximate_search/approximate_search_generate_candidates.h"
#include "approximate_search/approximate_search_filtering_adaptive.h"
#include "approximate_search/approximate_search_control.h"
#include "approximate_search/approximate_search_neighborhood.h"
#include "filtering/region_profile_schedule.h"
#include "filtering/filtering_candidates_align.h"

/*
 * Profile
 */
#define PROFILE_LEVEL PMED

/*
 * Region Profile Utils
 */
void approximate_search_stepwise_region_profile_adaptive_compute(
    approximate_search_t* const search) {
  PROF_START(GP_ASSW_REGION_PROFILE_UNSUCCESSFUL);
  // Re-Compute region profile
  search->processing_state = asearch_processing_state_begin;
  approximate_search_region_profile_adaptive(search,region_profile_adaptive,search->mm_stack);
  if (search->processing_state==asearch_processing_state_no_regions) return;
  // Schedule exact-candidates
  region_profile_schedule_filtering_exact(&search->region_profile);
  // Set State
  search->processing_state = asearch_processing_state_region_profiled;
  PROF_STOP(GP_ASSW_REGION_PROFILE_UNSUCCESSFUL);
}
void approximate_search_stepwise_region_profile_limit_exact_matches(
    approximate_search_t* const search) {
  // Check exact matches (limit the number of matches)
  region_profile_t* const region_profile = &search->region_profile;
  region_profile->candidates_limited = false;
  if (region_profile_has_exact_matches(region_profile)) {
    search_parameters_t* const search_parameters = search->search_parameters;
    select_parameters_t* const select_parameters = &search_parameters->select_parameters_align;
    region_search_t* const filtering_region = region_profile->filtering_region;
    const uint64_t total_candidates = filtering_region->hi - filtering_region->lo;
    if (select_parameters->min_reported_strata_nominal==0 &&
        total_candidates > select_parameters->max_reported_matches) {
      filtering_region->hi = filtering_region->lo + select_parameters->max_reported_matches;
      region_profile->total_candidates = select_parameters->max_reported_matches;
      region_profile->candidates_limited = true;
    }
  }
}
/*
 * AM Stepwise :: Region Profile
 */
void approximate_search_stepwise_region_profile_static_generate(
    approximate_search_t* const search) {
  while (true) {
    switch (search->search_stage) {
      case asearch_stage_begin: // Search Start. Check basic cases
        approximate_search_begin(search);
        break;
      case asearch_stage_filtering_adaptive:
        approximate_search_region_profile_static_partition(search);
        return;
      default:
        GEM_INVALID_CASE();
        break;
    }
  }
}
void approximate_search_stepwise_region_profile_static_copy(
    approximate_search_t* const search,
    gpu_buffer_fmi_ssearch_t* const gpu_buffer_fmi_ssearch) {
  if (search->processing_state == asearch_processing_state_region_partitioned) {
    approximate_search_region_profile_static_buffered_copy(search,gpu_buffer_fmi_ssearch);
  }
}
void approximate_search_stepwise_region_profile_static_retrieve(
    approximate_search_t* const search,
    gpu_buffer_fmi_ssearch_t* const gpu_buffer_fmi_ssearch) {
  // Retrieve regions-interval (compute if not enabled)
  if (search->processing_state == asearch_processing_state_region_partitioned) {
    if (gpu_buffer_fmi_ssearch->fmi_search_enabled) {
      approximate_search_region_profile_static_buffered_retrieve(search,gpu_buffer_fmi_ssearch);
    } else {
      approximate_search_region_profile_static_compute(search);
    }
  }
  // Compute Region Profile Adaptively (unsuccessful cases)
  if (search->processing_state == asearch_processing_state_no_regions) {
    approximate_search_stepwise_region_profile_adaptive_compute(search);
    if (search->processing_state == asearch_processing_state_no_regions) return; // Corner cases
  }
  // Check exact matches & limit the number of matches
  approximate_search_stepwise_region_profile_limit_exact_matches(search);
}
void approximate_search_stepwise_region_profile_adaptive_generate(
    approximate_search_t* const search) {
  while (true) {
    switch (search->search_stage) {
      case asearch_stage_begin: // Search Start. Check basic cases
        approximate_search_begin(search);
        break;
      case asearch_stage_filtering_adaptive:
        search->search_stage = asearch_stage_filtering_adaptive;
        return;
      default:
        GEM_INVALID_CASE();
        break;
    }
  }
}
void approximate_search_stepwise_region_profile_adaptive_copy(
    approximate_search_t* const search,
    gpu_buffer_fmi_asearch_t* const gpu_buffer_fmi_asearch) {
  if (search->search_stage==asearch_stage_filtering_adaptive) {
    approximate_search_region_profile_adaptive_buffered_copy(search,gpu_buffer_fmi_asearch);
  }
}
void approximate_search_stepwise_region_profile_adaptive_retrieve(
    approximate_search_t* const search,
    gpu_buffer_fmi_asearch_t* const gpu_buffer_fmi_asearch) {
  // Retrieve regions-interval (compute if not enabled)
  if (search->search_stage==asearch_stage_filtering_adaptive) {
    if (gpu_buffer_fmi_asearch->fmi_search_enabled) {
      approximate_search_region_profile_adaptive_buffered_retrieve(search,gpu_buffer_fmi_asearch);
    } else {
      approximate_search_stepwise_region_profile_adaptive_compute(search);
    }
    // Check exact matches & limit the number of matches
    approximate_search_stepwise_region_profile_limit_exact_matches(search);
    // Set state profiled
    search->processing_state = asearch_processing_state_region_profiled;
  }
}
/*
 * AM Stepwise :: Decode Candidates
 */
void approximate_search_stepwise_decode_candidates_copy(
    approximate_search_t* const search,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode) {
  if (search->processing_state == asearch_processing_state_region_profiled) {
    approximate_search_generate_candidates_buffered_copy(search,gpu_buffer_fmi_decode);
  }
}
void approximate_search_stepwise_decode_candidates_retrieve(
    approximate_search_t* const search,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode) {
  if (search->processing_state == asearch_processing_state_region_profiled) {
    approximate_search_generate_candidates_buffered_retrieve(search,gpu_buffer_fmi_decode);
  }
}
/*
 * AM Stepwise :: Verify Candidates
 */
void approximate_search_stepwise_verify_candidates_copy(
    approximate_search_t* const search,
    gpu_buffer_align_bpm_t* const gpu_buffer_align_bpm) {
  if (search->processing_state == asearch_processing_state_candidates_processed) {
    approximate_search_verify_candidates_buffered_copy(search,gpu_buffer_align_bpm);
  }
}
void approximate_search_stepwise_verify_candidates_retrieve(
    approximate_search_t* const search,
    gpu_buffer_align_bpm_t* const gpu_buffer_align_bpm,
    matches_t* const matches) {
  if (search->search_stage==asearch_stage_filtering_adaptive) {
    if (search->processing_state == asearch_processing_state_candidates_processed) {
      approximate_search_verify_candidates_buffered_retrieve(search,gpu_buffer_align_bpm,matches);
    }
    search->search_stage = asearch_stage_filtering_adaptive_finished;
  }
}

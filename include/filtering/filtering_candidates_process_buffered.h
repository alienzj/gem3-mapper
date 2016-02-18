/*
 * PROJECT: GEMMapper
 * FILE: filtering_candidates_process_buffered.h
 * DATE: 06/06/2013
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef FILTERING_CANDIDATES_PROCESS_BUFFERED_H_
#define FILTERING_CANDIDATES_PROCESS_BUFFERED_H_

#include "filtering/filtering_candidates.h"
#include "data_structures/pattern.h"
#include "gpu/gpu_buffer_fmi_decode.h"

/*
 * Decode Candidates Buffered (from GPU-Buffer)
 */
void filtering_candidates_decode_filtering_positions_buffered(
    filtering_candidates_t* const filtering_candidates,
    pattern_t* const pattern,
    region_search_t* const region_search,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode,
    const uint64_t buffer_offset_begin);
void filtering_candidates_decode_filtering_positions_buffered_prefetched(
    filtering_candidates_t* const filtering_candidates,
    pattern_t* const pattern,
    region_search_t* const region_search,
    gpu_buffer_fmi_decode_t* const gpu_buffer_fmi_decode,
    const uint64_t buffer_offset_begin);

/*
 * Process Candidates Buffered (from GPU-Buffer)
 */
void filtering_candidates_process_candidates_buffered(
    filtering_candidates_t* const filtering_candidates,
    const pattern_t* const pattern,
    const bool compose_region_chaining);

#endif /* FILTERING_CANDIDATES_PROCESS_BUFFERED_H_ */

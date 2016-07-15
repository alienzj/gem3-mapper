/*
 * PROJECT: GEMMapper
 * FILE: approximate_search_neighborhood.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION:
 */

#ifndef APPROXIMATE_SEARCH_NEIGHBORHOOD_H_
#define APPROXIMATE_SEARCH_NEIGHBORHOOD_H_

#include "utils/essentials.h"
#include "approximate_search/approximate_search.h"

/*
 * Exact Search
 */
void approximate_search_neighborhood_exact_search(
    approximate_search_t* const search,
    matches_t* const matches);

/*
 * Neighborhood Search Brute Force
 */
void approximate_search_neighborhood_search_brute_force(
    approximate_search_t* const search,
    matches_t* const matches);

/*
 * Neighborhood Search (Using partitions)
 */
void approximate_search_neighborhood_search_partition(
    approximate_search_t* const search,
    matches_t* const matches);

/*
 * Neighborhood Search (Using partitions + region-profile preconditioned)
 */
void approximate_search_neighborhood_search_partition_preconditioned(
    approximate_search_t* const search,
    matches_t* const matches);

#endif /* APPROXIMATE_SEARCH_NEIGHBORHOOD_H_ */

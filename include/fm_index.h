/*
 * PROJECT: GEMMapper
 * FILE: fm_index.h
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: Provides FM-Index functionalities
 */

#ifndef FM_INDEX_H_
#define FM_INDEX_H_

#include "essentials.h"

#include "bwt.h"
#include "sampled_sa.h"
#include "rank_mtable.h"
#include "rank_mtable_builder.h"

/*
 * FM-Index
 */
typedef struct {
  /* Meta-info */
  uint64_t text_length;                   // Text length
  uint64_t proper_length;                 // Pl=log(text_length,4)
  /* Sampled SA */
  sampled_sa_t* sampled_sa;               // Sampled SuffixArray positions
  /* BWT */
  rank_mtable_t* rank_table;              // Memoizated intervals
  bwt_t* bwt;                             // BWT forward text
  rank_mtable_t* rank_table_reverse;      // Memoizated reverse intervals
  bwt_reverse_t* bwt_reverse;             // BWT reverse text
} fm_index_t;

/*
 * Builder
 */
void fm_index_write(
    fm_t* const file_manager,dna_text_t* const bwt_text,uint64_t* const character_occurrences,
    sampled_sa_builder_t* const sampled_sa,const bool check,const bool verbose);
void fm_index_reverse_write(
    fm_t* const file_manager,dna_text_t* const bwt_reverse_text,
    uint64_t* const character_occurrences,const bool check,const bool verbose);

/*
 * Loader
 */
fm_index_t* fm_index_read_mem(mm_t* const memory_manager,const bool check);
bool fm_index_check(const fm_index_t* const fm_index,const bool verbose);
void fm_index_delete(fm_index_t* const fm_index);

/*
 * Accessors
 */
uint64_t fm_index_get_length(const fm_index_t* const fm_index);
double fm_index_get_proper_length(const fm_index_t* const fm_index);
uint64_t fm_index_get_size(const fm_index_t* const fm_index);

/*
 * FM-Index Operators
 */
// Compute SA[i]
uint64_t fm_index_lookup(const fm_index_t* const fm_index,const uint64_t bwt_position);
// Compute SA^(-1)[i]
uint64_t fm_index_inverse_lookup(const fm_index_t* const fm_index,const uint64_t text_position);
// Compute Psi[i]
uint64_t fm_index_psi(const fm_index_t* const fm_index,const uint64_t bwt_position);
// Decode fm_index->text[bwt_position..bwt_position+length-1] into @buffer.
uint64_t fm_index_decode(
    const fm_index_t* const fm_index,const uint64_t bwt_position,const uint64_t length,char* const buffer);
// Retrieve SA-Sample
uint64_t fm_index_retrieve_sa_sample(
    const fm_index_t* const fm_index,
    const uint64_t sampled_bwt_position,const uint64_t lf_dist);

/*
 * Display
 */
void fm_index_print(FILE* const stream,const fm_index_t* const fm_index);

/*
 * Errors
 */
#define GEM_ERROR_FM_INDEX_WRONG_MODEL_NO "FM-index error. Wrong FM-Index Model %"PRIu64" (Expected model %"PRIu64")"
#define GEM_ERROR_FM_INDEX_INDEX_OOR "FM-index error. Index position %"PRIu64" out-of-range BWT[0,%"PRIu64")"

#endif /* FM_INDEX_H_ */

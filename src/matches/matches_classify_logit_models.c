/*
 * PROJECT: GEMMapper
 * FILE: matches_classify_logit_models.c
 * DATE: 06/06/2012
 * AUTHOR(S): Santiago Marco-Sola <santiagomsola@gmail.com>
 * DESCRIPTION: TODO
 */

#include "matches/matches_classify_logit_models.h"

/*
 * SE-Model DEFAULT
 */
const matches_classify_logit_model_t logit_model_single_end_default = {
  .unique_logit_coeff = {
      .coeff_intercept                            = -495.180,
      .coeff_primary_edit_distance_norm           = 0.0,
      .coeff_primary_event_distance_norm          = 501.273,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = 0.0,
      .coeff_mapq_end1                            = 0.0,
      .coeff_mapq_end2                            = 0.0,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = 0.0,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 0.0,
      .coeff_accepted_candidates                  = 0.0,
      .coeff_accepted_matches                     = 0.0,
      .coeff_mcs_end1                             = 5.934,
      .coeff_mcs_end2                             = 0.0,
      .coeff_max_region_length_norm               = -2.393,
      .coeff_kmer_frequency                       = 0.0,
  },
  .mmaps_logit_coeff = {
      .coeff_intercept                            = -279.7521,
      .coeff_primary_edit_distance_norm           = 122.3983,
      .coeff_primary_event_distance_norm          = 163.5526,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = 0.0,
      .coeff_mapq_end1                            = 0.0,
      .coeff_mapq_end2                            = 0.0,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = 0.0,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 0.0,
      .coeff_accepted_candidates                  = 0.0,
      .coeff_accepted_matches                     = 0.0,
      .coeff_mcs_end1                             = 2.6897,
      .coeff_mcs_end2                             = 0.0,
      .coeff_max_region_length_norm               = -1.4876,
      .coeff_kmer_frequency                       = 0.6666,
  },
  .mmaps_d1_logit_coeff = {
      .coeff_intercept                            = -40.7092,
      .coeff_primary_edit_distance_norm           = 44.6855,
      .coeff_primary_event_distance_norm          = 0.0,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = 0.0,
      .coeff_mapq_end1                            = 0.0,
      .coeff_mapq_end2                            = 0.0,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = 0.0,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 0.0,
      .coeff_accepted_candidates                  = -0.9001,
      .coeff_accepted_matches                     = 0.3075,
      .coeff_mcs_end1                             = 0.4977,
      .coeff_mcs_end2                             = 0.0,
      .coeff_max_region_length_norm               = -0.2070,
      .coeff_kmer_frequency                       = 0.2900,
  },
};
/*
 * PE-Model DEFAULT
 */
const matches_classify_logit_model_t logit_model_paired_end_default = {
  .unique_logit_coeff = {
      .coeff_intercept                            = -360.2979,
      .coeff_primary_edit_distance_norm           = 0.0,
      .coeff_primary_event_distance_norm          = 389.9600,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = 0.0,
      .coeff_mapq_end1                            = 0.0,
      .coeff_mapq_end2                            = 0.0,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = 0.0,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 0.0,
      .coeff_accepted_candidates                  = 0.0,
      .coeff_accepted_matches                     = 0.0,
      .coeff_mcs_end1                             = 1.1548,
      .coeff_mcs_end2                             = 1.5731,
      .coeff_max_region_length_norm               = -3.5912,
      .coeff_kmer_frequency                       = -0.7007,
  },
  .mmaps_logit_coeff = {
      .coeff_intercept                            = -130.6,
      .coeff_primary_edit_distance_norm           = 0.0,
      .coeff_primary_event_distance_norm          = 701.4,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = 0.0,
      .coeff_mapq_end1                            = 0.0,
      .coeff_mapq_end2                            = 0.0,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = -566.5,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 0.0,
      .coeff_accepted_candidates                  = 0.0,
      .coeff_accepted_matches                     = 0.0,
      .coeff_mcs_end1                             = 0.0,
      .coeff_mcs_end2                             = 0.0,
      .coeff_max_region_length_norm               = 0.0,
      .coeff_kmer_frequency                       = 0.0,
  },
  .mmaps_d1_logit_coeff = {
      .coeff_intercept                            = 5.3442,
      .coeff_primary_edit_distance_norm           = 0.0,
      .coeff_primary_event_distance_norm          = 0.0,
      .coeff_primary_swg_score_norm               = 0.0,
      .coeff_primary_template_size_sigma_norm     = -13.6923,
      .coeff_mapq_end1                            = 2.9236,
      .coeff_mapq_end2                            = 4.0309,
      .coeff_subdominant_edit_distance_norm       = 0.0,
      .coeff_subdominant_event_distance_norm      = 0.0,
      .coeff_subdominant_swg_score_norm           = 0.0,
      .coeff_subdominant_template_size_sigma_norm = 10.5848,
      .coeff_accepted_candidates                  = -0.3718,
      .coeff_accepted_matches                     = 0.0,
      .coeff_mcs_end1                             = 0.0,
      .coeff_mcs_end2                             = 0.0,
      .coeff_max_region_length_norm               = 0.0,
      .coeff_kmer_frequency                       = 0.0,
  },
};

/*

.coeff_intercept                            = 0.0,
.coeff_primary_edit_distance_norm           = 0.0,
.coeff_primary_event_distance_norm          = 0.0,
.coeff_primary_swg_score_norm               = 0.0,
.coeff_primary_template_size_sigma_norm     = 0.0,
.coeff_mapq_end1                            = 0.0,
.coeff_mapq_end2                            = 0.0,
.coeff_subdominant_edit_distance_norm       = 0.0,
.coeff_subdominant_event_distance_norm      = 0.0,
.coeff_subdominant_swg_score_norm           = 0.0,
.coeff_subdominant_template_size_sigma_norm = 0.0,
.coeff_accepted_candidates                  = 0.0,
.coeff_accepted_matches                     = 0.0,
.coeff_mcs_end1                             = 0.0,
.coeff_mcs_end2                             = 0.0,
.coeff_max_region_length_norm               = 0.0,
.coeff_kmer_frequency                       = 0.0,

 */

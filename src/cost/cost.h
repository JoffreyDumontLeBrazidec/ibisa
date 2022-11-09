/*
 * cost.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class COST
 * TODO: 
 *
 */

#ifndef DEF_COST
#define DEF_COST

#include <main/basic_modules.h>

#include "observationOperator/observationOperator.h"
#include "include/indexation.h"
#include "include/distribution_indexation.h"
#include "include/compute_likelihood.h"
#include "include/compute_prior.h"
#include "include/compute_cost_couples.h"
#include "include/index_bigger_costs.h"
#include "include/read.h"
#include <include/configHelper.h>
#include <include/walk_conditions.h>

namespace ibisa {

    class Cost
    {

        public:
            // --------------------------------
            // Constructeur-Destructeur
            Cost (Configuration & config, int & temperature);

            ~Cost ();
  
            // Méthodes
            void                    update_variables                    (const Eigen::VectorXd & X);
            void                    update_number_impusions             (const int & current_number_impulsions);
            void                    update_indexing_observationerror    (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror);

            void                    compute_predictions                 (const ibisa::observationOperator & U, const bool & update_all);
            double                  compute_cost                        (const Eigen::VectorXd & X, ibisa::observationOperator & U);
            Eigen::VectorXd         get_predictions                     ();

        private:
            // --------------------------------------
            // Attributs
            // Constants
            Eigen::VectorXi         m_Index;
            Eigen::VectorXi         m_Length;
            int                     m_temperature;
            int                     m_beg_q;                
            int                     m_number_impulsions_meteo;
            Eigen::VectorXd         m_meteo;
            bool                    m_interpolation_meteo;
            std::string             m_trans;

                // observations
            Eigen::MatrixXd         m_y; 
            int                     m_number_obs; 
            bool                    m_reduction_number_obs;
            double                  m_decision_remove_obs;            

                // distributions
            Eigen::VectorXi         m_index_priors;
            double                  m_log_qb;
            double                  m_k;
            int                     m_likelihood;
            double                  m_yt;
            double                  m_scale_prior_on_borders;
            Eigen::MatrixXd         m_Markov_Transitions;

            // variables
                // source state
            double                  m_x1;
            double                  m_x2; 
            Eigen::VectorXd         m_log_q;
            int                     m_current_number_imp;
            Eigen::VectorXd         m_r;            
            Eigen::VectorXd         m_b;

            Eigen::VectorXd         m_yS;

                // other variables
            Eigen::VectorXi         m_index_observationerror;
            Eigen::VectorXi         m_size_indexing_observationerror;
            Eigen::MatrixXi         m_basic_index_observationerror;
            Eigen::VectorXi         m_labels_regPriorTerm;

    };
} // namespace ibisa
#endif

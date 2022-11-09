/*--------------------------------------------------
 * /IBISA/
 * MarkovState.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class MARKOVSTATE
 * TODO: 
 *
 */

#ifndef DEF_MARKOVSTATE
#define DEF_MARKOVSTATE

#include <main/basic_modules.h>
#include <include/computePhysics.h>

#include <sortingobs/sortingObs.h>
#include <state/StandardState.h>
#include <observationOperator/observationOperator.h>
#include <cost/cost.h>
#include <include/distribution_indexation.h>
#include <include/walk_conditions.h>
#include <state/initiate_state.h>
#include <include/configHelper.h>
#include <include/symmetrise_modelTransitions.h>

namespace ibisa {

    class MarkovState

    {

        public:
            // ---------------------------------------------
            // Constructeur-Destructeur
            MarkovState     (Configuration & config, const Eigen::Vector2i & seed);  
            ~MarkovState    ();
  
            // Méthodes
            void print                                                      (int & order, ibisa::observationOperator & U) ;

            // walk, MH
            void walk                                                       (const std::string & choice_walk, ibisa::observationOperator & U);
            void monodimensional_walk                                       ();
            void walkCoordinates                                            (const int & t);
            void walkLogRelease                                             (const int & t);
            void walkVarObsErr                                              (const int & t);
            void walkWeightMet                                              (const int & t);
            void walkRegPrior                                               (const int & t);
           
            void transMove_walk                                             (ibisa::observationOperator & U);
            void birth_walk                                                 (ibisa::observationOperator & U);
            void death_walk                                                 (ibisa::observationOperator & U);
            void order_hyper_walk                                           ();
             

            void compute_cost                                               (ibisa::observationOperator & U);
            void update_adaptively_variances                                (double & mean_within_acceptance_ratio);
            void swap_states                                                (const int & t, const int & s);

            void copy                                                       (ibisa::StandardState & sX);
            void copy_t                                                     (ibisa::StandardState & sX, int t);
            void copy_indexing_obs                                          (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror);

            // r gestion
            void order_reduce_set_obs                                       ();
            bool order_enhance_set_obs                                      (const ibisa::observationOperator & U);
            void set_cost_high                                              ();
            void uniformise_hyperparameters_through_temperatures            ();
            void uniformise_observationerror                                ();
            void uniformise_regprior                                        ();

            
            // get
            Eigen::VectorXd     get_cost                                    ();            
            double              get_cost_t                                  (const int & t);

            Eigen::MatrixXd     get_state                                   ();
            Eigen::VectorXd     get_state_t                                 (const int & t);
          
            double              get_cost_transition_t                       (const int & t);

            Eigen::VectorXi     get_current_number_impulsions               ();
            int                 get_current_number_impulsions_t             (const int & t);

            Eigen::VectorXd     get_predictions                             ();
            Eigen::VectorXi     get_index_observationerror                  ();    
            Eigen::VectorXi     get_size_indexing_observationerror          ();
            double              get_double_size_indexing_observationerror   ();
            Eigen::VectorXd     get_temperatures                            ();
            void                update_cycle_number                         (int & cycle_num);

        private:
            // ----------------------------------------------------
            // Attributs
            tppda::GRNG<double,int>                     m_rng;
            tppda::URNG<double,int>                     m_ung;

            // Constant
            bool                                        m_reduction_obs;
            int                                         m_number_temperatures;
            Eigen::VectorXd                             m_temperatures;
            std::string                                 m_trans;
            Eigen::VectorXi                             m_Index;
            Eigen::VectorXi                             m_Length;
            int                                         m_number_impulsions_meteo;
            int                                         m_maxNumber_variables;
            bool                                        m_interpolation_meteo;
            double                                      m_ratio_sigmaVarObsError_VarObsError;
            double                                      m_sigma_move;
            ibisa::sortingObs                           m_sortingObsModule;
            int                                         m_cycle_num;
            int                                         m_number_b_walking;
            int                                         m_number_obs;

            // Source state, copy with sX
            Eigen::MatrixXd                             m_state;
            Eigen::VectorXi                             m_current_number_impulsions;
            Eigen::VectorXd                             m_cost;
            Eigen::VectorXd                             m_J_modelTransitions;

            // Changed through X only
            Eigen::MatrixXd                             m_Markov_Transitions;
            Eigen::VectorXd                             m_sigmas_r;
            Eigen::VectorXd                             m_sigmas_b;
            std::vector<std::unique_ptr<ibisa::Cost>>   m_pJ;
            Eigen::VectorXi                             m_size_indexing_observationerror;
            Eigen::VectorXi                             m_index_observationerror;
    };
} //namespace ibisa
#endif



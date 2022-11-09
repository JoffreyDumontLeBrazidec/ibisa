/*-------------------------------------------------
 * /IBISA/
 * MarkovState.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class MARKOVSTATE
 * TODO: 
 *
 */

#include "MarkovState.h"

namespace ibisa {

    // Constructeur-Destructeur
    MarkovState::MarkovState 
        (Configuration & config, const Eigen::Vector2i & seed)
        : m_rng (seed[0], 0, 1), m_ung (seed[1]), m_number_temperatures (config.get_integer ("init.temperature.number")), m_pJ (m_number_temperatures), m_sortingObsModule (config, seed)
        {
            cout << " MarkovState::Init ...";
            std::tie (m_Index, m_Length)                = ibisa::indexation (config);
            m_current_number_impulsions                 = Eigen::VectorXi::Constant (m_number_temperatures, config.get_integer ("search.impulsions.number")) ;

            // Basic objects : temperature, cost, state, Markov transitions 
            // --------------------------------------------------------
            double ratio_init_temperature               = config.get_real ("init.temperature.ratio.geometric");
            m_temperatures.resize                       (m_number_temperatures);
            m_cost.resize                               (m_number_temperatures);
            for (int index_temp(0) ; index_temp < m_number_temperatures ; index_temp++) 
            {
                m_temperatures[index_temp]  = std::pow (ratio_init_temperature, index_temp);
                m_cost[index_temp]          = 1000000 ; 
                m_pJ[index_temp].reset      (new ibisa::Cost (config, index_temp));
            }
     
            m_Markov_Transitions                        = ibisa::walk_conditions (config);
            m_sigmas_r                                  = m_Markov_Transitions (3,1) * Eigen::VectorXd::Ones (m_Length[3]); 
            m_sigmas_b                                  = m_Markov_Transitions (5,1) * Eigen::VectorXd::Ones (m_Length[5]);            
            
            m_state                                     = ibisa::initiate_state (config, m_Index, m_Length, m_temperatures, m_Markov_Transitions, seed);
            m_J_modelTransitions                        = Eigen::VectorXd::Zero (m_number_temperatures);
            m_trans                                     = config.get_string ("release.transdimensionality");            
            m_number_b_walking                          = config.get_integer ("init.regprior.walk.number");

            m_maxNumber_variables                       = config.get_integer ("variables.number.max");
            m_ratio_sigmaVarObsError_VarObsError        = 0.03;
            m_sigma_move                                = m_Markov_Transitions (2,1);

            m_interpolation_meteo                       = config.get_bool("init.meteo.interpolation");


            // Reduction number obs
            // --------------------------------------------------
            Eigen::VectorXi tempo_size_indexing_observationerror    = m_sortingObsModule.get_size_indexing_observationerror();
            m_size_indexing_observationerror                        = tempo_size_indexing_observationerror.transpose();
            m_reduction_obs                                         = config.get_bool ("data.reduce.obs");

            Configuration metcon        = ibisa::download_associated_CIMC (config);
            m_number_obs                = metcon.get_integer ("meteo.observations.number");
            m_number_impulsions_meteo   = metcon.get_integer ("meteo.impulsions.number");

            cout << "done!" << endl;
        }
     
    MarkovState::~MarkovState
        ()
        {}


    // ---------------------------------------- 
    // Méthodes
    // --------------------------------------
    // compute cost
    void MarkovState::compute_cost 
        (ibisa::observationOperator & U)
        {            
            # pragma omp parallel for 
            for (int t = 0 ; t < m_number_temperatures ; t++)
            {
                if (m_trans != "false")
                    m_pJ[t] -> update_number_impusions (m_current_number_impulsions[t]);
                
                m_cost [t] = m_pJ[t] -> compute_cost (m_state.row(t), U);
            }            
        }
 

    // ------------------------------------------------------------
    // copy
    void MarkovState::copy 
        (ibisa::StandardState & sX) 
        {
            m_state                         = sX.get_state                      ();
            m_cost                          = sX.get_cost                       ();
            m_current_number_impulsions     = sX.get_current_number_impulsions  ();
        }

    // ------------------------------------------------------------
    // copy t
    void MarkovState::copy_t 
        (ibisa::StandardState & sX, int t)
        {
            m_state.row(t)                  = sX.get_state_t                        (t);
            m_cost[t]                       = sX.get_cost_t                         (t);
            m_current_number_impulsions[t]  = sX.get_current_number_impulsions_t    (t);
        }
    
    // ------------------------------------------------------------
    // copy_indexing_obs
    void MarkovState::copy_indexing_obs
        (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror)
        {
            m_index_observationerror            = index_observationerror;
            m_size_indexing_observationerror    = size_indexing_observationerror;
            for (int t = 0; t < m_number_temperatures; t ++)
            {
                m_pJ[t] -> update_indexing_observationerror (m_index_observationerror, m_size_indexing_observationerror);
            }
        }

    // ------------------------------------------------------------
    // swap states
    void MarkovState::swap_states 
        (const int & t, const int & s)
        {
            Eigen::VectorXd holding_state   = m_state.row (t);
            m_state.row (t)                 = m_state.row (s);
            m_state.row (s)                 = holding_state ; 
           
            double holding_cost             = m_cost [t];
            m_cost[t]                       = m_cost [s];
            m_cost[s]                       = holding_cost; 

            int holding_imp                 = m_current_number_impulsions [t];
            m_current_number_impulsions [t] = m_current_number_impulsions [s];
            m_current_number_impulsions [s] = holding_imp;
        }

    // ------------------------------------------------------------
    // order reduce set obs
    void MarkovState::order_reduce_set_obs
        ()
        {
            Eigen::VectorXd yS                      = m_pJ[0] -> get_predictions ();
            m_sortingObsModule.reduce_set_obs       (yS);
            m_index_observationerror                = m_sortingObsModule.get_index_observationerror() ;
            m_size_indexing_observationerror        = m_sortingObsModule.get_size_indexing_observationerror();

            for (int t = 0 ; t < m_number_temperatures ; t ++)
            {
                m_pJ[t] -> update_indexing_observationerror (m_index_observationerror, m_size_indexing_observationerror);
            }
        }

    // ------------------------------------------------------------
    // order enhance set obs
    bool MarkovState::order_enhance_set_obs 
        (const ibisa::observationOperator & U)
        {            
            bool update_all_predictions             = true;
            m_pJ[0]                                 -> compute_predictions (U, update_all_predictions);
            
            Eigen::VectorXd yS                      = m_pJ[0] -> get_predictions ();           
            bool indicator_model_change             = m_sortingObsModule.enhance_set_obs (yS);
            m_index_observationerror                = m_sortingObsModule.get_index_observationerror() ;
            m_size_indexing_observationerror        = m_sortingObsModule.get_size_indexing_observationerror();

            for (int t = 0; t < m_number_temperatures; t ++)
            {
                m_pJ[t] -> update_indexing_observationerror (m_index_observationerror, m_size_indexing_observationerror);
            }            
            return indicator_model_change;
        }

    // ------------------------------------------------------------
    // set cost high
    void MarkovState::set_cost_high 
        ()
        {
            for (int t = 0 ; t < m_number_temperatures ; t ++)
            {
                m_cost[t] = std::abs (m_cost[t]) * 10;
            }
        }

    // ------------------------------------------------------------
    // A code to solve a problem which appears because when we uniformise the observation error in each temperature, we do not compute the cost associated to this uniformisation to save time
    // We need sometimes to uniformise the observation error 
    void MarkovState::uniformise_observationerror 
        ()
        {
            // Exception with hyperparameter r
            if (m_Index[3]!=-1)
            {
                // observation error r at temperature != 0 are set to be observation error r at temperature 0                 
                for (int t = 1  ; t < m_temperatures.size() ; t ++)
                {
                    for (int j = 0 ; j < m_Length[3] ; j++)
                    {
                        m_state (t, m_Index[3]+j) = m_state (0, m_Index[3]+j) ; 
                    }
                }
            }
        }

    // ------------------------------------------------------------
    // uniformise regularisation prior term
    void MarkovState::uniformise_regprior
        ()
        {
            if (m_Index[5] != -1)
            {
                for (int t = 1  ; t < m_temperatures.size() ; t ++)
                {
                    for (int j = 0 ; j < m_Length[5] ; j++)
                    {
                        m_state (t, m_Index[5]+j) = m_state (0, m_Index[5]+j) ; 
                    }
                }
            }
        }
   
    // ------------------------------------------------------------
    // print
    void MarkovState::print 
        (int & order, ibisa::observationOperator & U)
        {
            cout << endl;
            int number_temperatures_to_print = m_number_temperatures;
            if (order == 0)
                number_temperatures_to_print = 1;
                
            Eigen::VectorXd trra    = Eigen::VectorXd::Zero (number_temperatures_to_print);
            Eigen::VectorXd trra_c  = Eigen::VectorXd::Zero (number_temperatures_to_print);
            Eigen::VectorXd trra_nc = Eigen::VectorXd::Zero (number_temperatures_to_print);           

            if (m_Index[2] != -1)
            {
                # pragma omp parallel for            
                for (int t = 0; t < number_temperatures_to_print; t ++)
                {
                    trra [t] = ibisa::computeTRRA (m_state.row(t).segment (m_Index[2], m_current_number_impulsions[t]), U.release_grid_borders(t), U.get_release_period(), m_trans);
                    if (m_Length[5] == 2)
                    {
                        trra_nc [t]     = ibisa::computeTRRANContraint (m_state.row(t).segment (m_Index[2], m_current_number_impulsions[t]), U.release_grid_borders(t), U.get_release_period(), m_trans);
                        trra_c [t]      = trra [t] - trra_nc[t];
                    }
                }
            }

                
            for (int t = 0 ; t < number_temperatures_to_print ; t ++) 
            {
                cout << "At temperature = " << t << " -> " ;
                
                if (m_Index[0] != -1)
                    cout << "[x1; x2] = [" << m_state (t, m_Index[0]) << "; " << m_state (t, m_Index[1]) << "]; ";

                if (m_Index[2] != -1)
                {
                    cout << "[ln(q)] = [";
                    for (int imp = 0 ; imp < m_current_number_impulsions[t]-1; imp ++)
                    {                            
                        cout << m_state.block (t,m_Index[2]+imp,1,1) << ", ";
                    }
                    cout << m_state (t, m_Index[2] + m_current_number_impulsions[t]-1) << "]";
                    cout << "; trra = " << trra[t] << " Bq" ;
                    if (m_Length[5]==2)
                    {
                        cout << "; trra_c = " << trra_c[t] << " Bq" << "; trra_nc = " << trra_nc [t] << " Bq";
                    }
                }
                
                if (m_Index[3] != -1)
                {
                    cout << " ; [r] = [" <<  m_state.block (t, m_Index[3], 1, m_Length[3]) << "]";
                }
                    
                if (m_Index[4] != -1)
                {
                    cout << endl << "[Met. w] = [" << m_state.block (t, m_Index[4], 1, m_Length[4]) << "]"; 
                }
 
                if (m_Index[5] != -1)
                {
                    cout << endl << "[b] = [" << m_state.block (t, m_Index[5], 1, m_Length[5]) << "]"; 
                }
                   
                if (m_size_indexing_observationerror.size() > 1)
                {
                    cout << " ;  m_size_indexing_observationerror = " << m_size_indexing_observationerror.transpose(); 
                } 
 
                if (m_trans != "false")
                {
                    cout << " ;  U.release_grid_borders = ["; 
                    for (int index_border = 0 ; index_border < m_current_number_impulsions[t] ; index_border ++)
                    {                                               
                        cout << U.release_grid_borders (t) [index_border] << ", ";
                    }
                    cout << U.release_grid_borders (t) [m_current_number_impulsions[t]] << "]";
                    cout << " ; Nborders = " << m_current_number_impulsions[t];
                }
                            
                cout << "; [cost] = " << m_cost [t] << endl;;
                cout << "; [sigmas] = " << m_Markov_Transitions(2,1) << " " << m_sigmas_r.transpose() << " " << m_sigmas_b.transpose() << endl;
            }
        }

    // ------------------------------------------------------------
    // update_cycle_number
    void MarkovState::update_cycle_number
        (int & cycle_num)
        {
            m_cycle_num = cycle_num;
        }

    // ------------------------------------------------------------
}












































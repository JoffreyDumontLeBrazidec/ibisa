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

    // ---------------------------------------- 
    // Méthodes
    // --------------------------------------
    // walk choice, if MarkovState 
    void MarkovState::walk 
        (const std::string & choice_walk, ibisa::observationOperator & U)    
        {      
            if (choice_walk == "mono-dim")
            {
                monodimensional_walk ();
            }

            if (choice_walk == "move")
            {
                transMove_walk (U);
            }

            if (choice_walk == "birth")
            {
                birth_walk (U);
            }

            if (choice_walk == "death")                
            {
                death_walk (U);
            }            
            
            if (choice_walk == "hyper")
            {
                order_hyper_walk ();
            }
        }

    // ------------------------------------------------------------
    // MarkovState::walk  for each temperature t ; each variable i ; each component j of the variable i
    // mono dimensional walk
    void MarkovState::monodimensional_walk 
        ()
        {
            # pragma omp parallel for 
            for (int t = 0  ; t < m_number_temperatures ; t ++)
            {
                walkCoordinates (t);
                walkLogRelease  (t);
                walkVarObsErr   (t);
                walkWeightMet   (t);
                walkRegPrior    (t); 
                m_J_modelTransitions[t] = 0;
            }           
                uniformise_hyperparameters_through_temperatures ();
        }

    // ------------------------------------------------------------
    // walkCoordinates
    void MarkovState::walkCoordinates 
        (const int & t)
        {
            if (m_Index[0] != -1)
            {
                for (int i = 0 ; i < 2 ; i ++)
                {
                    m_state (t, m_Index[i])    = m_state (t , m_Index[i]) + m_rng.randn() * m_Markov_Transitions(i,1) * std::sqrt (m_temperatures (t)) ;   
                
                    // condition at borders
                    while (m_state (t , m_Index[i]) < m_Markov_Transitions(i,2) or m_state (t , m_Index[i]) > m_Markov_Transitions(i,3))
                    {
                        if (m_state (t, m_Index[i]) < m_Markov_Transitions(i,2))
                        {
                            m_state (t, m_Index[i]) = 2 * m_Markov_Transitions(i,2) - m_state (t, m_Index[i]);
                        }

                        if (m_state (t, m_Index[i]) > m_Markov_Transitions(i,3)) 
                        {
                            m_state (t, m_Index[i]) = 2 * m_Markov_Transitions(i,3) - m_state (t, m_Index[i]) ;
                        }
                    }
                }
            }
        }

    // ------------------------------------------------------------
    // walkLogRelease
    void MarkovState::walkLogRelease
        (const int & t)
        {
            if (m_Index[2] != -1)
            {
                for (int time_release = 0 ; time_release < m_current_number_impulsions[t] ; time_release ++)
                {
                    m_state (t, m_Index[2] + time_release) = m_state (t, m_Index[2] + time_release) + m_rng.randn() *  m_Markov_Transitions(2,1) *  std::sqrt (m_temperatures (t));
                            
                    // condition at borders
                    if (m_state (t, m_Index[2]+time_release) < m_Markov_Transitions(2,2))
                    {
                        m_state (t, m_Index[2]+time_release) = 2 * m_Markov_Transitions(2,2) - m_state (t, m_Index[2]+time_release);
                    }
                }
            }    
        }

    // ------------------------------------------------------------
    // walkVarObsErr
    void MarkovState::walkVarObsErr
        (const int & t)
        {
            if (m_Index[3] != -1 and t==0)
            {
                for (int j = 0 ; j < m_Length[3] ; j++)
                {
                    m_state (t, m_Index[3]+j) = m_state (t, m_Index[3]+j) +  m_rng.randn() * m_sigmas_r[j] *  std::sqrt (m_temperatures(t));  
                    
                    // condition at borders
                    if (m_state (t, m_Index[3]+j) < m_Markov_Transitions(3,2))
                    {
                        m_state (t, m_Index[3]+j) = 2*m_Markov_Transitions(3,2) - m_state (t, m_Index[3]+j);
                    }
                }
            }   
        }

    // ------------------------------------------------------------
    // walkWeightMet
    void MarkovState::walkWeightMet
        (const int & t)
        {
            if (m_Index[4] != -1 and m_interpolation_meteo == true)
            {    
                // update weight variables
                for (int j = 0 ; j < m_Length[4] ; j ++)
                {
                    m_state (t, m_Index[4]+j) = m_state (t, m_Index[4]+j) +  m_rng.randn() * m_Markov_Transitions(4,1) * std::sqrt (m_temperatures(t));  
               
                    while (m_state (t, m_Index[4]+j) < m_Markov_Transitions(4,2) or m_state (t , m_Index[4]+j) > m_Markov_Transitions(4,3))
                    {
                        if (m_state (t, m_Index[4]+j) < m_Markov_Transitions(4,2))
                        {
                            m_state (t, m_Index[4]+j) = 2*m_Markov_Transitions(4,2) - m_state (t, m_Index[4]+j);
                        }

                        if (m_state (t, m_Index[4]+j) > m_Markov_Transitions(4,3)) 
                        {
                            m_state (t , m_Index[4]+j) = 2*m_Markov_Transitions(4,3) - m_state (t, m_Index[4]+j) ;
                        }
                    }
                }
 
                // total weight
                double wmet_tot = 0;
                for (int l = 0 ; l < m_Length[4] ; l ++)
                {
                    wmet_tot = wmet_tot +  m_state (t, m_Index[4]+l);
                }     
          
                // divide each weight by the total of their sum
                for (int l = 0 ; l < m_Length[4] ; l ++) 
                {
                    m_state (t, m_Index[4]+l) = m_state (t, m_Index[4]+l) / wmet_tot ;
                }   

            }
        }

    // ------------------------------------------------------------
    // walkRegPrior
    void MarkovState::walkRegPrior
        (const int & t)
        {
            if (m_Index[5] != -1 and t == 0 and m_cycle_num > 5000)
            {
                for (int j = 0 ; j < std::min (m_number_b_walking, m_Length[5]) ; j++)
                {
                    m_state (t, m_Index[5]+j) = m_state (t, m_Index[5]+j) +  m_rng.randn() * m_sigmas_b[j] * std::sqrt (m_temperatures(t))  ;  
                    if (m_state (t, m_Index[5]+j) < m_Markov_Transitions (5,2))
                    {
                        m_state (t, m_Index[5]+j) = 2 * m_Markov_Transitions (5,2) - m_state (t, m_Index[5]+j);
                    }                    
                }
            }
            if (m_cycle_num < 5000 and m_Length[5] == 2)
            {
                m_state (t, m_Index[5] + 1) = 1;
            }
        }

    // ------------------------------------------------------------
    // uniformise_hyperparameters_through_temperatures
    void MarkovState::uniformise_hyperparameters_through_temperatures
        ()
        {
            // Hyperparameter r 
            if (m_Index[3]!=-1)
            {
                // before burn-in, set r_{nd}==r_{d}
                if (m_reduction_obs == true and m_size_indexing_observationerror [m_Length[3]-1] == 0)
                {
                    m_state (0, m_Index[3]+m_Length[3]-1) = m_state (0, m_Index[3]) ;
                }
                
                // observation error r at temperature != 0 are set to be observation error r at temperature 0 
                for (int t = 1; t < m_number_temperatures; t ++)
                {
                    for (int j = 0; j < m_Length[3]; j++)
                    {
                        m_state (t, m_Index[3]+j) = m_state (0, m_Index[3]+j) ; 
                    }
                }               
            }
            
            // Hyperparameter b
            if (m_Index[5]!=-1)
            {
                // regularisation prior term b at temperature != 0 are set to be regularisation prior term b at temperature 0 
                for (int t = 1; t < m_number_temperatures; t ++)
                {
                    for (int j = 0; j < m_Length[5]; j++)
                    {
                        m_state (t, m_Index[5]+j) = m_state (0, m_Index[5]+j) ; 
                    }
                }               
            }
        }

    // ------------------------------------------------------------
    // transMove_walk
    void MarkovState::transMove_walk
        (ibisa::observationOperator & U)
        {
                Eigen::VectorXi index_moving_border_in_vec_borders  (m_number_temperatures);
                Eigen::VectorXi magnitude_move                      (m_number_temperatures);
                Eigen::VectorXd magnitude_random                    (m_number_temperatures);
                
                //# pragma omp parallel for 
                for (int t = 0  ; t < m_number_temperatures ; t ++)
                {
                    // evaluate border moving
                    index_moving_border_in_vec_borders[t]       = std::floor (m_ung.rand(1)*(U.number_borders(t)-2)+1);        
                    magnitude_random[t]                         = m_rng.randn();
                    magnitude_move[t]                           = magnitude_random[t] / std::abs (magnitude_random[t]);                    
                  
                    while (     U.release_grid_borders (t) [index_moving_border_in_vec_borders[t]] + magnitude_move[t] ==  U.release_grid_borders(t) [index_moving_border_in_vec_borders[t]-1]
                            or  U.release_grid_borders (t) [index_moving_border_in_vec_borders[t]] + magnitude_move[t] ==  U.release_grid_borders(t) [index_moving_border_in_vec_borders[t]+1] )
                    {                        
                        index_moving_border_in_vec_borders[t]   = std::floor (m_ung.rand(1)*(U.number_borders(t)-2)+1);     // neither the first, neither the last border can be changed
                        magnitude_random[t]                     = m_rng.randn();
                        magnitude_move[t]                       = magnitude_random[t] / std::abs (magnitude_random[t]);
                    }
                    
                    // Update U
                    U.moving_update (t, U.release_grid_borders (t) [index_moving_border_in_vec_borders[t]], U.release_grid_borders (t) [index_moving_border_in_vec_borders[t]] + magnitude_move[t]);
                    m_J_modelTransitions[t] = 0;
                }
        }
            
    // ------------------------------------------------------------
    // birth_walk : birth of a new border (and death of a city)
    void MarkovState::birth_walk
        (ibisa::observationOperator & U)
        {                                         
                Eigen::VectorXi index_moving_border                 = Eigen::VectorXi::Zero (m_number_temperatures);
                Eigen::VectorXd release_noise                       (m_number_temperatures);
                Eigen::VectorXi index_new_border_in_vec_cities      (m_number_temperatures);

                # pragma omp parallel for 
                for (int t = 0 ; t < m_number_temperatures ; t ++)
                {
                    if (m_current_number_impulsions[t] < m_Length[2])
                    {
                        // Choose where border is born
                        index_new_border_in_vec_cities[t]   = std::floor (m_ung.rand(1) * (U.number_cities(t)));

                        // modify borders_grid, cities_grid, U
                        index_moving_border[t] = U.birth_update (t, index_new_border_in_vec_cities[t]); 

                        // modify state
                        for (int index_impulsion = m_current_number_impulsions[t] ; index_impulsion > index_moving_border[t]-1 ; index_impulsion --)
                        {
                            m_state (t, m_Index[2] + index_impulsion) = m_state (t, m_Index[2] + index_impulsion - 1);
                        }                   
                        release_noise[t]                                    = m_rng.randn() * m_sigma_move;
                        m_state (t, m_Index[2] + index_moving_border[t])    = m_state (t, m_Index[2] + index_moving_border[t]) - release_noise[t] ;                    
                        m_state (t, m_Index[2] + index_moving_border[t]-1)  = m_state (t, m_Index[2] + index_moving_border[t]-1) + release_noise[t] ;
                    
                        // adapt m_current_number_impulsions[t] of state and cost                    
                        m_current_number_impulsions[t]  = m_current_number_impulsions[t]+1;

                        // compute the J_transitions and send to pJ
                        m_J_modelTransitions[t]    = ibisa::symmetrise_modelTransitions ("birth", U.number_cities(t), U.number_borders(t), release_noise[t], m_sigma_move);
                    }
                }
        }
 
    // ------------------------------------------------------------
    // death_walk : death of a border (and birth of a city)
    void MarkovState::death_walk
        (ibisa::observationOperator & U)
        {           
                Eigen::VectorXi index_moving_border                 (m_number_temperatures);               
                Eigen::VectorXi index_old_border_in_vec_borders     (m_number_temperatures);
                Eigen::VectorXd release_noise                       (m_number_temperatures);

                # pragma omp parallel for 
                for (int t = 0; t < m_number_temperatures; t ++)
                {
                    if (m_current_number_impulsions[t] > 2)
                    {
                        // evaluate border dying:
                        index_old_border_in_vec_borders[t]  = std::floor (m_ung.rand(1) * (U.number_borders(t)-2)+1); 
                                   
                        // modify borders_grid, cities_grid, U
                        U.death_update (t, index_old_border_in_vec_borders[t]);
                   
                        
                        release_noise[t]    = (m_state (t, m_Index[2] + index_old_border_in_vec_borders[t]-1) - m_state (t, m_Index[2] + index_old_border_in_vec_borders[t])) / 2;
                        release_noise[t]    = std::pow (release_noise[t], 2);

                        // modify state
                        m_state (t, m_Index[2] + index_old_border_in_vec_borders[t]-1) 
                            = (m_state (t, m_Index[2] + index_old_border_in_vec_borders[t]-1) + m_state (t, m_Index[2] + index_old_border_in_vec_borders[t]))/2;

                        for (int index_impulsion = index_old_border_in_vec_borders[t] ; index_impulsion < m_current_number_impulsions[t] - 1 ; index_impulsion ++)
                        {
                            m_state (t, m_Index[2] + index_impulsion) = m_state (t, m_Index[2] + index_impulsion + 1);
                        }                   
                        // adapt m_current_number_impulsions[t] of state and cost
                        m_current_number_impulsions[t]  = m_current_number_impulsions[t]-1;

                        // compute the J_transitions and send to pJ
                        m_J_modelTransitions[t]    = ibisa::symmetrise_modelTransitions ("death", U.number_cities(t), U.number_borders(t), release_noise[t], m_sigma_move);
                    }
               } 
        }

    // ------------------------------------------------------------
    // hyper_walk : transition of index of observation error variance hyperparameters
    void MarkovState::order_hyper_walk
        ()
        {
            m_sortingObsModule.general_sorting_walk     ();
            m_index_observationerror                    = m_sortingObsModule.get_index_observationerror();
            m_size_indexing_observationerror            = m_sortingObsModule.get_size_indexing_observationerror();
            for (int t = 0; t < m_number_temperatures; t ++)
            {
                m_pJ[t] -> update_indexing_observationerror (m_index_observationerror, m_size_indexing_observationerror);
            }
        }

    // ------------------------------------------------------------
    // update adaptively variances
    void MarkovState::update_adaptively_variances
        (double & mean_within_acceptance_ratio)
        {
            double rate_decay_variances = 1;
            if (mean_within_acceptance_ratio < 0.1)
            {
                rate_decay_variances = 7.5 * mean_within_acceptance_ratio + 0.25; 
            }

            if (mean_within_acceptance_ratio > 0.5)
            {
                rate_decay_variances = 2 * mean_within_acceptance_ratio;          
            }

            for (int index_var = 0; index_var < m_maxNumber_variables; index_var ++)           
            {    
                if (m_Index[index_var]!=-1 and index_var != 3 and index_var !=5)
                {
                    m_Markov_Transitions (index_var,1) = rate_decay_variances * m_Markov_Transitions (index_var,1); 
                }
            }

            // For hyperparameter r, sigma is infered from ratio_sigmaVarObsError_VarObsError * r 
            if (m_Index[3]!=-1 and m_Length[3] > 2)
            {
                for (int diag_coeff = 0; diag_coeff < m_Length[3]; diag_coeff++)
                {
                    m_sigmas_r [diag_coeff] = m_ratio_sigmaVarObsError_VarObsError * m_state (0, m_Index[3] + diag_coeff);
                }            
            }

            // For hyperparameter b active and prior Gaussian, sigma is infered from value of the release 
            if (m_Index[5]!=-1 and m_Markov_Transitions(5,0) == 4)
            {
                for (int diag_coeff = 0; diag_coeff < m_Length[5]; diag_coeff++)
                {               
                    m_sigmas_b [diag_coeff] = m_ratio_sigmaVarObsError_VarObsError * m_state (0, m_Index[5] + diag_coeff);
                }
            }       
        }

    // ----------------------------------------------------------------------------------------
}












































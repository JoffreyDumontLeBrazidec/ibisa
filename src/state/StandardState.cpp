/*-------------------------------------------------
 * /IBISA/
 * StandardState.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class StandardState
 * TODO: 
 *
 */

#include "state/StandardState.h"

namespace ibisa {

    // Constructeur-Destructeur
    StandardState::StandardState 
        (Configuration & config, const Eigen::MatrixXd & state, const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror)
        : m_number_temperatures (config.get_integer ("init.temperature.number"))
        {
            std::tie (m_Index, m_Length)    = ibisa::indexation (config);
            m_current_number_impulsions     = Eigen::VectorXi::Constant (m_number_temperatures, config.get_integer ("search.impulsions.number")) ;

            double ratio_init_temperature   = config.get_real ("init.temperature.ratio.geometric");
            m_temperatures.resize           (m_number_temperatures);
            m_cost.resize                   (m_number_temperatures);
            
            for (int index_temp(0) ; index_temp < m_number_temperatures ; index_temp++) 
            {
                m_temperatures[index_temp]  = std::pow (ratio_init_temperature, index_temp);
                m_cost[index_temp]          = 1000000 ; 
            }
     
            m_state                                                 = state;
            m_size_indexing_observationerror                        = size_indexing_observationerror;
            m_index_observationerror                                = index_observationerror;
        }
     
    StandardState::~StandardState
        ()
        {}


    // --------------------------------------
    // Méthodes
    // --------------------------------------
    // copy
    void StandardState::copy 
        (const Eigen::MatrixXd & state, const Eigen::VectorXd & cost, const Eigen::VectorXi & current_number_impulsions) 
        {
            m_state                             = state;
            m_cost                              = cost;
            m_current_number_impulsions         = current_number_impulsions;
        }

    // ------------------------------------------------------------
    // copy t
    void StandardState::copy_t 
        (const int & t, const Eigen::VectorXd & state, const double & cost, const int & current_number_impulsions) 
        {
            m_state.row(t)                  = state;
            m_cost[t]                       = cost;
            m_current_number_impulsions[t]  = current_number_impulsions;
        }

    // ------------------------------------------------------------
    // copy_indexing_obs
    void StandardState::copy_indexing_obs 
        (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror) 
        {
            m_index_observationerror            = index_observationerror;
            m_size_indexing_observationerror    = size_indexing_observationerror;
        } 
    
    // ------------------------------------------------------------
    // StandardState::get_cost
    Eigen::VectorXd StandardState::get_cost
        ()
        {   
            return m_cost ;
        }

    // ------------------------------------------------------------
    // StandardState::get_cost_t
    double StandardState::get_cost_t
        (const int & t)
        {   
            return m_cost[t] ;
        }

    // ------------------------------------------------------------
    // StandardState::get_state
    Eigen::MatrixXd StandardState::get_state
        ()
        {   
            return m_state ;
        }

    // ------------------------------------------------------------
    // StandardState::get_state_t
    Eigen::VectorXd StandardState::get_state_t
        (const int & t)
        {   
            return m_state.row(t) ;
        }

    // ------------------------------------------------------------
    // StandardState::current_number_impulsions
    Eigen::VectorXi StandardState::get_current_number_impulsions
        ()
        {   
            return m_current_number_impulsions;
        }

    // ------------------------------------------------------------
    // StandardState::get_state_t
    int StandardState::get_current_number_impulsions_t
        (const int & t)
        {   
            return m_current_number_impulsions [t];
        }

    // ------------------------------------------------------------
    // set cost high
    void StandardState::set_cost_high
        ()
        {
            for (int t = 0; t < m_number_temperatures; t ++)
            {
                m_cost[t] = std::abs (m_cost[t]) * 10;
            }
        }
            
    // ------------------------------------------------------------
    // get index observationerror    
    Eigen::VectorXi StandardState::get_index_observationerror
        ()
        {
            return m_index_observationerror;
        }

    // ------------------------------------------------------------
    // get size indexing observationerror
    Eigen::VectorXi StandardState::get_size_indexing_observationerror
        ()
        {
            return m_size_indexing_observationerror;
        }

    // ------------------------------------------------------------

}




















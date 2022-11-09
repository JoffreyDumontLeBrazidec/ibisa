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
    // ------------------------------------------------------------
    // MarkovState::get_cost
    Eigen::VectorXd MarkovState::get_cost 
        ()
        {
            return m_cost;
        }

    // ------------------------------------------------------------
    // MarkovState::get_cost_t
    double MarkovState::get_cost_t 
        (const int & t)
        {
            return m_cost[t];
        }

    // ------------------------------------------------------------
    // MarkovState::get_cost_t
    double MarkovState::get_cost_transition_t
        (const int & t)
        {
            return m_J_modelTransitions[t];
        }

    // ------------------------------------------------------------
    // MarkovState::get_state
    Eigen::MatrixXd MarkovState::get_state 
        ()
        {
            return m_state;
        }

    // ------------------------------------------------------------
    // MarkovState::get_state_t
    Eigen::VectorXd MarkovState::get_state_t 
        (const int & t)
        {
            return m_state.row(t);
        }

    // ------------------------------------------------------------
    // MarkovState::current_number_impulsions
    Eigen::VectorXi MarkovState::get_current_number_impulsions
        ()
        {
            return m_current_number_impulsions;
        }

    // ------------------------------------------------------------
    // MarkovState::get_state_t
    int MarkovState::get_current_number_impulsions_t
        (const int & t)
        {
            return m_current_number_impulsions [t];
        }

    // ------------------------------------------------------------
    // MarkovState::get_temperatures
    Eigen::VectorXd MarkovState::get_temperatures
        ()
        {
            return m_temperatures ;
        }

    // ------------------------------------------------------------
    // get predictions
    Eigen::VectorXd MarkovState::get_predictions 
        ()
        {
            return m_pJ[0] -> get_predictions();
        }

    // ------------------------------------------------------------
    // get double size indexing observaionerror
    double MarkovState::get_double_size_indexing_observationerror
        ()
        {
            int a       = m_size_indexing_observationerror[0];
            double b    = static_cast<double> (a) ;
            return b;
        }

    // ------------------------------------------------------------
    // get index observationerror    
    Eigen::VectorXi MarkovState::get_index_observationerror
        ()
        {
            return m_index_observationerror;
        }

    // ------------------------------------------------------------
    // get size indexing observationerror
    Eigen::VectorXi MarkovState::get_size_indexing_observationerror
        ()
        {
            return m_size_indexing_observationerror;
        }

    // ------------------------------------------------------------

}












































/*--------------------------------------------------
 * /IBISA/include/
 * symmetrise_modelTransitions.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class compute_likelihood
 * TODO: 
 *
 */


#include <include/symmetrise_modelTransitions.h>

namespace ibisa
{    
    //----------------------------------------
    // symmetrise_modelTransitions
    double symmetrise_modelTransitions 
        (const std::string & transition_type, const int & number_cities, const int & number_borders, const double & u_lnq, const double & sigmaBirthLogRelease)
        {
            double J_transitions    = 0;
            double J_Jacobien       = 0;

            if (transition_type == "birth")
            {
                J_transitions   = - std::log (number_cities) + std::log (number_borders-2) - std::pow (u_lnq, 2) / (2*sigmaBirthLogRelease)
                                - 1/2 * std::log (sigmaBirthLogRelease) - 1/2 * std::log (2*3.14);
                J_Jacobien      = - std::log (2);
            }
            
            if (transition_type == "death")
            {
                J_transitions   = + std::log (number_cities) - std::log (number_borders-2) + std::pow (u_lnq, 2) / (2*sigmaBirthLogRelease)
                                + 1/2 * std::log (sigmaBirthLogRelease) + 1/2 * std::log (2*3.14);
                J_Jacobien      = std::log (2);
            }
            
            return J_transitions + J_Jacobien;
        }

     //----------------------------------------

} // namespace ibisa











































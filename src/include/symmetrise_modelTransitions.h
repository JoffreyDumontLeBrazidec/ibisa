/*--------------------------------------------------
 * /IBISA/include/
 * symmetrise_modelTransitions.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class symmetrise_modelTransitions
 * TODO: 
 *
 */

#ifndef DEF_symmetrise_modelTransitions
#define DEF_symmetrise_modelTransitions

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // symmetrise_modelTransitions
    double symmetrise_modelTransitions 
        (const std::string & transition_type, const int & number_cities, const int & number_borders, const double & u_lnq, const double & sigmaBirthLogRelease);

     //----------------------------------------

} // namespace ibisa

#endif















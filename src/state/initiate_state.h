/*--------------------------------------------------
 * /IBISA/state/
 * initiate_state.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class initiate_state
 * TODO: 
 *
 */

#ifndef DEF_initiate_state
#define DEF_initiate_state

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    
    Eigen::MatrixXd 
        initiate_state (Configuration & config, const Eigen::VectorXi & m_Index, 
                        const Eigen::VectorXi & m_Length, const Eigen::VectorXd & m_temperatures, 
                        const Eigen::MatrixXd & m_Markov_Transitions, const Eigen::Vector2i & seed);

            



} // namespace ibisa

#endif















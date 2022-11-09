/*--------------------------------------------------
 * /IBISA/include/
 * walk_conditions.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class WALK_CONDITIONS
 * TODO: 
 *
 */

#ifndef DEF_WALK_CONDITIONS
#define DEF_WALK_CONDITIONS

#include <main/basic_modules.h>
#include <include/distribution_indexation.h>

namespace ibisa
{
    //----------------------------------------
    // function to index correctly the distributions
    // Uniform      := 0
    // Gaussian     := 1
    // Log-normal   := 2
    // Log-gamma    := 3

    Eigen::MatrixXd walk_conditions(Configuration & config);


} // namespace ibisa

#endif















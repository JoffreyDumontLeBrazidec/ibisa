/*--------------------------------------------------
 * /IBISA/include/
 * generate_aleatory.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of function generate_aleatory
 * TODO: 
 *
 */

#ifndef DEF_generate_aleatory
#define DEF_generate_aleatory

#include "main/basic_modules.h"
#include <include/distribution_indexation.h>

namespace ibisa
{

    double generate_aleatory();
    
    Eigen::Vector2i generate_seed();

} // namespace ibisa

#endif















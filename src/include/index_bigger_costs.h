/*--------------------------------------------------
 * /IBISA/include/
 * index_bigger_costs.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class index_bigger_costs
 * TODO: 
 *
 */

#ifndef DEF_index_bigger_costs
#define DEF_index_bigger_costs

#include <iostream>
#include <string>

#include <Eigen/Dense>
// library for random numbers
#define TPPDA_USE_TPPRANDOM

// library for linear algebra
#define TPPDA_USE_TPPLINALG
#define TPPLINALG_USE_GENERIC_BLAS

#include <tppda/tppda.cpp>

using Configuration             = tppda::TPPDAConfiguration<double, int, int> ;

namespace ibisa
{
    //----------------------------------------

    const Eigen::VectorXd find_max_values (  const Eigen::VectorXd & J_likelihood , const int & number_higherrorobs) ;
            



} // namespace ibisa

#endif















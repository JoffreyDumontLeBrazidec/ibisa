/*--------------------------------------------------
 * /IBISA/include/
 * compute_cost_couples.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class compute_cost_couples
 * TODO: 
 *
 */

#ifndef DEF_compute_cost_couples
#define DEF_compute_cost_couples

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
    // function to compute the likelihood according to the distribution
    
    // Gaussian     := 1
    // Log-normal   := 2
    // Log-Logistic := 8
    // Logistic     := 9
    // Log-Laplace  := 10
    // LOg-Cauchy   := 11

    const Eigen::VectorXd compute_cost_couples (   const int & index_distribution ,         
                                        const double & m_yt , const Eigen::MatrixXd & m_y , const Eigen::VectorXd & m_yS , const Eigen::VectorXd & m_r ,
                                        const Eigen::VectorXi & m_index_observationerror, const Eigen::VectorXi & m_size_indexing_observationerror ) ;

            



} // namespace ibisa

#endif















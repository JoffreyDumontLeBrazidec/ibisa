/*--------------------------------------------------
 * /IBISA/include/
 * compute_likelihood.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class compute_likelihood
 * TODO: 
 *
 */

#ifndef DEF_compute_likelihood
#define DEF_compute_likelihood

#include <main/basic_modules.h>

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

    double compute_likelihood ( int & index_distribution ,         
                                        double & m_yt , Eigen::MatrixXd & m_y , Eigen::VectorXd & m_yS , Eigen::VectorXd & m_r ,
                                        Eigen::VectorXi & m_index_observationerror, Eigen::VectorXi & m_size_indexing_observationerror ) ;

            



} // namespace ibisa

#endif















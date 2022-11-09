/*--------------------------------------------------
 * /IBISA/include/
 * compute_prior.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class compute_prior
 * TODO: 
 *
 */

#ifndef DEF_compute_prior
#define DEF_compute_prior

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // function to compute the prior according to the distribution
    
    
    double compute_prior_on_log_release
        (const int & index_prior, const int & m_current_number_imp, const Eigen::VectorXi & m_release_grid_borders, const Eigen::VectorXd & m_log_q,
        const Eigen::VectorXd & m_b, const double & m_k, const double & m_log_qb, const double & min_log_q, const double & max_log_q, const Eigen::VectorXi & m_labels_regPriorTerm, std::string & m_trans);

    double compute_prior_on_R
        (const int & index_prior, const Eigen::VectorXd & m_r, const int & m_number_obs);

    double compute_prior_on_B
        (const int & index_prior, const Eigen::VectorXd & m_b);

    double compute_prior_on_grid_borders
        (const int & index_prior, const int & number_borders, const std::string & trans, const double & m_scale_prior_on_borders);

} // namespace ibisa

#endif















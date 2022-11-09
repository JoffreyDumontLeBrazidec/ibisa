/*--------------------------------------------------
 * /IBISA/include/
 * compute_prior.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class compute_prior
 * TODO: 
 *
 */


#include <include/compute_prior.h>

namespace ibisa
{

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    // compute_prior_on_log_release
    double compute_prior_on_log_release 
        (const int & index_prior, const int & m_current_number_imp, const Eigen::VectorXi & m_release_grid_borders, const Eigen::VectorXd & m_log_q, 
         const Eigen::VectorXd & m_b, const double & m_k, const double & m_log_qb, const double & min_log_q, const double & max_log_q, const Eigen::VectorXi & m_labels_regPriorTerm, std::string & m_trans)
        {
            double J_prior_on_log_q = 0;
            
            //-------------------------------------------
            // Prior on the logarithms of the release
            // uniform
            if (index_prior == 0)
            {
                //J_prior = J_prior + std::exp (m_log_q.maxCoeff()) / m_theta;
                J_prior_on_log_q = m_current_number_imp * std::log (max_log_q - min_log_q);
            }

            //........................................................           
            // Log gamma prior (m_b scale parameter, m_k shape parameter)
            if (index_prior == 3)
            {
                for (int imp = 0; imp < m_current_number_imp; imp++)
                {
                    J_prior_on_log_q = J_prior_on_log_q + std::exp (m_log_q [imp]) / std::exp (m_log_qb) - m_k * m_log_q [imp];
                }
                //J_prior_on_log_q = J_prior_on_log_q + std::exp (m_log_q.maxCoeff()) / std::exp (m_b[m_labels_regPriorTerm[0]]) - m_k * m_log_q.maxCoeff() + m_k * std::log (m_b[0]);
            }

            //........................................................           
            // Gaussian prior on the logarithms of the release
            // m_b scale parameter, m_log_qb location parameter
            if (index_prior == 1)
            {                
                double diff;
                // m_trans False
                if (m_trans == "false")
                {
                    for (int imp = 0; imp < m_current_number_imp; imp ++)
                    {
                        diff                = m_log_q[imp] - m_log_qb;
                        J_prior_on_log_q    = J_prior_on_log_q + 0.5 * std::pow (diff, 2) / m_b [m_labels_regPriorTerm[imp]];
                        J_prior_on_log_q    = J_prior_on_log_q + 0.5 * std::log (2*3.14*m_b [m_labels_regPriorTerm[imp]]);
                    }
                }
                
                // m_trans True
                if (m_trans != "false")
                {                
                    double b_current_imp;
                    Eigen::VectorXd weights = Eigen::VectorXd::Zero (m_b.size());
                   
                    for (int imp = 0; imp < m_current_number_imp; imp ++)
                    {                   
                        diff                = m_log_q[imp] - m_log_qb;
                        b_current_imp       = 0;
                        weights             = Eigen::VectorXd::Zero (m_b.size());

                        for (int impulsion_horaire = m_release_grid_borders[imp]; impulsion_horaire < m_release_grid_borders[imp+1]; impulsion_horaire ++)
                        {
                            weights [m_labels_regPriorTerm[impulsion_horaire]] = weights [m_labels_regPriorTerm[impulsion_horaire]] + 1;
                        }                    
                        for (int index_regprior = 0; index_regprior < m_b.size(); index_regprior ++)
                        {
                            weights [index_regprior]    = weights [index_regprior] / (m_release_grid_borders[imp+1]-m_release_grid_borders[imp]);
                            b_current_imp               = b_current_imp + weights [index_regprior] * m_b [index_regprior]; 
                        }

                        J_prior_on_log_q    = J_prior_on_log_q + std::pow (diff, 2) / (2*b_current_imp) + 0.5 * std::log (2*3.14*b_current_imp);
                    }                
                }
            }
           
            //........................................................           
            // Folded gaussian prior on the logarithms of the release
            // m_b scale parameter, m_log_qb location parameter
            if (index_prior == 4)
            {
                double diff, b_current_imp;
                Eigen::VectorXd weights = Eigen::VectorXd::Zero (m_b.size());

                for (int imp = 0; imp < m_current_number_imp; imp ++)
                {                   
                    diff                = m_log_q[imp] - m_log_qb;                    
                    b_current_imp       = 0;
                    weights             = Eigen::VectorXd::Zero (m_b.size());

                    for (int impulsion_horaire = m_release_grid_borders[imp]; impulsion_horaire < m_release_grid_borders[imp+1]; impulsion_horaire ++)
                    {
                        weights [m_labels_regPriorTerm[impulsion_horaire]] = weights [m_labels_regPriorTerm[impulsion_horaire]] + 1;
                    }                    
                    for (int index_regprior = 0; index_regprior < m_b.size(); index_regprior ++)
                    {
                        weights [index_regprior]    = weights [index_regprior] / (m_release_grid_borders[imp+1]-m_release_grid_borders[imp]);
                        b_current_imp               = b_current_imp + weights [index_regprior] * m_b [index_regprior]; 
                    }                
                    
                    J_prior_on_log_q    = J_prior_on_log_q + std::pow (diff, 2) / (2*b_current_imp) + 0.5 * std::log (3.14*b_current_imp/2);
                }                
            }
 
            //........................................................           
            // Exponential prior on the logarithms of the release
            // m_b scale parameter, m_log_qb location parameter
            if (index_prior == 13)
            {
                for (int imp = 0; imp < m_current_number_imp; imp ++)
                {                   
                    J_prior_on_log_q = J_prior_on_log_q + m_log_q [imp];
                }                
            }
            return J_prior_on_log_q;
        }
    
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    // compute_prior_on_R
    double compute_prior_on_R
        (const int & index_prior, const Eigen::VectorXd & m_r, const int & m_number_obs)
        {
            double J_prior_on_R = 0;

            // prior jeffrey
            if (index_prior == 6)
            { 
                J_prior_on_R = J_prior_on_R + 0.5 * m_number_obs * std::log (m_r[0]);
            }
            return J_prior_on_R;
        }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    // compute_prior_on_B
    double compute_prior_on_B
        (const int & index_prior, const Eigen::VectorXd & m_b)
        {
            double J_prior_on_B = 0;
            // prior jeffrey
            if (index_prior == 6)
            {
                for (int i = 0; i < m_b.size(); i++)
                {
                    J_prior_on_B = J_prior_on_B + 0.5 * std::log (m_b[i]);
                }
            }
            return  J_prior_on_B;
        }

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    // compute_prior_on_grid_borders
    double compute_prior_on_grid_borders
        (const int & index_prior, const int & number_borders, const std::string & trans, const double & m_scale_prior_on_borders)
        {            
            double J_prior_on_grid_borders  = 0;
            int Nmax = 312; // ATTENTION A CHANGER

            if (trans == "full")
            {           
                // Uniform prior on the grid of borders
                if (index_prior == 0)
                {                    
                    J_prior_on_grid_borders = J_prior_on_grid_borders;
                }

                // Uniform prior on the number of borders (uniform on size of the partitions) + uniform prior on the grid of borders at each size
                if (index_prior == 14)
                {                    
                    J_prior_on_grid_borders = J_prior_on_grid_borders - std::log (ibisa::fact(number_borders)) - std::log (ibisa::fact(Nmax-number_borders));
                }

                // Folded gaussian prior on the number of the borders
                if (index_prior == 4)
                {                    
                    J_prior_on_grid_borders = std::pow (number_borders, 2) / m_scale_prior_on_borders ;
                        // * 0.5 + 0.5 * std::log (m_scale_prior_on_borders);
                }
              
                // Exponential prior on the number of borders
                if (index_prior == 13)
                {                    
                    J_prior_on_grid_borders         = J_prior_on_grid_borders + m_scale_prior_on_borders * number_borders ;
                        //- std::log (m_scale_prior_on_borders);
                }
            }            
            return  J_prior_on_grid_borders;
        }

    // ---------------------------------------------

} // namespace ibisa











































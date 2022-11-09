/*--------------------------------------------------
 * /IBISA/include/
 * compute_likelihood.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class compute_likelihood
 * TODO: 
 *
 */


#include <include/compute_likelihood.h>

namespace ibisa
{

    double compute_likelihood 
        (int & index_distribution, double & m_yt, Eigen::MatrixXd & m_y, Eigen::VectorXd & m_yS, 
            Eigen::VectorXd & m_r, Eigen::VectorXi & m_index_r, Eigen::VectorXi & m_size_indexing_observationerror)
        {

            double J_likelihood = 0;

            // Gaussian Likelihood
            //  -------------------------------
            if (index_distribution == 1) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    J_likelihood =  J_likelihood + std::pow (m_yS[i] - m_y(i,3), 2) / m_r [m_index_r[i]] ;
                }

                J_likelihood = 0.5 * J_likelihood;
                for (int centroid_r = 0; centroid_r < m_r.size() - 1; centroid_r ++)
                {
                    J_likelihood = J_likelihood + 0.5 * m_size_indexing_observationerror[centroid_r] * std::log (m_r[centroid_r]);
                } 
            }
           
            //--------------------------------
            // Log-normal likelihood
            else if (index_distribution == 2) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    if (m_index_r[i] != m_r.size() - 1 or m_r.size() == 1)
                    {
                        J_likelihood =  J_likelihood + std::pow (std::log (m_yS[i] + m_yt) - std::log (m_y(i,3) + m_yt), 2) / m_r [m_index_r[i]];
                    }
                }
                
                for (int centroid_r = 0; centroid_r < m_r.size() - 1; centroid_r ++)
                {
                    J_likelihood = J_likelihood + m_size_indexing_observationerror[centroid_r] * std::log (m_r[centroid_r]);
                }
                
                if (m_r.size() == 1)
                {
                    J_likelihood = J_likelihood + m_size_indexing_observationerror[0] * log(m_r[0]);
                }
                J_likelihood = 0.5 * J_likelihood;
            }
            
            //--------------------------------
            // Log-Laplace likelihood
            else if (index_distribution == 10) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    if (m_index_r[i] != m_r.size() - 1 or m_r.size() == 1 )
                    {
                        J_likelihood =  J_likelihood + std::abs (std::log (m_y(i,3) + m_yt) - std::log (m_yS[i] + m_yt)) / m_r [m_index_r[i]] ;
                    }
                }

                for (int centroid_r = 0; centroid_r < m_r.size() - 1; centroid_r ++)
                {
                    J_likelihood = J_likelihood + m_size_indexing_observationerror[centroid_r] * std::log (m_r[centroid_r]);
                }

                if (m_r.size() == 1)
                {
                    J_likelihood = J_likelihood + m_size_indexing_observationerror[0] * std::log(m_r[0]);
                }
            }
            
            //--------------------------------
            // Log - Cauchy Likelihood
            // ici r_i est un écart type plutôt qu'une variance comme dans la loi log-normale
            else if (index_distribution == 11) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    if (m_index_r[i] != m_r.size() - 1 or m_r.size() == 1)
                    {
                        double a        = std::log (m_y(i,3) + m_yt) - std::log (m_yS[i] + m_yt);
                        J_likelihood    = J_likelihood + std::log (std::pow (m_r [m_index_r[i]], 2) + std::pow (a, 2) + m_yt/2 ) - std::log (m_r [m_index_r[i]]);
                    }
                }
            }

            //--------------------------------
            // Logistic Likelihood
            else if (index_distribution == 9) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    J_likelihood =  J_likelihood 
                                    + (m_y(i,3) - m_yS[i]) / m_r [m_index_r[i]]
                                    + 2 * std::log (1 + std::exp (- (m_y(i,3) - m_yS[i]) / m_r [m_index_r[i]] ) ) ;
                }

                for (int centroid_r = 0; centroid_r < m_r.size() - 1; centroid_r ++)
                {
                    J_likelihood = J_likelihood + m_size_indexing_observationerror [centroid_r] * std::log (m_r[centroid_r]);
                }                
            }
 
            //--------------------------------    
            // Folded Gaussian Likelihood
            else if (index_distribution == 4) 
            {
                for (int i = 0; i < m_y.col(3).size(); i++)
                {
                    J_likelihood =  J_likelihood 
                                    + std::pow ( m_yS[i] - m_y(i,3), 2) / m_r [m_index_r[i]] 
                                    + std::pow (-m_yS[i] - m_y(i,3), 2) / m_r [m_index_r[i]];
                }
                J_likelihood = 0.5 * J_likelihood + 0.5 * m_size_indexing_observationerror[0] * std::log (m_r[0]);
            }
     
            else 
            {
                cout << "Warning: no likelihood selected!" << endl;
                throw std::exception();
            }

            return  J_likelihood ;
        }


} // namespace ibisa











































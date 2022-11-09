/*--------------------------------------------------
 * /IBISA/include/
 * compute_cost_couples.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class compute_cost_couples
 * TODO: 
 *
 */


#include <include/compute_cost_couples.h>
#include <Eigen/Dense>
#include <string>

using namespace std;

namespace ibisa
{

    const Eigen::VectorXd compute_cost_couples 
        (const int & index_distribution , const double & m_yt , const Eigen::MatrixXd & m_y , const Eigen::VectorXd & m_yS , const Eigen::VectorXd & m_r , 
         const Eigen::VectorXi & m_index_observationerror, const Eigen::VectorXi & m_size_indexing_observationerror)
        {

            Eigen::VectorXd J_likelihood (m_y.col(3).size());

            // Gaussian Likelihood
            //  -------------------------------
            if ( index_distribution == 1 ) 
            {                
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  (  m_yS[i] - m_y(i,3) ) * ( m_yS[i] - m_y(i,3) ) / m_r [m_index_observationerror[i]] ;
                }
        
            }

            // Folded Gaussian Likelihood
            //  -------------------------------
            else if ( index_distribution == 4 ) 
            {
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  (  m_yS[i] - m_y(i,3) ) * ( m_yS[i] - m_y(i,3) ) / m_r [m_index_observationerror[i]] + ( - m_yS[i] - m_y(i,3) ) * ( - m_yS[i] - m_y(i,3) ) / m_r [m_index_observationerror[i]];
                }

            }
            
            // Log - normal Likelihood
            //  -------------------------------
            else if ( index_distribution == 2 ) 
            {
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  ( log( m_yS[i] + m_yt ) - log( m_y(i,3) + m_yt ) ) * ( log( m_yS[i] + m_yt ) - log( m_y(i,3) + m_yt ) ) / m_r [m_index_observationerror[i]];
                }
            }



            // Logistic Likelihood
            //  -------------------------------
            else if ( index_distribution == 9 ) 
            {
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  ( m_y(i,3) - m_yS[i] ) / m_r [m_index_observationerror[i]] + 2 * log ( 1 + exp ( - ( m_y(i,3) - m_yS[i] ) / m_r [m_index_observationerror[i]] ) ) ;
                }


            }
            
            // Log - Laplace Likelihood
            //  -------------------------------
            else if ( index_distribution == 10 ) 
            {
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  abs ( log ( m_y(i,3) + m_yt ) - log ( m_yS[i] + m_yt ) ) / m_r [m_index_observationerror[i]] ;
                }


            }

            // Log - Cauchy Likelihood
            //  -------------------------------
            else if ( index_distribution == 11 ) 
            {
                for ( int i = 0 ; i < m_y.col(3).size() ; i++ )
                {
                    J_likelihood[i] =  log ( 1 + pow ( ( log ( m_y(i,3) + m_yt ) - log ( m_yS[i] + m_yt ) ) / m_r [m_index_observationerror[i]] , 2 )  );
                }

            }
                 
            else 
            {
                cout << "Warning: no likelihood selected!" << endl;
                throw std::exception();
            }

            return  J_likelihood ;
        }


} // namespace ibisa











































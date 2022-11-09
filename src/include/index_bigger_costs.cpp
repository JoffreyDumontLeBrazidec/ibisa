/*--------------------------------------------------
 * /IBISA/include/
 * index_bigger_costs.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class index_bigger_costs
 * TODO: 
 *
 */


#include <include/index_bigger_costs.h>
#include <Eigen/Dense>
#include <string>

using namespace std;

namespace ibisa
{

    const Eigen::VectorXd find_max_values 
        (const Eigen::VectorXd & J_likelihood , const int & number_higherrorobs)
        {

            Eigen::VectorXd the_index ( J_likelihood.size() );
            int current_index ;
            Eigen::VectorXd J_likelihood_copy = J_likelihood ;

            for ( int i = 0 ; i < J_likelihood.size() ; i ++ )
            {
                the_index[i] = 0;
            }

            for ( int i = 0 ; i < number_higherrorobs ; i ++ )
            {
                for ( int j = 0 ; j < J_likelihood_copy.size() ; j ++)
                {
                    if (  J_likelihood_copy[j] == J_likelihood_copy.maxCoeff() )
                    {
                        current_index = j ;
                    }
                }

                J_likelihood_copy [current_index] = -10000;

                the_index[current_index] = 1; 
            }
        
            return the_index;
        }


} // namespace ibisa











































/*--------------------------------------------------
 * /IBISA/
 * indexation.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class INDEXATION
 * TODO: 
 *
 */


#include <include/indexation.h>
#include <Eigen/Dense>
#include <tuple>

using namespace std;

namespace ibisa
{
    
    // number_variables = Longitude + Latitude + Release rates + uncertainties + meteo

    //----------------------------------------
    // function to index correctly the variables

    std::tuple <Eigen::VectorXi, Eigen::VectorXi> indexation 
        (const Configuration & config)
        {

            int number_variables = config.get_integer ("variables.number.max"); 
            Eigen::VectorXi Index (number_variables);
            Eigen::VectorXi Length (number_variables);
            
            for (int k = 0 ; k < number_variables ; k++)
            {
                Index[k]    = -1;
                Length[k]   = 0;
            }

            int i = 0;

            if (config.get_bool ("chain.search.location") == true)
            {
                Index  [0]  = i; 
                Length [0]  = 1;
                Index  [1]  = i + 1; 
                Length [1]  = 1;
                i           = i+2;
            }

            if (config.get_bool ("chain.search.release") == true)
            {
                Index[2]    = i; 
                Length[2]   = std::max (config.get_integer ("search.impulsions.end") - config.get_integer ("search.impulsions.beg"), config.get_integer ("search.impulsions.number"));
                i           = i + Length[2];
            }


            if (config.get_bool ("chain.search.observationerror") == true)
            {
                Index[3]    = i; 
                Length[3]   = config.get_integer ("search.observationerror.number");
                i           = i + Length[3] ; 
            }
     
            if (config.get_bool ("chain.search.met") == true)
            {
                Index[4]    = i; 
                Length[4]   = config.get_integer ("init.number.meteo");
                i           = i + Length[4];
            }

            if (config.get_bool ("chain.search.regprior") == true)
            {
                Index[5]    = i; 
                Length[5]   = config.get_integer ("search.regprior.number");
                i           = i + Length[5];
            }
           
            return std::make_tuple (Index, Length);
        }


} // namespace ibisa











































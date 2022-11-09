/*--------------------------------------------------
 * /IBISA/include/
 * walk_conditions.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class WALK_CONDITIONS
 * TODO: 
 *
 */


#include <include/walk_conditions.h>

namespace ibisa
{
    
    // Nvar = Longitude + Latitude + Release rates + uncertainties + meteo

    //----------------------------------------
    // function to index correctly the variables

    
    Eigen::MatrixXd walk_conditions
        (Configuration & config)
        {

            int maxNumber_variables = config.get_integer ("variables.number.max");
            Eigen::MatrixXd Markov_Transitions (maxNumber_variables, 4) ;

            double min_positif = 0.0000000001 ;

            cout << "include::walk_conditions" << endl;
            // Location
           
            // Longitude
            Markov_Transitions(0,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.coord.name"));
            Markov_Transitions(0,1) = config.get_real                   ("distribution.transition.coord.var");
            Markov_Transitions(0,2) = config.get_real                   ("search.location.spatial.west") ;
            Markov_Transitions(0,3) = config.get_real                   ("search.location.spatial.east") ;

            // Latitude
            Markov_Transitions(1,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.coord.name"));
            Markov_Transitions(1,1) = config.get_real                   ("distribution.transition.coord.var");
            Markov_Transitions(1,2) = config.get_real                   ("search.location.spatial.south") ;
            Markov_Transitions(1,3) = config.get_real                   ("search.location.spatial.north") ;

            // Log. release rates
            Markov_Transitions(2,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.release.name"));
            Markov_Transitions(2,1) = config.get_real                   ("distribution.transition.release.var");
            Markov_Transitions(2,2) = config.get_real                   ("search.log_release.min");
            Markov_Transitions(2,3) = config.get_real                   ("search.log_release.max");

            // Hyperparameter r
            Markov_Transitions(3,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.observationerror.name"));
            Markov_Transitions(3,1) = config.get_real                   ("distribution.transition.observationerror.var");
            Markov_Transitions(3,2) = min_positif ;
            Markov_Transitions(3,3) = 20;

            // Meteo
            Markov_Transitions(4,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.meteo.name"));
            Markov_Transitions(4,1) = config.get_real                   ("distribution.transition.meteo.var");
            Markov_Transitions(4,2) = 0.001 ;
            Markov_Transitions(4,3) = 1;
 
            // Regularisation prior term (b)
            Markov_Transitions(5,0) = ibisa::distribution_indexation    (config.get_string("distribution.transition.regprior.name"));
            Markov_Transitions(5,1) = config.get_real                   ("distribution.transition.regprior.var");
            Markov_Transitions(5,2) = min_positif ;
            Markov_Transitions(5,3) = 10000;

            
            if (config.get_bool ("init.meteo.interpolation") == false)
            {
                Markov_Transitions(4,0) = 7;
            }

            return (Markov_Transitions);
        }


} // namespace ibisa











































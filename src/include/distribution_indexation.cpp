/*--------------------------------------------------
 * /IBISA/include/
 * distribution_indexation.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class DISTRIBUTION_INDEXATION
 * TODO: 
 *
 */


#include <include/distribution_indexation.h>

namespace ibisa
{
    
    // Nvar = Longitude + Latitude + Release rates + uncertainties + meteo

    //----------------------------------------
    // distribution indexation
    int distribution_indexation
        (std::string name_distribution)
        {
            int index_distribution;
        
            // Uniform
            if (name_distribution == "uniform" or name_distribution == "Uniform" )
                index_distribution = 0;

            // Gaussian
            if (name_distribution == "Gaussian" or name_distribution == "gaussian" or name_distribution == "Normal" or name_distribution == "normal" )
                index_distribution = 1;
     
            // log-noral
            if (name_distribution == "log_normal" or name_distribution == "lognormal" or name_distribution == "Lognormal" or name_distribution == "Log_normal")
                index_distribution = 2;

            // log-gamma
            if (name_distribution == "log_gamma" or name_distribution == "loggamma" or name_distribution == "Loggamma" or name_distribution == "Log_gamma" 
                    or name_distribution == "Log_Gamma" or name_distribution == "log_Gamma")
                index_distribution = 3;
            
            // Folded Gaussian mn
            if (name_distribution == "Folded_Gaussian_min" or name_distribution == "Folded_gaussian_min" or name_distribution == "Folded_Normal_min" or name_distribution == "Folded_normal_min")
                index_distribution = 4;
            
            // Folded Gaussian min max
            if (name_distribution == "Folded_Gaussian_min_max" or name_distribution == "Folded_gaussian_min_max" or name_distribution == "Folded_Normal_min_max" or name_distribution == "Folded_normal_min_max")
                index_distribution = 5;
     
            // Jeffrey
            if (name_distribution == "jeffrey" or name_distribution == "Jeffrey" )
                index_distribution = 6;
            
            // discrete uniform
            if (name_distribution == "discrete_uniform" )
                index_distribution = 7;
     
            // log-logistic
            if (name_distribution == "log_logistic" or name_distribution == "log-logistic" or name_distribution == "Log-logistic" or name_distribution == "Log_logistic" 
                    or name_distribution == "Log_Logistic" or name_distribution == "log_Logistic")
                index_distribution = 8;

            // Logistic
            if (name_distribution == "logistic" or name_distribution == "Logistic" ) 
                index_distribution = 9;

            // log-Laplace
            if (name_distribution == "log_laplace" or name_distribution == "loglaplace" or name_distribution == "Loglaplace" or name_distribution == "Log_laplace" 
                    or name_distribution == "Log_Laplace" or name_distribution == "log_Laplace")
                index_distribution = 10;
     
            // log-Cauchy
            if (name_distribution == "log_cauchy" or name_distribution == "logcauchy" or name_distribution == "Logcauchy" or name_distribution == "Log_cauchy" 
                    or name_distribution == "Log_Cauchy" or name_distribution == "log_Cauchy")
                index_distribution = 11;
     
            // Lévy
            if (name_distribution == "levy" or name_distribution == "Lévy")
                index_distribution = 12;

            // Exponential
            if (name_distribution == "exp" or name_distribution == "exponential" or name_distribution == "Exp" or name_distribution == "Exponential")
                index_distribution = 13;
 
            // Uniform_size_partition
            if (name_distribution == "uniform_size_partition" or name_distribution == "Uniform_size_partition")
                index_distribution = 14;           
            
            // Folded_Gaussian_min_impMin
            if (name_distribution == "Folded_Gaussian_min_impMin" or name_distribution == "Folded_gaussian_min_impMin")
                index_distribution = 15;
    
            return index_distribution;
        }

} // namespace ibisa











































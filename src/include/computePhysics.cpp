/*--------------------------------------------------
 * /IBISA/include/
 * computePhysics.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class computePhysics
 * TODO: 
 *
 */


#include <include/computePhysics.h>

namespace ibisa
{
    
    // ----------------------------------
    // computeTRRA - ReleaseRate=Impulsion
    double computeTRRA
        (const Eigen::VectorXd & logReleaseRates, const Eigen::VectorXi & durationRates, const int & FullOperatorPeriod, const std::string & trans)
        {   
            double trra                         = 0;            
            double convert_HourlyReleaseRate    = 3600;

            for (int imp = 0 ; imp < logReleaseRates.size(); imp ++)
            {
                if (trans == "false")
                {
                    trra = trra + std::exp (logReleaseRates[imp]);
                }
                if (trans != "false")
                {
                    trra = trra + std::exp (logReleaseRates[imp]) * (durationRates[imp+1] - durationRates[imp]);
                }                
            }
            trra = trra * FullOperatorPeriod * convert_HourlyReleaseRate;

            return trra;
        }

    // ----------------------------------
    // computeTRRAContraint - ReleaseRate=Impulsion - ATTENTION ! A ETE CODE POUR LE CAS FUKUSHIMA avec REVERSIBLE JUMP EN MODE FULL
    double computeTRRANContraint
        (const Eigen::VectorXd & logReleaseRates, const Eigen::VectorXi & durationRates, const int & FullOperatorPeriod, const std::string & trans)
        {   
            double trra_nc                       = 0;            
            double convert_HourlyReleaseRate    = 3600;
            double pinc                         = 0;

            for (int imp = 0 ; imp < logReleaseRates.size() ; imp ++)
            {
                if (trans != "false")
                {
                    pinc    = std::min (16,  durationRates [imp+1]) - std::min (16,  durationRates [imp])     
                            + std::max (116, durationRates [imp+1]) - std::max (116, durationRates [imp])   
                            + std::min (166, durationRates [imp+1]) - std::min (166, durationRates [imp])   
                            + std::max (294, durationRates [imp+1]) - std::max (294, durationRates [imp])   
                            - (durationRates [imp+1] - durationRates [imp]);
                                                                                                                                                                                                          
                    
                    trra_nc = trra_nc + std::exp (logReleaseRates[imp]) * pinc;
                }                
            }
            trra_nc = trra_nc * FullOperatorPeriod * convert_HourlyReleaseRate;

            return trra_nc;
        }

    // ----------------------------------

} // namespace ibisa











































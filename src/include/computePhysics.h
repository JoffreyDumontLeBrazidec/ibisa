/*--------------------------------------------------
 * /IBISA/include/
 * computePhysics.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class computePhysics
 * TODO: 
 *
 */

#ifndef DEF_computePhysics
#define DEF_computePhysics

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // list of computePhysics functions
    double computeTRRA      
        (const Eigen::VectorXd & logReleaseRates, const Eigen::VectorXi & durationRates, const int & FullOperatorPeriod, const std::string & trans);
    
    double computeTRRANContraint
                (const Eigen::VectorXd & logReleaseRates, const Eigen::VectorXi & durationRates, const int & FullOperatorPeriod, const std::string & trans);

} // namespace ibisa

#endif















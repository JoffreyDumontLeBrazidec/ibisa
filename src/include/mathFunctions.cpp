/*--------------------------------------------------
 * /IBISA/include/
 * mathFunctions.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class mathFunctions
 * TODO: 
 *
 */


#include <include/mathFunctions.h>

namespace ibisa
{
   
    // ----------------------------------
    // fact
    double fact
        (const int & n)
        {
            int int_factoriel_n = 1; 
            for (int i = 2; i <= n; i++) 
                int_factoriel_n *= i; 

            double double_factoriel_n = (double) int_factoriel_n;
            return double_factoriel_n; 
        }

    // ----------------------------------

} // namespace ibisa











































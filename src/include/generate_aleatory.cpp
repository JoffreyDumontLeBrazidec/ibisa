/*--------------------------------------------------
 * /IBISA/include/
 * generate_aleatory.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2020/06
 *--------------------------------------------------
 *
 * Implementation of function GENERATE_ALEATORY
 * TODO: 
 *
 */


#include <include/generate_aleatory.h>

namespace ibisa
{
    // ----------------------------------------
    // generate_random_double
    double generate_random_double
        ()
        {
            std::mt19937                                mersenne (static_cast<std::mt19937::result_type> (std::time(nullptr)));
            std::uniform_real_distribution<double>      dice    (0,1);
            double u                                    = dice (mersenne) ;
            return u;
        }

    // ----------------------------------------
    // generate_seed 
    Eigen::Vector2i generate_seed 
        ()
        {
            std::mt19937                        mersenne (static_cast<std::mt19937::result_type> (std::time(nullptr)));
            std::uniform_int_distribution<>     dice (0, int(1e10));
            int u1                              = dice (mersenne) ;
            int u2                              = dice (mersenne) ;
            Eigen::Vector2i                     seed;
            seed                                << u1, u2 ;
            return seed;
        } 
    
    // ----------------------------------------

} // namespace ibisa











































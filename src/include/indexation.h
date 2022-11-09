/*--------------------------------------------------
 * /IBISA/include/tools/
 * indexation.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class INDEXATION
 * TODO: 
 *
 */

#ifndef DEF_INDEXATION
#define DEF_INDEXATION

#include <iostream>
#include <string>
#include <tuple>
#include <Eigen/Dense>
// library for random numbers
#define TPPDA_USE_TPPRANDOM

// library for linear algebra
#define TPPDA_USE_TPPLINALG
#define TPPLINALG_USE_GENERIC_BLAS

#include <tppda/tppda.cpp>

using Configuration             = tppda::TPPDAConfiguration<double, int, int> ;

namespace ibisa
{
    //----------------------------------------
    // function to index correctly the variables
    // Longitude x1 : 0
    // Latitude  x2 : 1
    // Release   q  : 2
    // Diagonal term R : 3
    // Meteo choice m : 4 
    // ?

    std::tuple <Eigen::VectorXi, Eigen::VectorXi> indexation (const Configuration & config);




} // namespace ibisa

#endif















/*--------------------------------------------------
 * /IBISA/main
 * basic_modules.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of BASIC_MODULES
 * TODO: 
 *
 */



#include <iostream>
#include <fstream>

#include <string>
#include <tuple>
#include <Eigen/Dense>
#include <random>
#include "math.h"
#include <memory>
#include <ctime>
#include <chrono>
using namespace std::chrono; 
#include <string>
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>

#define TPPDA_USE_TPPRANDOM
#define TPPDA_USE_TPPLINALG
#define TPPLINALG_USE_GENERIC_BLAS
#include <tppda/tppda.cpp>
#include <tpprandom/urng.h>

#include <include/mathFunctions.h>

using namespace std;
using Configuration             = tppda::TPPDAConfiguration<double, int, int> ;
















/*--------------------------------------------------
 * /IBISA/controleur/
 * controleur.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Controleur
 * TODO: 
 *
 */

#ifndef DEF_Controleur
#define DEF_Controleur

#include <main/basic_modules.h>
#include "include/indexation.h"
#include "include/tools.h"
#include "include/distribution_indexation.h"
#include "include/compute_likelihood.h"
#include "include/compute_cost_couples.h"
#include "include/index_bigger_costs.h"
#include "include/read.h"

namespace ibisa {

    class Controleur

    {

        public:
            // ---------------------------------------
            // Constructeur-Destructeur
            Controleur (Configuration & config);

            ~Controleur ();
  
        private:
            // -------------------------------------

    };
} // namespace ibisa
#endif

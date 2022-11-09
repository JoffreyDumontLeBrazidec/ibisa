/*--------------------------------------------------
 * /IBISA/
 * paralleltempering.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class PTalgo
 * TODO: 
 *
 */

#ifndef DEF_PTALGO
#define DEF_PTALGO

#include <main/basic_modules.h>
#include "state/StandardState.h"
#include "include/indexation.h"
#include "state/MarkovState.h"
#include "cost/cost.h"
#include "observationOperator/observationOperator.h"
#include "storage/storageoperator.h"
#include "controleur/controleur.h"
#include "redoperator/redoperator.h"
#include <managers/cycle_manager.h>
#include <include/generate_aleatory.h>
#include <managers/alpha_manager.h>
#include <managers/store_manager.h>

namespace ibisa {

    class PTalgo

    {

        public:
            // -------------------------    
            // Constructeur-Destructeur
            PTalgo (std::string config_file);  

            ~PTalgo();
  
            // Méthodes
            void run();

        private:
            // ------------------------
            // Attributs
            std::string m_config_file; 

};
} //namespace ibisa
#endif





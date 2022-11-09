/*--------------------------------------------------
 * /IBISA/
 * PTalgo.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class PTalgo
 * TODO: 
 *
 */

#include "main/basic_modules.h"
#include "paralleltempering.h"
#include <main/modules.h>

namespace ibisa
{
    // Constructeur-Destructeur
    PTalgo::PTalgo
        (std::string config_file)
        {
            m_config_file = config_file;
        }
  
    PTalgo::~PTalgo
        ()
        {}
  
    // Méthodes
    void PTalgo::run
        ()
        {
            // --------------------------
            // Initialisation 
            cout << "PT::Initialisation Parallel Tempering algorithm with config_file: " << m_config_file << endl;
            Configuration config                (m_config_file);
            int m_number_cycles                 = config.get_integer ("control.sizeLoop"); 
            
            // rng
            Eigen::Vector2i seed                    = ibisa::generate_seed();
            tppda::URNG<double,int>                 m_ung (seed[1]);

            // controls
            ibisa::Controleur control (config);

            // Big objects
            // *
            // observationOperator
            cout << "PT::observationOperator ..." << endl;
            ibisa::observationOperator  U (config, seed[0]);
            
            // MarkovState
            cout << "PT::Markov State ..." << endl ;
            ibisa::MarkovState      X   (config, seed) ;
            ibisa::StandardState    sX  (config, X.get_state(), X.get_index_observationerror(), X.get_size_indexing_observationerror());
            
            // Temperatures
            Eigen::VectorXd temperatures            = X.get_temperatures () ;
            
            // Within acceptance ratio gestion
            int number_type_walks_max                       = config.get_integer ("number.type.walks.max"); ;
            Eigen::MatrixXd within_acceptance_ratio         (temperatures.size(), number_type_walks_max); // 0 for normal move, 1 for trans move, 2 for birth, 3 for death
            Eigen::VectorXd swap_acceptance_ratio           (temperatures.size() - 1);
            double          mean_within_acceptance_ratio;
            int index_walk_choice;

            // Storage
            cout << "PT::Storage Operator ... " << endl; 
            ibisa::Storage_Operator S (config, m_config_file);

            // Cost
            cout << "PT::Compute Cost..." << endl;         
            X.compute_cost (U);

            // Managers
            cout << "PT::Init Managers ..." << endl;
            ibisa::Cycle_Manager    Admin_C (config, seed, temperatures);
            ibisa::Alpha_Manager    Admin_A (config, temperatures);
            ibisa::Store_Manager    Admin_S (config,temperatures); 

            // -------------------------
            // cycling begin
            // --------------------------
            cout << "PT::Beginning Parallel tempering algorithm ... " << endl;
     
            for (int cycle_num(0) ; cycle_num < m_number_cycles ; cycle_num++)
            {
                Admin_C.update_MS_cycle_number                      (X, cycle_num);
                
                Admin_C.modular_print                               (cycle_num, X, U, Admin_A.get_last_within_acceptance_ratio(), Admin_A.get_last_swap_acceptance_ratio());
       
                Admin_C.reduce_or_enhance_set_obs                   (cycle_num, X, sX, U);

                // metropolis-hastings step
                index_walk_choice 
                    = Admin_C.walk                                  (cycle_num, X, U);
                                
                within_acceptance_ratio.col (index_walk_choice)
                    = Admin_C.metroplis_hastings_step               (X, sX, U);
               

                // parallel tempering step
                swap_acceptance_ratio                               
                    = Admin_C.swap_temperatures_step                (X, swap_acceptance_ratio, U); 
                
                // treatment step
                Admin_C.uniformise (X, sX, cycle_num, U);
               
                mean_within_acceptance_ratio        
                    = Admin_A.update_acceptance_ratios              (within_acceptance_ratio, swap_acceptance_ratio, index_walk_choice);
                
                Admin_C.update_variances                            (cycle_num, mean_within_acceptance_ratio, X); 

                Admin_S.store_at_each_iteration                     (S, X, U, within_acceptance_ratio.col(0));
                Admin_S.store_at_last_iterations                    (S, cycle_num, X);
                Admin_S.store_for_each_temperature                  (S, X, within_acceptance_ratio.col(0), swap_acceptance_ratio);
           }

            Admin_S.store_at_lastest_iteration (S, X);
     
            cout << "End::Parallel tempering algorithm. Beginning writing results to file ..." << endl;

            // Write to file
            S.write_to_file ();         
            cout << "Bravo ! Storage done." << endl;
                    
            // ------------------------
        }

} // namespace ibisa
























/*--------------------------------------------------
 * /IBISA/managers/
 * Store_Manager.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Store_Manager
 * TODO: 
 *
 */

#include <managers/store_manager.h>

namespace ibisa
{
        
        // ------------------------
        // Constructeur-Destructeur
        Store_Manager::Store_Manager
            (Configuration & config, Eigen::VectorXd & temperatures)
            {
                cout << "Store_Manager::Configuration of the Manager of the storage ..." << endl;

                m_storing_size_predictions              = config.get_integer ("control.store.predictions");
                m_number_cycles                         = config.get_integer ("control.sizeLoop");
                m_decision_save_all_temperatures        = config.get_bool ("control.savealltemp");
                m_reduction_number_obs                  = config.get_bool ("data.reduce.obs");
                m_trans                                 = config.get_string ("release.transdimensionality");
                m_temperatures                          = temperatures;
            }

        Store_Manager::~Store_Manager
            ()
            {}

        // methodes
        // --------
        // store at each iteration
        void Store_Manager::store_at_each_iteration 
            (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp, ibisa::observationOperator & U, const Eigen::VectorXd & within_acceptance_ratio)
            {                
                if (m_reduction_number_obs == true)
                {
                    S.store_real (Xp.get_double_size_indexing_observationerror(), 2);
                }

                if (m_trans != "false")
                {
                    int index_temperature = 0;
                    S.store_vec (U.release_grid_borders (index_temperature).cast <double>(), 1);
                }
                S.store_real    (within_acceptance_ratio[0], 1);
                S.store_real    (Xp.get_cost_t(0), 0);
                S.store_state   (Xp.get_state());
            }
    
        // ----------------------------
        // store at last iterations
        void Store_Manager::store_at_last_iterations
            (ibisa::Storage_Operator & S, const int & cycle_num, ibisa::MarkovState & Xp)
            {                
                if (cycle_num >= m_number_cycles - m_storing_size_predictions)
                {                                        
                    S.store_vec (Xp.get_predictions(), 0);
                }
            }

        // ----------------------------
        // store for each temperature
        void Store_Manager::store_for_each_temperature
            (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp, const Eigen::VectorXd & within_acceptance_ratio, Eigen::VectorXd & swap_acceptance_ratio)
            {                                
                if (m_decision_save_all_temperatures == true and m_temperatures.size() > 1)
                {
                    Eigen::VectorXd cost_temperatures       (m_temperatures.size()-1);
                    for (int index_temperature = 1 ; index_temperature < m_temperatures.size() ; index_temperature ++)
                    {
                        cost_temperatures [index_temperature] = Xp.get_cost_t (index_temperature);
                    }
                    S.store_vec (cost_temperatures, 4);
                    S.store_vec (within_acceptance_ratio.tail (m_temperatures.size()-1), 2);
                    S.store_vec (swap_acceptance_ratio.tail (m_temperatures.size()-1), 3);
                }
            }

        // ----------------------------
        // store at lastest iteration
        void Store_Manager::store_at_lastest_iteration
            (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp)
            {                
                if (m_reduction_number_obs == true)
                {
                    S.store_vec (Xp.get_index_observationerror().cast <double> (), 5);
                }
            }

        // ----------------------------


} // namespace ibisa











































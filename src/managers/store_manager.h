/*--------------------------------------------------
 * /IBISA/managers/
 * Store_Manager.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class Store_Manager
 * TODO: 
 *
 */

#ifndef DEF_Store_Manager
#define DEF_Store_Manager

#include <main/basic_modules.h>
#include <main/modules.h>

namespace ibisa
{

    class Store_Manager

    {
        public:
            // ---------------------------------------
            // Constructeur-Destructeur
            Store_Manager (Configuration & config, Eigen::VectorXd & temperatures);

            ~Store_Manager ();
            
            // methodes
            void store_at_each_iteration        (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp, ibisa::observationOperator & U, const Eigen::VectorXd & within_acceptance_ratio);
            void store_at_last_iterations       (ibisa::Storage_Operator & S, const int & cycle_num, ibisa::MarkovState & Xp);
            void store_for_each_temperature     (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp, const Eigen::VectorXd & within_acceptance_ratio, Eigen::VectorXd & swap_acceptance_ratio);
            void store_at_lastest_iteration     (ibisa::Storage_Operator & S, ibisa::MarkovState & Xp);

        private:
            // -------------------------------------
            
            int                         m_number_cycles;
            int                         m_storing_size_predictions;
            Eigen::VectorXd             m_temperatures;
            std::string                 m_trans;
            bool                        m_decision_save_all_temperatures;
            bool                        m_reduction_number_obs;

    };




} // namespace ibisa

#endif















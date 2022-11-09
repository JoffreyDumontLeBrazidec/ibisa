/*--------------------------------------------------
 * /IBISA/managers/
 * cycle_manager.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class cycle_manager
 * TODO: 
 *
 */

#ifndef DEF_cycle_manager
#define DEF_cycle_manager

#include <main/basic_modules.h>
#include <main/modules.h>

namespace ibisa
{

    class Cycle_Manager

    {
        public:
            // ---------------------------------------
            // Constructeur-Destructeur
            Cycle_Manager (Configuration & config, const Eigen::Vector2i & seed, Eigen::VectorXd & temperatures);

            ~Cycle_Manager ();
            
            // methodes

            void modular_print                              (const int & cycle_num, ibisa::MarkovState & X, ibisa::observationOperator & U, 
                                                                const Eigen::MatrixXd & last_within_acceptance_ratio, const Eigen::MatrixXd & last_swap_acceptance_ratio);            
            
            void reduce_or_enhance_set_obs                  (const int & cycle_num, ibisa::MarkovState & X, ibisa::StandardState & sX, ibisa::observationOperator & U); 
            void update_variances                           (const int & cycle_num, double & mean_within_acceptance_ratios, ibisa::MarkovState & Xp);
            
            int walk                                        (const int & cycle_num, ibisa::MarkovState & Xp, ibisa::observationOperator & U);            
            Eigen::VectorXd metroplis_hastings_step         (ibisa::MarkovState & X, ibisa::StandardState & sX, ibisa::observationOperator & U);
            Eigen::VectorXd swap_temperatures_step          (ibisa::MarkovState & X, Eigen::VectorXd & swap_acceptance_ratio, ibisa::observationOperator & U);
            void uniformise                                 (ibisa::MarkovState & X, ibisa::StandardState & sX, int & cycle_num, ibisa::observationOperator & U);
            void update_MS_cycle_number                     (ibisa::MarkovState & X, int & cycle_num);

        private:
            // -------------------------------------
            int m_order_print; 
            std::chrono::high_resolution_clock::time_point  m_step;
            std::chrono::high_resolution_clock::time_point  m_last_step;
            std::chrono::duration<double, std::milli>       m_fp_ms;

            bool                            m_reduction_number_obs;
            bool                            m_tri_obs;

            int                             m_burn_in;
            int                             m_modprint;
            int                             m_uniformisation;
            int                             m_mod_enhance_number_obs;            
            int                             m_number_temperatures;
            std::vector<std::string>        m_possible_walks;
            int                             m_update_acceptance_ratio;

            tppda::GRNG<double,int>         m_rng;
            tppda::URNG<double,int>         m_ung;
            
            std::string                     m_trans;
            int                             m_index_walk_choice;

            Eigen::VectorXd                 m_temperatures;
            Eigen::VectorXd                 m_inverse_temperatures;
                
};




} // namespace ibisa

#endif















/*--------------------------------------------------
 * /IBISA/managers/
 * Alpha_Manager.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Alpha_Manager
 * TODO: 
 *
 */

#include <managers/alpha_manager.h>

namespace ibisa
{
        
        // ------------------------
        // Constructeur-Destructeur
        Alpha_Manager::Alpha_Manager
            (Configuration & config, Eigen::VectorXd & temperatures)
            {
                cout << "Alpha_Manager::Configuration of the Manager of the acceptance ratios ..." << endl;

                m_temperatures                                  = temperatures;
                m_modprint                                      = config.get_integer ("control.modprint");
                
                m_size_mean_acceptance_ratio                    = std::max (m_modprint, 100);
                int number_type_walks_max                       = config.get_integer ("number.type.walks.max"); ;

                m_last_within_acceptance_ratio                  = Eigen::MatrixXd::Zero (m_size_mean_acceptance_ratio, number_type_walks_max); // walk, move, birth, death, hyper
                m_last_swap_acceptance_ratio                    = Eigen::MatrixXd::Zero (m_size_mean_acceptance_ratio, temperatures.size()-1);

                m_compteur_for_mean_acceptance                  = Eigen::VectorXi::Zero (number_type_walks_max) ;
                m_compteur_for_swap_mean_acceptance             = 0;
                
                m_mean_within_acceptance_ratios                 = Eigen::VectorXd::Zero (number_type_walks_max);
           }

        Alpha_Manager::~Alpha_Manager
            ()
            {}

        // methodes
        // --------

        // ----------------------
        // Update acceptance ratios : only use within acceptance ratio at temperature 0 
        double Alpha_Manager::update_acceptance_ratios
            (Eigen::MatrixXd & within_acceptance_ratio, const Eigen::VectorXd & swap_acceptance_ratio, const int & index_walk_choice)
            {
                // update acceptance ratios
                m_last_within_acceptance_ratio (m_compteur_for_mean_acceptance[index_walk_choice], index_walk_choice) = within_acceptance_ratio (0, index_walk_choice);

                for (int temp = 0; temp < m_temperatures.size()-1; temp ++)
                {
                    m_last_swap_acceptance_ratio (m_compteur_for_swap_mean_acceptance, temp) = swap_acceptance_ratio [temp] ;
                }
                m_compteur_for_mean_acceptance [index_walk_choice]      = m_compteur_for_mean_acceptance[index_walk_choice] + 1;
                m_compteur_for_swap_mean_acceptance                     = m_compteur_for_swap_mean_acceptance +1;

                // if cycle_num =modulo m_size_mean_acceptance_ratio 
                if (m_compteur_for_mean_acceptance [index_walk_choice] == m_size_mean_acceptance_ratio)
                {
                    m_compteur_for_mean_acceptance [index_walk_choice]  = 0;                
                    m_mean_within_acceptance_ratios[index_walk_choice]  = m_last_within_acceptance_ratio.col(index_walk_choice).mean();
                }

                if (m_compteur_for_swap_mean_acceptance == m_size_mean_acceptance_ratio)
                {
                    m_compteur_for_swap_mean_acceptance = 0;
                }

                return m_mean_within_acceptance_ratios[0];
            }
    
        // ----------------------------
        // get_last_swap_acceptance_ratio
        Eigen::MatrixXd Alpha_Manager::get_last_swap_acceptance_ratio
            ()
            {
                return m_last_swap_acceptance_ratio;
            }

        // ----------------------------
        // get_mean_within_acceptance_ratios
        Eigen::VectorXd Alpha_Manager::get_mean_within_acceptance_ratio
            ()
            {
                return m_mean_within_acceptance_ratios;
            }

        // ----------------------------
        // get_last_within_acceptance_ratios
        Eigen::MatrixXd Alpha_Manager::get_last_within_acceptance_ratio
            ()
            {
                return m_last_within_acceptance_ratio;
            }

        // ----------------------------

} // namespace ibisa











































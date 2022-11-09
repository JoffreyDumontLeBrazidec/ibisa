/*--------------------------------------------------
 * /IBISA/managers/
 * Alpha_Manager.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/05
 *--------------------------------------------------
 *
 * Implementation of class Alpha_Manager
 * TODO: 
 *
 */

#ifndef DEF_alpha_manager
#define DEF_alpha_manager

#include <main/basic_modules.h>
#include <main/modules.h>

namespace ibisa
{

    class Alpha_Manager

    {
        public:
            // ---------------------------------------
            // Constructeur-Destructeur
            Alpha_Manager (Configuration & config, Eigen::VectorXd & temperatures);

            ~Alpha_Manager ();
            
            // methodes
            double                  update_acceptance_ratios                     
                                        (Eigen::MatrixXd & within_acceptance_ratio, const Eigen::VectorXd & swap_acceptance_ratio, const int & index_walk_choice);
            Eigen::VectorXd         get_mean_within_acceptance_ratio             ();
            Eigen::MatrixXd         get_last_swap_acceptance_ratio              ();
            Eigen::MatrixXd         get_last_within_acceptance_ratio            ();


        private:
            // -------------------------------------
            int                         m_size_mean_acceptance_ratio;             
            Eigen::MatrixXd             m_last_within_acceptance_ratio;
            Eigen::MatrixXd             m_last_swap_acceptance_ratio;      

            Eigen::VectorXi             m_compteur_for_mean_acceptance;                   
            int                         m_compteur_for_swap_mean_acceptance;

            Eigen::VectorXd             m_mean_within_acceptance_ratios;    

            
            Eigen::VectorXd             m_temperatures;
            int                         m_modprint;
    };




} // namespace ibisa

#endif















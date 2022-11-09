/*--------------------------------------------------
 * /IBISA/redoperator/
 * redoperator.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2020/05
 *--------------------------------------------------
 *
 * Implementation of class ReducDim_Obs_Operator
 * TODO: 
 *
 */

#ifndef DEF_ReducDim_Obs_Operator
#define DEF_ReducDim_Obs_Operator


#include <main/basic_modules.h>
#include <include/read.h>
#include <include/LinearAction.h>
#include <include/indexation.h>

namespace ibisa {

    class ReducDim_Obs_Operator

    {

        public:
            // ----------------------------
            // Constructeur-Destructeur
            ReducDim_Obs_Operator (Configuration & config, const int & index_temperature, const int & seed);

            ~ReducDim_Obs_Operator ();
  
            // Méthodes 
            void moving_update                      (const int & column_1, const int & column_2);
            int birth_update                        (const int & index_new_border_in_vec_cities);
            int death_update                        (const int & index_old_border_in_vec_borders);

            void compute_col_D_from_U               (const int & index_left_border);
 
            void update_base_or_proposal            (const int & acceptance); 
           
            // get
            Eigen::VectorXi release_grid_borders    ();
            Eigen::VectorXi release_grid_cities     ();
            int number_borders                      ();
            int number_cities                       ();
            Eigen::MatrixXd get_source              ();



        private:
            // ----------------------------
            // Attributs
            int                         m_number_observations;
            int                         m_number_sources;
            int                         m_number_impulsions_meteo;
            int                         m_number_impulsions;
            int                         m_index_left_border;
            int                         m_index_right_border;
            int                         m_temperature;
            
            tppda::URNG<double,int>     m_ung;
            std::string                 m_trans;
           
            Eigen::MatrixXd             m_D;
            Eigen::MatrixXd             m_D_base;
            Eigen::MatrixXd             m_U;

            Eigen::VectorXi             m_Index;
            Eigen::VectorXi             m_Length;

            int                         m_number_borders;
            int                         m_number_cities;
            int                         m_number_borders_base;
            int                         m_number_cities_base;

            Eigen::VectorXi             m_release_grid_borders;
            Eigen::VectorXi             m_release_grid_borders_base; 
            
            Eigen::VectorXi             m_release_grid_cities;
            Eigen::VectorXi             m_release_grid_cities_base;
    };
} // namespace ibisa
#endif

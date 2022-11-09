/*--------------------------------------------------
 * /IBISA/
 * observationoperator.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class observationOperator
 * TODO: 
 *
 */

#ifndef DEF_observationOperator
#define DEF_observationOperator

#include <main/basic_modules.h>
#include "redoperator/redoperator.h"
#include "include/read.h"

namespace ibisa {

    class observationOperator

    {

        public:
            // -------------------------------
            // constructeur-destructeur
            observationOperator                        (Configuration & config, const int & seed);
            ~observationOperator                       ();
 
            // -------------------------------
            // methods
            Eigen::MatrixXd get_source                  (const double & x1, const double & x2, const int & index_meteo, const int & index_temperature) const;    
            void moving_update                          (const int & t, const int & column_1, const int & column_2);
            int birth_update                            (const int & t, const int & index_new_border_in_vec_cities);
            void death_update                           (const int & t, const int & index_old_border_in_vec_borders);

            void update_base_or_proposal                (const int & index_temperature, const int & acceptance);
            void swap_R_temperatures                    (const int & index_temperature_i, const int & index_temperature_j);

            // get
            Eigen::VectorXi release_grid_cities         (const int & index_temperature);
            Eigen::VectorXi release_grid_borders        (const int & index_temperature);
            int number_borders                          (const int & index_temperature);
            int number_cities                           (const int & index_temperature);

            double get_pas_met                          () const;
            int get_release_period                      ();


        private:
            // -------------------------------
            // Attributs
            int                                         m_number_temperatures   ;
            Eigen::Vector4d                             m_area                  ;
            double                                      m_pas_met               ;
            int                                         m_number_sources        ;
            int                                         m_number_impulsions     ;
            int                                         m_number_borders        ;
            int                                         m_number_observations   ;
            int                                         m_release_period        ;
            Eigen::VectorXi                             m_release_grid_borders  ;
            std::string                                 m_trans                 ;
            
            int                                         m_number_meteos;
            std::vector<Eigen::MatrixXd>                m_list_U;
            
            std::vector<std::unique_ptr<ibisa::ReducDim_Obs_Operator>>  m_list_D;
    };
} // namespace ibisa
#endif

/*--------------------------------------------------
 * /IBISA/redoperator/
 * redoperator.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class ReducDim_Obs_Operator
 *
 */

#include "redoperator.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    
    ReducDim_Obs_Operator::ReducDim_Obs_Operator 
        (Configuration & config, const int & index_temperature, const int & seed)
        : m_ung (seed)
        {
            cout << "ReducDim_Obs_Operator::Init ... " << endl;
            
            if (m_trans != "false")
            {
                m_temperature                           = index_temperature;
                
                m_trans                                 = config.get_string ("release.transdimensionality");
                std::tie (m_Index, m_Length)            = ibisa::indexation (config);                
                std::string meteo_path                  = std::string (config.get_string ("data.meteo.directory")) + std::string (config.get_string ("data.meteo.name"));
                Configuration metcon                    (meteo_path + std::string ("CIMC.cfg")) ;
                m_number_observations                   = metcon.get_integer ("meteo.observations.number"); 
                m_number_sources                        = metcon.get_integer ("meteo.domain.number.sources");
                m_number_impulsions_meteo               = metcon.get_integer ("meteo.impulsions.number");
                m_number_impulsions                     = config.get_integer ("search.impulsions.number");                
                m_index_right_border                    = config.get_integer ("search.impulsions.end");
                m_index_left_border                     = config.get_integer ("search.impulsions.beg");
                int n_digits                            = metcon.get_integer ("meteo.storage.style.ndigit") ;
                
                cout << "ReducDim_Obs_Operator::Meteo_name is " << meteo_path << endl;

                // Fill ReducDim Obs Operator D (m_U with good Dimensions)
                //--------------------------------------------------------
                int index_source                        = 0;
                std::string index_Ucol                  = std::string (n_digits - to_string(index_source).length(), '0') + to_string(index_source);
                std::string dir_I                       = meteo_path + std::string ("/unitpred_cpp/UP") + std::string (index_Ucol) + std::string ("/unitpred_cpp.bin") ; 
                m_U                                     = readf_inMatrixXd_fillInCol (dir_I, m_number_observations, m_number_impulsions_meteo);
                 
                // init release_grid_borders
                m_number_borders                        = m_number_impulsions + 1;
                m_release_grid_borders                  = Eigen::VectorXi::Zero (m_Length[2]+1);
                int width_interval                      = std::floor (m_index_right_border / m_number_impulsions);
                m_release_grid_borders [0]              = m_index_left_border;
                for (int index_interval = 1 ; index_interval < m_number_impulsions  ; index_interval ++)
                {
                    m_release_grid_borders [index_interval] = index_interval * width_interval + std::floor (m_ung.rand (1) * width_interval);
                }                    
                m_release_grid_borders [m_number_impulsions] = m_index_right_border;
                
                // init m_D between left_border and right_border 
                m_D = Eigen::MatrixXd::Zero (m_number_observations, m_Length[2]);
                for (int index_border = 0 ; index_border < m_number_impulsions ; index_border ++)
                {
                    for (int time_release = m_release_grid_borders [index_border] ; time_release < m_release_grid_borders[index_border+1] ; time_release ++)
                    {
                        m_D.col (index_border) =  m_D.col (index_border) + m_U.col (time_release); 
                    }
                }

                // base for proposal (MCMC)
                m_release_grid_borders_base = m_release_grid_borders;
                m_D_base                    = m_D;
                m_number_borders_base       = m_number_borders;
                // Case FULL 
                // init release_grid_cities
                if (m_trans == "full")
                {
                    m_number_cities             = m_index_right_border - m_number_impulsions;
                    m_release_grid_cities       = Eigen::VectorXi::Zero (m_Length[2]);
                    int index_current_border    = 0;
                    int index_current_city      = 0;
                    for (int time_release = 0 ; time_release < m_index_right_border ; time_release ++)
                    {
                        if (m_release_grid_borders [index_current_border] != time_release)
                        {
                            m_release_grid_cities [index_current_city] = time_release;
                            index_current_city  = index_current_city + 1;
                        }
                        
                        if (m_release_grid_borders [index_current_border] == time_release)
                        {
                            index_current_border  = index_current_border + 1;
                        }                        
                    }
                }                
                m_number_cities_base        = m_number_cities;
                m_release_grid_cities_base  = m_release_grid_cities;

            }
        }
      
    ReducDim_Obs_Operator::~ReducDim_Obs_Operator
        ()
        {}
      
    // ------------------------------------------------------------ 
    // Methods
    // return results for an identified source by its coordinates
    Eigen::MatrixXd ReducDim_Obs_Operator::get_source 
        ()
        {
            return m_D;
        }

    // ------------------------------------- 
    // birth_update
    int ReducDim_Obs_Operator::birth_update
        (const int & index_new_border_in_vec_cities)
        {                        
            int new_border              = m_release_grid_cities [index_new_border_in_vec_cities];
            // border = border.head + insertion border + border.tail            
            int index_moving_border     = ibisa::find_position_of_integer_in_vector_of_increasing_integers (new_border, m_release_grid_borders, m_number_borders);
            m_release_grid_borders      = ibisa::add_integer_into_vector_of_increasing_integers (new_border, m_release_grid_borders, m_number_borders, index_moving_border);
            
            // cities = cities oté de la cité qui est devenue frontière
            m_release_grid_cities       = ibisa::remove_integer_from_vector_of_increasing_integers (m_release_grid_cities, m_number_cities, index_new_border_in_vec_cities);
            
            // m_D
            m_D                         = add_col_to_matrix_of_double (m_D, m_number_borders - 1, index_moving_border);
            compute_col_D_from_U        (index_moving_border-1);
            compute_col_D_from_U        (index_moving_border);
            
            m_number_borders            = m_number_borders + 1;
            m_number_cities             = m_number_cities - 1 ;
            
            return index_moving_border;
        }

    // ------------------------------------- 
    // death_update
    int ReducDim_Obs_Operator::death_update
        (const int & index_old_border_in_vec_borders)
        {                        
            int old_border = m_release_grid_borders [index_old_border_in_vec_borders];
            cout << " old border " << m_release_grid_borders [index_old_border_in_vec_borders];
            // cities
            int index_moving_city           = ibisa::find_position_of_integer_in_vector_of_increasing_integers (old_border, m_release_grid_cities, m_number_cities);
            m_release_grid_cities           = ibisa::add_integer_into_vector_of_increasing_integers (old_border, m_release_grid_cities, m_number_cities, index_moving_city);

            // borders = borders oté de la border qui est devenue cité
            m_release_grid_borders          = ibisa::remove_integer_from_vector_of_increasing_integers (m_release_grid_borders, m_number_borders, index_old_border_in_vec_borders);

            // U
            m_D                             = remove_col_from_matrix_of_double (m_D, m_number_borders-1, index_old_border_in_vec_borders);
            compute_col_D_from_U            (index_old_border_in_vec_borders-1);
 
            m_number_borders                = m_number_borders - 1;
            m_number_cities                 = m_number_cities + 1 ;
           
            return index_moving_city;
        }

    // ------------------------------------- 
    // moving update
    void ReducDim_Obs_Operator::moving_update 
        (const int & column_1, const int & column_2)
        {
            for (int index_border = 0 ; index_border < m_number_borders ; index_border ++)
            {
                if (column_1 == m_release_grid_borders [index_border])
                {
                    m_release_grid_borders [index_border]   = column_2;
                    compute_col_D_from_U                    (index_border-1);
                    compute_col_D_from_U                    (index_border);
                }
            }

            if (m_trans == "full")
            {
                for (int index_city = 0 ; index_city < m_number_cities ; index_city ++)
                {
                    if (m_release_grid_cities [index_city] == column_2)
                    {
                        m_release_grid_cities [index_city] = column_1;
                    }
                }   
            }
        }

    // ------------------------------------- 
    // compute the column (index_left_border) in D using U   |B0  col(0)   |B1   col(1) ...   |B(index_left_border)    col(index_left_border)    |...
    // compute the column between index_left_border and index_right_border
    void ReducDim_Obs_Operator::compute_col_D_from_U 
        (const int & index_left_border)
        {
            m_D.col (index_left_border) = Eigen::VectorXd::Zero (m_number_observations);

            for (int time_release = m_release_grid_borders [index_left_border] ; time_release < m_release_grid_borders [index_left_border+1] ; time_release ++)
            {
                m_D.col (index_left_border) =  m_D.col (index_left_border) + m_U.col (time_release); 
            }
        }

    // ------------------------------------- 
    // update m_D or m_D_base
    void ReducDim_Obs_Operator::update_base_or_proposal
        (const int & acceptance)
        {
            if (acceptance == 1)
            {   
                m_D_base                    = m_D;
                m_release_grid_borders_base = m_release_grid_borders;
                m_number_borders_base       = m_number_borders;
                if (m_trans == "full")
                {
                    m_release_grid_cities_base  = m_release_grid_cities;
                    m_number_cities_base        = m_number_cities;
                }
            }

            if (acceptance == 0)
            {   
                m_D                     = m_D_base;
                m_release_grid_borders  = m_release_grid_borders_base;
                m_number_borders        = m_number_borders_base;
                if (m_trans == "full")
                {
                    m_release_grid_cities   = m_release_grid_cities_base;
                    m_number_cities         = m_number_cities_base;
                }
            }
        }

    // ------------------------------------- 
    // return m_release_grid_borders
    Eigen::VectorXi ReducDim_Obs_Operator::release_grid_borders
        ()
        {
            return m_release_grid_borders;
        }

    // ------------------------------------- 
    // return m_release_grid_cities
    Eigen::VectorXi ReducDim_Obs_Operator::release_grid_cities
        ()
        {
            return m_release_grid_cities;
        }

    // ------------------------------------- 
    // return number_borders
    int ReducDim_Obs_Operator::number_borders
        ()
        {
            return m_number_borders;
        }

    // -------------------------------------
    // return number_cities
    int ReducDim_Obs_Operator::number_cities
        ()
        {
            return m_number_cities;
        }

    // ------------------------------------- 


} //namespace ibisa


























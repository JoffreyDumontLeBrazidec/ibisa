/*--------------------------------------------------
 * /IBISA/
 * observationOperator.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class observationOperator
 *
 */

#include "observationOperator.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    observationOperator::observationOperator 
        (Configuration & config, const int & seed)
        : m_number_temperatures (config.get_integer("init.temperature.number")), m_number_meteos (config.get_integer ("init.number.meteo")), m_list_D (m_number_temperatures)
        {

            //--------------------------------------------------------
            // Init parameters of the Unitary Observation Operator matrix
            cout << "observationOperator::Init ... " << endl;

            std::string base_name_meteo     = std::string (config.get_string ("data.meteo.directory")) + std::string (config.get_string ("data.meteo.name"));
            std::string name_meteo          = base_name_meteo;
            
            if (config.get_bool ("chain.search.met") == true)
            {
                name_meteo  = base_name_meteo + std::string ("/membre0")  + std::string("/"); 
            }
            cout << " Path meteo is " << name_meteo << endl;
            Configuration       metcon (name_meteo + std::string ("/CIMC.cfg"));

            m_number_observations           =   metcon.get_integer  ("meteo.observations.number");
            m_number_impulsions             =   metcon.get_integer  ("meteo.impulsions.number");
            m_number_sources                =   metcon.get_integer  ("meteo.domain.number.sources");
            int n_digits                    =   metcon.get_integer  ("meteo.storage.style.ndigit");
            m_release_period                =   metcon.get_integer  ("meteo.impulsions.period");
            m_pas_met                       =   metcon.get_real     ("meteo.domain.spatial.step");
            m_area                          <<  metcon.get_real     ("meteo.domain.spatial.north"), 
                                                metcon.get_real     ("meteo.domain.spatial.west"), 
                                                metcon.get_real     ("meteo.domain.spatial.south"), 
                                                metcon.get_real     ("meteo.domain.spatial.east");            
            
            //--------------------------------------------------------
            // Fill Unitary Observation Operator matrix U
            cout << "observationOperator::In case of several meteorologies or several sources... " << endl;
 
            m_list_U.resize    (m_number_meteos);
            Eigen::MatrixXd U   (m_number_observations * m_number_impulsions, m_number_sources);
            std::string         dir_I, index_Ucol, name_member;
         
            
            std::FILE*  f;
            Eigen::VectorXd     I (m_number_observations * m_number_impulsions) ;
            for (int index_meteo = 0; index_meteo < m_number_meteos; index_meteo ++)
            {                    
                cout << " index_meteo " << index_meteo << endl;
                if (config.get_bool ("chain.search.met") == true)
                {              
                    name_member     = std::string ("/membre") + to_string (index_meteo) + std::string ("/");
                    name_meteo      = base_name_meteo + name_member;
                }
                for (int index_source = 0; index_source < m_number_sources; index_source ++)
                {
                    //
                    index_Ucol                  = std::string (n_digits - to_string(index_source).length(), '0') + to_string(index_source);
                    dir_I                       = name_meteo + std::string ("/unitpred_cpp/UP") + std::string (index_Ucol) + std::string ("/unitpred_cpp.bin") ;
                    f                           = std::fopen (dir_I.c_str(), "rb");
                    std::fread                  (&I[0], sizeof (double), m_number_observations * m_number_impulsions, f);
                    U.col(index_source)         = I;
                    std::fclose                 (f);
                    //
                    
                    //cout << " index_source " << index_source << endl;
                    //index_Ucol                  = std::string (n_digits - to_string(index_source).length(), '0') + to_string (index_source);
                    //dir_I                       = name_meteo + std::string ("/unitpred_cpp/UP") + std::string (index_Ucol) + std::string ("/unitpred_cpp.bin") ;
                    //U.col(index_source)         = ibisa::readf_inVectorXd (dir_I, m_number_observations * m_number_impulsions);
                }                         

                m_list_U [index_meteo] = U;
            }
            
            cout << "observationOperator::In case of transdimensionality... " << endl;

            m_trans = config.get_string ("release.transdimensionality");
            if (m_trans == "false")
            {
                m_release_grid_borders = Eigen::VectorXi::Zero (m_number_impulsions);
                for (int index_impulsion = 0; index_impulsion < m_number_impulsions; index_impulsion ++)
                {
                    m_release_grid_borders[index_impulsion] = index_impulsion;
                }
                m_number_borders = m_number_impulsions;
            }

            if (m_trans != "false")
            {
                for (int index_temperature = 0 ; index_temperature < m_number_temperatures ; index_temperature ++)
                {
                    m_list_D[index_temperature].reset (new ibisa::ReducDim_Obs_Operator (config, index_temperature, seed));
                }
            }
        }
      
    observationOperator::~observationOperator
        ()
        {}
      
    // -------------------------------------------------------------------------------------    
    // Methods
    // return results for an identified source by its coordinates
    Eigen::MatrixXd observationOperator::get_source 
        (const double & x1, const double & x2, const int & index_meteo, const int & index_temperature) const
        {
            if (m_trans == "false")
            {
                int i_mat                   = ((x1-m_area[1]) * ((m_area[0]-m_area[2])/m_pas_met + 1) + (x2 - m_area[2])) / m_pas_met;
                Eigen::MatrixXd U_return    = m_list_U [index_meteo].col (i_mat);
                U_return.resize             (m_number_observations, m_number_impulsions);
                return U_return;
            }

            if (m_trans != "false") 
            {
                return m_list_D [index_temperature] -> get_source ();
            }            
        }

    // ------------------------------------- 
    // return m_pas_met
    double observationOperator::get_pas_met 
        () const
        {
            return m_pas_met;
        }

    // ------------------------------------- 
    // return m_release_period
    int observationOperator::get_release_period
        ()
        {
            return m_release_period;
        }

    // ------------------------------------- 
    // birth_update D of temperature t
    int observationOperator::birth_update
        (const int & t, const int & index_new_border_in_vec_cities)
        {
            return m_list_D [t] -> birth_update (index_new_border_in_vec_cities);
        }

    // ------------------------------------- 
    // death_update D of temperature t
    void observationOperator::death_update
        (const int & t, const int & index_old_border_in_vec_borders)
        {
            m_list_D [t] -> death_update (index_old_border_in_vec_borders);
        }

    // ------------------------------------- 
    // update base or proposal
    void observationOperator::update_base_or_proposal 
        (const int & index_temperature, const int & acceptance)
        {
            m_list_D [index_temperature] -> update_base_or_proposal (acceptance);
        }

    // ------------------------------------- 
    // moving_update D of temperature t
    void observationOperator::moving_update 
        (const int & t, const int & column_1, const int & column_2)
        {
            m_list_D [t] -> moving_update (column_1, column_2);
        }

    
    // ------------------------------------- 
    // swap R_t(i) and R_t(j)
    void observationOperator::swap_R_temperatures
        (const int & index_temperature_i, const int & index_temperature_j)
        {
            std::iter_swap (m_list_D.begin() + index_temperature_i, m_list_D.begin() + index_temperature_j);
        }

    // ------------------------------------- 
    // return m_release_grid_borders
    Eigen::VectorXi observationOperator::release_grid_borders
        (const int & index_temperature)
        {
            if (m_trans == "false")
            {
                return m_release_grid_borders; 
            }

            if (m_trans != "false")
            {
                return m_list_D [index_temperature] -> release_grid_borders ();
            }
        }

    // ------------------------------------- 
    // return m_release_grid_cities
    Eigen::VectorXi observationOperator::release_grid_cities
        (const int & index_temperature)
        {
            return m_list_D [index_temperature] -> release_grid_cities ();
        }

    // ------------------------------------- 
    // return m_list_D[t].number_borders
    int observationOperator::number_borders
        (const int & index_temperature)
        {
            if (m_trans == "false")
            {
                return m_number_borders; 
            }

            if (m_trans != "false")
            {
                return m_list_D [index_temperature] -> number_borders ();               
            }                       
        }

    // ------------------------------------- 
    // return m_list_D[t].number_cities
    int observationOperator::number_cities
        (const int & index_temperature)
        {
            return m_list_D [index_temperature] -> number_cities ();
        }

    // ------------------------------------- 

} //namespace ibisa


























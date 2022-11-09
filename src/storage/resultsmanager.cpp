/*--------------------------------------------------
 * /IBISA/storage/
 * resultsmanager.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Results_Manager
 *
 */

#include "resultsmanager.h"
#include "include/indexation.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace Eigen;

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    Results_Manager::Results_Manager 
        (Configuration & config)
        {
            // Load configuration   
            std::tie (m_Index, m_Length) = ibisa::indexation (config);

            m_cycles                        = config.get_integer    ("control.sizeLoop");
            m_storing_size_predictions      = config.get_integer    ("control.store.predictions");
            m_number_temperatures           = config.get_integer    ("init.temperature.number");
            Configuration metcon            = ibisa::download_associated_CIMC (config);
            m_number_obs                    = metcon.get_integer    ("meteo.observations.number");
            m_directory                     = config.get_string     ("orga.directory") + "/" + config.get_string ("orga.folder");     
        }

    Results_Manager::~Results_Manager()
    {}

    // -------------------------------------------------------------------------------------    
    
    void Results_Manager::initiate_vector_of_result_vectors 
        (std::vector<Eigen::MatrixXd> & m_vector_of_result_vectors)
        {
            // VEC
            // *
            m_vec_natures = {};
            m_vector_of_result_vectors.resize (6);
            
            // predictions : 0 
            m_vec_natures.push_back ("predictions");
            m_vector_of_result_vectors[0].resize (m_storing_size_predictions, m_number_obs);
            
            // release_grid_borders base : 1
            m_vec_natures.push_back ("release_grid_borders_base");
            m_vector_of_result_vectors[1].resize (m_cycles, m_Length[2]+1);

            // within acceptance : 2
            m_vec_natures.push_back ("within_acceptance");
            m_vector_of_result_vectors[2].resize (m_cycles, m_number_temperatures-1);

            // swap acceptance : 3
            m_vec_natures.push_back ("swap_acceptance");
            m_vector_of_result_vectors[3].resize (m_cycles, m_number_temperatures-1);

            // cost : 4
            m_vec_natures.push_back ("cost");
            m_vector_of_result_vectors[4].resize (m_cycles, m_number_temperatures-1);

            // indexing observation error : 5
            m_vec_natures.push_back ("indexing_observationerror");
            m_vector_of_result_vectors[5].resize (1, m_number_obs);
        }

    // ------------------------------------------------

    void Results_Manager::initiate_vector_of_result_reals 
        (std::vector<Eigen::VectorXd> & m_vector_of_result_reals) 
        {
            // REAL
            // *
            m_real_natures = {};
            m_vector_of_result_reals.resize (3);

            // cost_base : 0 
            m_real_natures.push_back ("cost_base");
            m_vector_of_result_reals[0].resize (m_cycles);

            // within acceptance base : 1
            m_real_natures.push_back ("within_acceptance_base");
            m_vector_of_result_reals[1].resize (m_cycles);

            // size indexing observation error : 2
            m_real_natures.push_back ("size_indexing_observationerror");
            m_vector_of_result_reals[2].resize (m_cycles);
           
        }

    // ------------------------------------------------

    std::string Results_Manager::return_savename_of_real 
        (const int & index_real_to_save)
        {
            std::string savename = m_directory + "/temperature00/" + m_real_natures [index_real_to_save] + ".bin";
            return savename;
        }

    // ------------------------------------------------

    std::string Results_Manager::return_savename_of_vector 
        (const int & index_vector_to_save)
        {
            std::string savename;
            
            if (index_vector_to_save == 2 or index_vector_to_save == 3 or index_vector_to_save == 4)
            {
                savename = m_directory + "/all_temperatures/";
            }
            
            if (index_vector_to_save == 0 or index_vector_to_save == 1 or index_vector_to_save == 5)
            {
                savename = m_directory + "/temperature00/";
            }
            
            savename = savename + m_vec_natures [index_vector_to_save] + ".bin";
            
            return savename;
        }

    // ------------------------------------------------

} //namespace ibisa


























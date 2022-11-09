/*--------------------------------------------------
 * /IBISA/
 * sortingObs.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class sortingObs
 *
 */

#include "sortingObs.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    sortingObs::sortingObs 
        (Configuration & config, const Eigen::Vector2i & seed)
        : m_rng (seed[0], 0, 1), m_ung (seed[1])
        {
            cout << "sortingObs::Init ...";
            
            std::tie (m_Index, m_Length)        = ibisa::indexation (config);
            
            // download observations set
            cout << "sortingObs::ObsSet ...";
            Configuration metcon                        = ibisa::download_associated_CIMC (config); 
            std::string dir_y                           = metcon.get_string ("meteo.observations.directory");
            m_number_obs                                = metcon.get_integer ("meteo.observations.number");
            int number_info_obs                         = 4;
            m_y                                         = ibisa::readf_inMatrixXd_fillInRow (dir_y, m_number_obs, number_info_obs);
           
            // download informations related to observation sorting algorithm
            cout << "sortingObs::ObsSortingAlgo ...";

            m_reduction_number_obs                      = config.get_bool ("data.reduce.obs");
            int number_infos_labels                     = 3;
            std::string dir_labels                      = config.get_string ("data.obs.labels");            
            m_basic_index_observationerror              = ibisa::readf_inMatrixXi_fillInRow (dir_labels, m_number_obs, number_infos_labels);
            m_index_observationerror                    = m_basic_index_observationerror.col (2);
            m_decision_remove_obs                       = config.get_real ("data.reduce.obs.limit");
            m_size_indexing_observationerror            = Eigen::VectorXi::Zero (std::max (m_Length[3],1));
            for (int index_obs = 0 ; index_obs < m_number_obs ; index_obs++)
            {
                m_size_indexing_observationerror [m_index_observationerror[index_obs]] =  m_size_indexing_observationerror [m_index_observationerror[index_obs]] + 1; 
            }

            // tri general
            m_tri_obs                       = config.get_bool    ("data.tri.general.obs");
            if (m_tri_obs == true)
            {
                m_number_modif_hyper    = config.get_integer ("data.tri.general.obs.walk.number");
            }
            
            cout << "done!" << endl;
        }   

      
    sortingObs::~sortingObs
        (){}
      
    // -------------------------------------------------------------------------------------    
    // Methods
    // reduce_set_obs
    void sortingObs::reduce_set_obs 
        (const Eigen::VectorXd & yS)
        {
            for (int i = 0 ; i < m_number_obs ; i ++)
            {
                if (std::abs (yS[i] - m_y(i,3)) < m_decision_remove_obs)
                {
                    m_size_indexing_observationerror    [m_index_observationerror[i]]   = m_size_indexing_observationerror  [m_index_observationerror[i]] - 1 ;
                    m_size_indexing_observationerror    [m_Length[3]-1]                 = m_size_indexing_observationerror  [m_Length[3]-1] + 1 ;
                    m_index_observationerror            [i]                             = m_Length [3] - 1 ;
                }
            }            
        }

    // -------------------------------------
    // enhance set obs
    bool sortingObs::enhance_set_obs 
        (const Eigen::VectorXd & yS)
        {
            bool indicator_of_model_change  = false;

            for (int i = 0 ; i < m_number_obs ; i ++)
            {
                if (m_index_observationerror[i] == m_Length[3]-1 and std::abs (yS[i]-m_y(i,3)) > m_decision_remove_obs)
                {
                    indicator_of_model_change                                               = true;
                    m_index_observationerror[i]                                             = m_basic_index_observationerror (i,2);
                    m_size_indexing_observationerror [m_basic_index_observationerror(i,2)]  = m_size_indexing_observationerror [m_basic_index_observationerror(i,2)] + 1 ;
                    m_size_indexing_observationerror [m_Length[3]-1]                        = m_size_indexing_observationerror [m_Length[3]-1] - 1 ;
                }
            }        
            return indicator_of_model_change;
        }         

    // -------------------------------------
    // enhance set obs : algorithme de pertinence est probablement aussi appliqué dans le même temps
    void sortingObs::general_sorting_walk
        ()
        {
            int indexation_range_max;
            if (m_reduction_number_obs == true)
            {            
                indexation_range_max = m_Length[3] - 1;
            }
            
            if (m_reduction_number_obs == false)
            {            
                indexation_range_max = m_Length[3];
            }
                
            int index_hyper_modif;
            int old_indexation, new_indexation;
                
            for (int i = 0; i < m_number_modif_hyper; i ++)
            {
                index_hyper_modif                                       = std::floor (m_ung.rand(1) * m_number_obs);
                old_indexation                                          = m_index_observationerror [index_hyper_modif];
                while (old_indexation == indexation_range_max)
                {
                    index_hyper_modif   = std::floor (m_ung.rand(1) * m_number_obs);
                    old_indexation      = m_index_observationerror [index_hyper_modif];
                }
                new_indexation                                          = std::floor (m_ung.rand(1) * indexation_range_max);
                m_index_observationerror[index_hyper_modif]             = new_indexation;
                m_size_indexing_observationerror[old_indexation]        = m_size_indexing_observationerror[old_indexation] - 1;
                m_size_indexing_observationerror[new_indexation]        = m_size_indexing_observationerror[new_indexation] + 1;
            }           
        }

    //--------------------------------------
    // get size indexing observationerror
    Eigen::VectorXi sortingObs::get_size_indexing_observationerror 
        ()
        {
            return m_size_indexing_observationerror.transpose();
        }

    // -------------------------------------
    // get index observationerror
    const Eigen::VectorXi sortingObs::get_index_observationerror 
        ()
        {
            return m_index_observationerror;
        }

    // -------------------------------------

} //namespace ibisa


























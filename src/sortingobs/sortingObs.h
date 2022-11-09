/*
 * sortingObs.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class sortingObs
 * TODO: 
 *
 */

#ifndef DEF_sortingObs
#define DEF_sortingObs

#include <main/basic_modules.h>

#include "observationOperator/observationOperator.h"
#include "include/indexation.h"
#include "include/read.h"
#include <include/configHelper.h>

namespace ibisa {

    class sortingObs
    {

        public:
            // --------------------------------
            // Constructeur-Destructeur
            sortingObs (Configuration & config, const Eigen::Vector2i & seed);

            ~sortingObs ();
  
            // Méthodes
            void                        reduce_set_obs                      (const Eigen::VectorXd & yS);
            bool                        enhance_set_obs                     (const Eigen::VectorXd & yS);
            const Eigen::VectorXi       get_index_observationerror          ();
            Eigen::VectorXi             get_size_indexing_observationerror  ();

            void                        general_sorting_walk                ();


        private:
            // --------------------------------------
            // Attributs
            Eigen::VectorXi             m_Index;
            Eigen::VectorXi             m_Length;
            tppda::GRNG<double,int>     m_rng;
            tppda::URNG<double,int>     m_ung;
            
            // Constants
            Eigen::MatrixXd             m_y; 
            int                         m_number_obs; 
            bool                        m_reduction_number_obs;
            double                      m_decision_remove_obs;            
            
            bool                        m_tri_obs;
            int                         m_number_modif_hyper;

            // other variables
            Eigen::VectorXi             m_index_observationerror;
            Eigen::VectorXi             m_size_indexing_observationerror;
            Eigen::MatrixXi             m_basic_index_observationerror;            
    };
} // namespace ibisa
#endif

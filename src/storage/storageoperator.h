/*--------------------------------------------------
 * /IBISA/
 * storageoperator.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class STORAGE_OPERATOR
 * TODO: 
 *
 */

#ifndef DEF_STORAGE_OPERATOR
#define DEF_STORAGE_OPERATOR

#include <main/basic_modules.h>
#include "storage/resultsmanager.h"
#include "include/indexation.h"
#include "include/tools.h"

namespace ibisa {

    class Storage_Operator

    {

        public:
            // ------------------------------
            // Constructeur-Destructeur
            Storage_Operator            (Configuration & config, std::string & config_file);

            ~Storage_Operator           ();
  
            // Méthodes
    
            void store_state            (const Eigen::MatrixXd & state);
            void store_vec              (const Eigen::VectorXd & vector, const int & index_nature_vector);
            void store_real             (const double & real, const int & index_nature_real);

            void write_to_file          ();
            void write_state_to_file    ();
            void write_vec_to_file      ();
            void write_real_to_file     ();
   
            void store_procedure        (const std::string & filename, const Eigen::VectorXd & data_to_save);

        private: 
            // ------------------------------
            // Attributs

            int                                 m_cycles;
            int                                 m_burn_in;
            int                                 m_current_cycle;
            bool                                m_decision_save_all_temperatures;
            int                                 m_number_temperatures;
            std::string                         m_directory;
            std::string                         m_ambition;
            ibisa::Results_Manager              m_R;

            Eigen::VectorXi                     m_Index;
            Eigen::VectorXi                     m_Length;

            std::vector<Eigen::MatrixXd>        m_store_state;
            std::vector<Eigen::MatrixXd>        m_vector_of_vecRes;
            std::vector<Eigen::VectorXd>        m_vector_of_realRes;
            Eigen::VectorXi                     m_vecRes_storage_index;
            Eigen::VectorXi                     m_realRes_storage_index;

    };

} // namespace ibisa
#endif

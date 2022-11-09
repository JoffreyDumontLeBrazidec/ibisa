/*--------------------------------------------------
 * /IBISA/storage/
 * resultsmanager.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Results_Manager
 * TODO: 
 *
 */

#ifndef DEF_Results_Manager
#define DEF_Results_Manager

#include <main/basic_modules.h>
#include <include/configHelper.h>

namespace ibisa {

    class Results_Manager

    {

        public:
            // ---------------------------------------
            // Constructeur-Destructeur
            Results_Manager (Configuration & config);

            ~Results_Manager ();
  
            // Méthodes
            void                                initiate_vector_of_result_vectors   (std::vector<Eigen::MatrixXd> & m_vector_of_result_vectors);
            void                                initiate_vector_of_result_reals     (std::vector<Eigen::VectorXd> & m_vector_of_result_reals);
            std::string                         return_savename_of_real             (const int & index_real_to_save);
            std::string                         return_savename_of_vector           (const int & index_vector_to_save);
            
        private:
            // -------------------------------------
            // Attributs
            Eigen::VectorXi                     m_Index;
            Eigen::VectorXi                     m_Length;

            int                                 m_cycles;
            int                                 m_storing_size_predictions;
            int                                 m_number_temperatures;
            int                                 m_number_obs;
            std::string                         m_directory;

            std::vector<std::string>            m_vec_natures;
            std::vector<std::string>            m_real_natures;

    };
} // namespace ibisa
#endif

/*--------------------------------------------------
 * /IBISA/
 * StandardState.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class StandardState
 * TODO: 
 *
 */

#ifndef DEF_StandardState
#define DEF_StandardState

#include <main/basic_modules.h>
#include <include/indexation.h>

namespace ibisa {
    
    class StandardState
    
    {
        public:
            // ---------------------------------------------
            // Constructeur-Destructeur
            StandardState       (Configuration & config, const Eigen::MatrixXd & state, const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror);
            ~StandardState      ();
  
            // Méthodes
            void                copy                                        (const Eigen::MatrixXd & state, const Eigen::VectorXd & cost, const Eigen::VectorXi & current_number_impulsions);
            void                copy_t                                      (const int & t, const Eigen::VectorXd & state, const double & cost, const int & current_number_impulsions);            
            void                copy_indexing_obs                           (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror);
            
            void                                set_cost_high                       ();

            // get
            Eigen::VectorXd                     get_cost                            ();
            double                              get_cost_t                          (const int & t);
            Eigen::MatrixXd                     get_state                           ();
            Eigen::VectorXd                     get_state_t                         (const int & t);
            Eigen::VectorXi                     get_current_number_impulsions       ();
            int                                 get_current_number_impulsions_t     (const int & t);
            Eigen::VectorXi                     get_index_observationerror          ();
            Eigen::VectorXi                     get_size_indexing_observationerror  ();

        private:
            // ----------------------------------------------------
            // Attributs
            Eigen::VectorXi                     m_Index;
            Eigen::VectorXi                     m_Length;

            Eigen::MatrixXd                     m_state;
            Eigen::VectorXi                     m_current_number_impulsions;
            Eigen::VectorXd                     m_cost;
            Eigen::VectorXi                     m_size_indexing_observationerror;
            Eigen::VectorXi                     m_index_observationerror;

            Eigen::VectorXd                     m_temperatures;
            int                                 m_number_temperatures;
    };
} //namespace ibisa
#endif



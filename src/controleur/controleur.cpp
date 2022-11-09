/*--------------------------------------------------
 * /IBISA/controleur/
 * controleur.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class Controleur
 *
 */

#include "controleur.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    Controleur::Controleur 
        (Configuration & config)
        {
            
            cout << "Contrôle des papiers!" << endl;
            
            std::string m_trans                                 = config.get_string         ("release.transdimensionality");
            std::string meteo_dir                               = config.get_string         ("data.meteo.directory");            
            std::string meteo_name                              = config.get_string         ("data.meteo.name");
            std::string meteo_path                              = meteo_dir + meteo_name;
            Configuration metcon                                (meteo_path + std::string ("CIMC.cfg"));
            cout << "Controleur::meteo_path is..." << meteo_path<< endl;
            int m_number_obs                                    = metcon.get_integer        ("meteo.observations.number")      ;
            int m_number_impulsions_meteo                       = metcon.get_integer        ("meteo.impulsions.number")        ;
            int m_number_impulsions                             = config.get_integer        ("search.impulsions.number")       ;
            int m_number_sources                                = metcon.get_integer        ("meteo.domain.number.sources")    ;
            int m_beg_q                                         = config.get_integer        ("search.impulsions.beg");
            int m_right_border                                  = config.get_integer        ("search.impulsions.end");
            cout << "Controleur:: Liste 2 de papiers à jour ..." << endl;
            Eigen::VectorXi m_Index;
            Eigen::VectorXi m_Length;
            std::tie (m_Index, m_Length)                        = ibisa::indexation         (config);

            double logreleasevalueprior                         = config.get_real ("distribution.prior.log_release.value");
            double searchlogreleasemin                          = config.get_real ("search.log_release.min");

            // reduction obs
            // *
            bool m_reduction_number_obs                             = config.get_bool           ("data.reduce.obs");
            int number_infos_labels                                 = 3;
            std::string dir_labels                                  = config.get_string ("data.obs.labels");
            Eigen::MatrixXi m_basic_index_observationerror          = ibisa::readf_inMatrixXi_fillInRow (dir_labels, m_number_obs, number_infos_labels);
            Eigen::VectorXi m_index_observationerror                = m_basic_index_observationerror.col (2);
            double m_decision_remove_obs                            = config.get_real   ("data.reduce.obs.limit");
            Eigen::VectorXi m_size_indexing_observationerror        = Eigen::VectorXi::Zero (std::max (m_Length[3],1));
            for (int index_obs = 0 ; index_obs < m_number_obs ; index_obs++)
            {
                m_size_indexing_observationerror [m_index_observationerror[index_obs]] =  m_size_indexing_observationerror [m_index_observationerror[index_obs]] + 1;
            }
            
            cout << "Controleur:: Liste *Reduc Obs* de papiers à jour ..." << endl;

            // Tri général
            bool m_tri_obs                       = config.get_bool    ("data.tri.general.obs");
            int m_number_modif_hyper            = config.get_integer ("data.tri.general.obs.walk.number");
            int number_type_walks               = config.get_integer ("number.type.walks.max");
            cout << " Controleur:: List tri général de papiers à jour ..." << endl;

            // -

            int m_control_draw                                  = config.get_integer        ("control.draw");
            int m_control_sizeloop                              = config.get_integer        ("control.sizeLoop");
            std::string ambition                                = config.get_string         ("orga.ambition");
            cout << "Controleur:: Liste 3 de papiers à jour ..." << endl;

            if (m_number_sources > 1 and m_trans != "false")
            {
                cout << "Controleur::ATTENTION: Nombre de sources > 1 et transdimensionnalité est un cas non autorisé !" << endl;                                                                            
                throw std::exception();                           
            }
            
            if (config.get_bool ("chain.search.met") == true and m_trans != "false")
            {
                cout << "Controleur::ATTENTION: ReducDim_Obs_Operator: Ensemble meteo + RJ-MCMC non codé..." << endl;   
                throw std::exception();
            }

/*            if (m_number_impulsions_meteo != m_number_impulsions and m_trans != "false")
            {
                cout << "Controleur::ATTENTION : number of impulsions max of the H should be equal to number of impulsion max chosen if you want to do RJ-MCMC" << endl;
                throw std::exception();
            }*/
        
            if (m_reduction_number_obs == true and m_Length[3] < 2) 
            {
                cout << "Controleur::ATTENTION: reduction obs activé et m_r de taille < 2 !" << endl;
                throw std::exception();                                                                                                                                                                
            }

            if (m_reduction_number_obs == false and m_Length[3] > 1)
            {                                                                                                        
                cout << "Controleur::ATTENTION: code n'est pas fait pour utiliser l'algorithme label sans l'algorithme reduction nombre observations !" << endl;                                                                                                                            
                throw std::exception();                                                                                                                                
            }

            if (m_control_draw > m_control_sizeloop and ambition == "result")
            {
                cout << "Controleur::ATTENTION: draw plus petit que sizeloop et ton ambition est d'avoir des résultats" << endl;
                throw std::exception();
            }      

            if (ambition != "test" and ambition != "result")
            {
                cout << "Controleur::ATTENTION:: Vous avez un problème d'ambition..." << endl;
                throw std::exception();
            }
       
            bool check_path_meteo       = ibisa::IsPathExist (meteo_path + "/unitpred_cpp");        
            if (check_path_meteo == false)
            {
                cout << "Controleur::ATTENTION: Path Meteo n'a pas de unitpred_cpp" << endl;
                //throw std::exception();
            }

            bool check_path_labels      = ibisa::IsPathExist (config.get_string ("data.obs.labels"));
            if (check_path_labels == false)
            {
                cout << "Controleur::ATTENTION: Path labels n'existe pas ..." << endl;
                throw std::exception();
            }

            bool check_path_obs         = ibisa::IsPathExist (config.get_string ("data.obstxt.path"));       
            if (check_path_obs == false)
            {
                cout << "Controleur::ATTENTION: Path observations n'existe pas ..." << endl;
                throw std::exception();
            }

            bool check_path_dir         = ibisa::IsPathExist (config.get_string ("orga.directory"));
            if (check_path_dir == false)
            {
                cout << "Controleur::ATTENTION: Path directory n'existe pas ..." << endl;
                throw std::exception();
            }
      
            if (m_size_indexing_observationerror.size() != m_Length[3] and m_Length[3] > 0)
            {
                cout << "Controleur::ATTENTION: Size Indexing Observation Error et Length Var Obs Error ne sont pas de même taille ..." << endl;
                cout << "m_size_indexing_observationerror.size() " << m_size_indexing_observationerror.size() << endl;
                cout << "m_Length[3] " << m_Length[3] << endl;
                throw std::exception();
            }

            if (m_index_observationerror.maxCoeff() > m_Length[3])
            {
                cout << "Controleur::ATTENTION: maximum Coeff de Index Observation Error plus grand que sa possible indexation dans Var Obs Error ..." << endl;
                throw std::exception();
            }

            if (m_trans != "false" and m_beg_q != 0)
            {
                cout << "Controleur::ATTENTION: transition activé avec un beg q différent de 0 ..." << endl;
                throw std::exception();
            }

            if (m_right_border > m_number_impulsions_meteo)
            {
                cout << "Controleur::ATTENTION: right border too big! ..." << endl;
                throw std::exception();
            }
           
/*            if (logreleasevalueprior != searchlogreleasemin)
            {
                cout << "Controleur::ATTENTION: logreleasevalue prior et searchlogreleasemin pas égaux!! ..." << endl;
                throw std::exception();
            }
            */

        cout << " CONTROLES PASS " << endl;
        
        }
    
    
    Controleur::~Controleur
        ()
        {}

    // ------------------------------------------------

} //namespace ibisa


























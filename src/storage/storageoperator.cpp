/*--------------------------------------------------
 * /IBISA/
 * storageoperator.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class STORAGE_OPERATOR
 *
 */

#include "storageoperator.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    Storage_Operator::Storage_Operator 
        (Configuration & config, std::string & config_file) 
        : m_R (config)
        {

            // Load configuration
            // *
            cout << "Storage_Operator::Load configuration... " << endl;
            
            std::tie (m_Index, m_Length)    = ibisa::indexation (config);
            
            m_number_temperatures               = config.get_integer    ("init.temperature.number"); 
            m_cycles                            = config.get_integer    ("control.sizeLoop");
            m_decision_save_all_temperatures    = config.get_bool       ("control.savealltemp");

            m_store_state.resize (m_number_temperatures);
            for (int t(0); t < m_number_temperatures; t ++) 
            {
                m_store_state[t].resize (m_cycles, m_Length.sum());
            }
            m_current_cycle     = 0;
            
            m_R.initiate_vector_of_result_vectors       (m_vector_of_vecRes);
            m_R.initiate_vector_of_result_reals         (m_vector_of_realRes) ;
            m_vecRes_storage_index                      = Eigen::VectorXi::Zero (m_vector_of_vecRes.size());
            m_realRes_storage_index                     = Eigen::VectorXi::Zero (m_vector_of_realRes.size());
            
            // MKDIR
            m_directory                         = config.get_string ("orga.directory") + "/" + config.get_string ("orga.folder");
            m_ambition                          = config.get_string ("orga.ambition");    
            std::string name_config_to_save     = "config.cfg";

            ibisa::createFolder             (m_directory, m_ambition);
            
            if (m_ambition != "test" or ibisa::IsPathExist (m_directory + name_config_to_save) == false)
            {
                ibisa::saveCfgFile          (config_file, m_directory, name_config_to_save);
            }
        }
      
    Storage_Operator::~Storage_Operator
        ()
        {}
      
    //--------------------------------------------------------------------------------------    
    // Methods
    void Storage_Operator::store_state 
        (const Eigen::MatrixXd & state)
        {
            for (int t(0) ; t < m_number_temperatures ; t ++) 
            {
                m_store_state[t].row (m_current_cycle) = state.row (t) ; 
            }
            m_current_cycle = m_current_cycle + 1;
        }

    //--------------------------------------
    // store_vec
    void Storage_Operator::store_vec 
        (const Eigen::VectorXd & vector, const int & index_nature_vector)
        {
            m_vector_of_vecRes [index_nature_vector].row (m_vecRes_storage_index [index_nature_vector]) = vector;
            m_vecRes_storage_index [index_nature_vector]                                                = m_vecRes_storage_index [index_nature_vector] + 1;
        }

    //--------------------------------------
    // store_real
    void Storage_Operator::store_real 
        (const double & real, const int & index_nature_real)
        {
            m_vector_of_realRes [index_nature_real] [m_realRes_storage_index[index_nature_real]]   = real;
            m_realRes_storage_index[index_nature_real]                                             = m_realRes_storage_index[index_nature_real] + 1;
        }

    //-------------------------------------- 
    // write_to_file
    void Storage_Operator::write_to_file
        ()
        {
            write_state_to_file ();
            write_vec_to_file   ();
            write_real_to_file  ();
            cout << "Storage_Operator: Results written in directory " << m_directory << " !" << endl;
        }

    //-------------------------------------- 
    // write_state_to_file
    void Storage_Operator::write_state_to_file
        ()
        {
            std::string     filename;
            std::string     directory_t ;
            std::string     index_temperature;
     
            Eigen::VectorXd data_to_save (m_cycles);

            int n_digits_temp = 2;            
            int number_temperatures_to_save = 1;

            if (m_decision_save_all_temperatures == true)
            {   
                number_temperatures_to_save         = m_number_temperatures;
                
                std::string directory_temperatures  = m_directory + "/all_temperatures";
                ibisa::createFolder                 (directory_temperatures, m_ambition);
                
            }
            
            for (int t = 0; t < number_temperatures_to_save ; t ++)
            {
                index_temperature               = std::string (n_digits_temp - to_string(t).length(), '0') + to_string(t);
                directory_t                     = m_directory + "/temperature" + std::string(index_temperature) + "/";
                ibisa::createFolder             (directory_t, m_ambition);

                // store coordinates
                if (m_Index[0] != -1)
                {
                    filename        = directory_t + "/longitude.bin";
                    data_to_save    = m_store_state[t].col(m_Index[0]);
                    store_procedure (filename, data_to_save);               

                    filename        = directory_t + "/latitude.bin";
                    data_to_save    = m_store_state[t].col(m_Index[1]);
                    store_procedure (filename, data_to_save);
                }

                // store release
                if (m_Index[2] != -1)
                {
                    for (int i(0) ; i < m_Length[2] ; i++)
                    {
                        int n_digits            = 3;
                        std::string index_Ucol  = std::string (n_digits - to_string(i).length(), '0') + to_string(i);
                        filename                = directory_t + "/release_" + std::string(index_Ucol) + ".bin";
                        data_to_save            = m_store_state[t].col(m_Index[2]+i);
                        store_procedure         (filename, data_to_save);                   
                    }
                }

                // store observationerror
                if (m_Index[3] != -1)
                {
                    for (int i(0) ; i < m_Length[3] ; i++)
                    {
                        int n_digits            = 3;
                        std::string index_Ucol  = std::string (n_digits - to_string(i).length(), '0') + to_string(i);
                        filename                = directory_t + "/observationerror_" + std::string(index_Ucol) + ".bin"; 
                        data_to_save            = m_store_state[t].col(m_Index[3]+i);
                        store_procedure         (filename, data_to_save);                   
                    }
                }

                // store meteo
                if (m_Index[4] != -1)
                {
                    for (int i(0) ; i < m_Length[4] ; i++)
                    {
                        int n_digits            = 3;
                        std::string index_Ucol  = std::string(n_digits - to_string(i).length(), '0') + to_string(i);
                        filename                = directory_t + "/met_" + std::string(index_Ucol) + ".bin";
                        data_to_save            = m_store_state[t].col(m_Index[4]+i);
                        store_procedure         (filename, data_to_save);                   
                    }
                }

                // store regularisation variance term
                if (m_Index[5] != -1)
                {
                    for (int i(0); i < m_Length[5]; i++)
                    {
                        int n_digits            = 3;
                        std::string index_Ucol  = std::string (n_digits - to_string(i).length(), '0') + to_string(i);
                        filename                = directory_t + "/regpriorterm_" + std::string(index_Ucol) + ".bin";
                        data_to_save            = m_store_state[t].col (m_Index[5]+i);
                        store_procedure         (filename, data_to_save);
                    }
                }
               
            } 
        }
     
    //-------------------------------------- 
    // write_vec_to_file
    void Storage_Operator::write_vec_to_file 
        ()
        {
            //Eigen::VectorXd data_to_save;
            std::string filename;
            for (int index_vector_to_save = 0; index_vector_to_save < m_vecRes_storage_index.size() ; index_vector_to_save++)
            {
                if (m_vecRes_storage_index[index_vector_to_save] > 0)                
                {
                    filename                            = m_R.return_savename_of_vector (index_vector_to_save);
                    Eigen::VectorXd data_to_save        (Eigen::Map<Eigen::VectorXd> (m_vector_of_vecRes[index_vector_to_save].data(), 
                                                        m_vector_of_vecRes[index_vector_to_save].cols() * m_vector_of_vecRes[index_vector_to_save].rows()));
                    store_procedure                     (filename, data_to_save);
                }
            }
        }

    //-------------------------------------- 
    // write_real_to_file
    void Storage_Operator::write_real_to_file 
        ()
        {
            Eigen::VectorXd data_to_save;
            std::string filename;
            for (int index_real_to_save = 0; index_real_to_save < m_realRes_storage_index.size() ; index_real_to_save++)
            {
                if (m_realRes_storage_index [index_real_to_save] > 0)
                {
                    filename        = m_R.return_savename_of_real (index_real_to_save);
                    data_to_save    = m_vector_of_realRes [index_real_to_save];
                    store_procedure (filename, data_to_save);
                }
            }
        }
        
    //-------------------------------------- 
    // store_procedure
    void Storage_Operator::store_procedure 
        (const std::string & filename, const Eigen::VectorXd & data_to_save)
        {
            std::ofstream   out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
            out.write       ((char*) data_to_save.data(), data_to_save.size()*sizeof(double) );
            out.close       ();
        }

    //-------------------------------------- 

} //namespace ibisa


























/*--------------------------------------------------
 * /IBISA/include/
 * tools.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class tools
 * TODO: 
 *
 */


#include <include/tools.h>

namespace ibisa
{
    
    // ----------------------------------
    // IsPathExist
    bool IsPathExist 
        (const std::string & path)
        {   
            struct stat buffer;
            return (stat (path.c_str(), &buffer) == 0);
        }

    // ----------------------------------
    // saveCfgFile
    void saveCfgFile 
        (std::string & config_file_original, std::string & directory_of_save, std::string & name_save)
        {
            std::ifstream                       src (config_file_original, std::ios::binary);
            std::ofstream                       dst (directory_of_save + "/" + name_save, std::ios::binary);
            dst                                 << src.rdbuf ();
        }
    
    // ----------------------------------
    // createFolder
    void createFolder
        (std::string & directory, std::string & ambition)
        {
            if (mkdir (directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) // creation folder
            {
                if (ambition == "test")
                {
                    cout << "tools/createFolder - Directory " << directory << " already exists, and can't be created." << endl;
                }

                else if (ambition == "result")
                {
                    cout << "tools/createFolder - Directory " << directory << " already exists and computation must stop..." << endl;
                    throw std::exception();
                }
            }
            
            else
            {
                cout << "Creation of folder : " << directory << endl;
            }
        }
    
    // ----------------------------------


} // namespace ibisa











































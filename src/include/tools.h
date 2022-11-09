/*--------------------------------------------------
 * /IBISA/include/
 * tools.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class tools
 * TODO: 
 *
 */

#ifndef DEF_TOOLS
#define DEF_TOOLS

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // list of tools functions
    bool IsPathExist        (const std::string & path);    
    void saveCfgFile        (std::string & config_file_original, std::string & directory_of_save, std::string & name_save);
    void createFolder       (std::string & directory, std::string & ambition);

} // namespace ibisa

#endif















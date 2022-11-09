/*--------------------------------------------------
 * /IBISA/include/
 * configHelper.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class read
 * TODO: 
 *
 */


#include <include/configHelper.h>

namespace ibisa
{    
    // ----------------------------------
    // download_associated_CIMC 
    Configuration download_associated_CIMC 
        (Configuration & config)
        {
            std::string meteo         = std::string (config.get_string("data.meteo.directory")) + std::string (config.get_string("data.meteo.name"));
            if (config.get_bool ("chain.search.met") == true)
            {
                std::string member  = std::string ("/membre0")  + string("/");
                meteo               = meteo + member;
            }
            Configuration           metcon (meteo + std::string("/CIMC.cfg"));
            return metcon;

        }

} // namespace ibisa











































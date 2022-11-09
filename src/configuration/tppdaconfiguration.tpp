
/*--------------------------------------------------
 * /tppda/configuration/
 * tppdaconfiguration.tpp
 *--------------------------------------------------
 * author  : alban.farchi@enpc.fr
 * created : 2018/10/3
 *--------------------------------------------------
 *
 * Implementation of class TPPDAConfiguration
 * TODO: allow more flexible format string ?
 *
 */

#ifndef TPPDA_CONFIGURATION_TPPDACONFIGURATION_TPP
#define TPPDA_CONFIGURATION_TPPDACONFIGURATION_TPP

#include <tppda/configuration/tppdaconfiguration.h>

namespace tppda 
{

    //--------------------------------------------------
    // construction
    template <typename R, typename Int, typename Ind> 
        TPPDAConfiguration<R, Int, Ind>::
        TPPDAConfiguration(int argc, char * argv[])
        {
            for ( int arg(1) ; arg < argc ; ++arg )
            {
                std::ifstream file(argv[arg]) ;
                String line ;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
                while ( getline(file, line) )
#pragma GCC diagnostic pop
                {
                    if ( line.empty() )
                    {
                        continue ;
                    }

                    String::size_type ieq   = line.find(" = ") ;

                    String name             = line.substr(0, ieq) ;
                    String value            = line.substr(ieq+3) ;

                    m_values[name]          = value ;
                }
            }
        }

    // added by Joffrey
    template <typename R, typename Int, typename Ind>
        TPPDAConfiguration<R, Int, Ind>::
        TPPDAConfiguration(std::string dir)
    {
        std::ifstream file(dir);
        String line ;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
        while ( getline(file, line) )
#pragma GCC diagnostic pop
        {
            if ( line.empty() ) 
            {
                continue ;
            }

            String::size_type ieq = line.find(" = ");

            String name = line.substr(0, ieq);
            String value = line.substr(ieq+3);

            m_values[name] = value ;
        }
    }

    //--------------------------------------------------
    // access to bool
    template <typename R, typename Int, typename Ind> 
        bool
        TPPDAConfiguration<R, Int, Ind>::
        get_bool(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting bool        : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            std::string b(m_values.at(key)) ;
            return ( ( b == "1" ) or ( b == "yes" ) or ( b == "true" ) or ( b == "True" ) ) ;
        }

    //--------------------------------------------------
    // access to index
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::Index
        TPPDAConfiguration<R, Int, Ind>::
        get_index(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting index       : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            return static_cast<Index>(std::stoll(m_values.at(key))) ;
        }

    //--------------------------------------------------
    // access to index
    // special function for seeds
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::Index
        TPPDAConfiguration<R, Int, Ind>::
        get_seed(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting seed        : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            return static_cast<Index>(std::stoll(m_values.at(key))) ;
        }

    //--------------------------------------------------
    // access to integer
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::Integer
        TPPDAConfiguration<R, Int, Ind>::
        get_integer(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting integer     : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            return static_cast<Integer>(std::stoll(m_values.at(key))) ;
        }

    //--------------------------------------------------
    // access to real
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::Real
        TPPDAConfiguration<R, Int, Ind>::
        get_real(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting real        : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            return static_cast<Real>(std::stold(m_values.at(key))) ;
        }

    //--------------------------------------------------
    // access to string
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::String
        TPPDAConfiguration<R, Int, Ind>::
        get_string(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting string      : " << key << std::endl ;
#           endif // TPPDA_VERBOSE
            return m_values.at(key) ;
        }

    //--------------------------------------------------
    // access to stringlist
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::StringList
        TPPDAConfiguration<R, Int, Ind>::
        get_string_list(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting string list : " << key << std::endl ;
#           endif // TPPDA_VERBOSE

            StringList  string_list ;
            String      value = m_values.at(key) ;

            if ( value.size() == 0 )
            {
                return string_list ;
            }

            String::size_type istart = 0 ;
            String::size_type icomma = value.find(", ") ; 

            while ( icomma != String::npos )
            {
                string_list.push_back(value.substr(istart, icomma-istart)) ;
                istart = icomma + 2 ;
                icomma = value.find(", ", istart) ;
            }

            string_list.push_back(value.substr(istart)) ;
            return string_list ;
        }

    //--------------------------------------------------
    // access to indexlist
    template <typename R, typename Int, typename Ind> 
        typename TPPDAConfiguration<R, Int, Ind>::IndexList
        TPPDAConfiguration<R, Int, Ind>::
        get_index_list(const String & key) const
        {
#           ifdef TPPDA_VERBOSE
            std::cout << "    Requesting string list : " << key << std::endl ;
#           endif // TPPDA_VERBOSE

            IndexList  index_list ;

            // first get the list of strings
            StringList string_list(get_string_list(key)) ;

            // then convert to a list of Indices
            for ( StringList::iterator it(string_list.begin()) ; it != string_list.end() ; ++it )
            {
                index_list.push_back(static_cast<Index>(std::stoll(*it))) ;
            }

            return index_list ;
        }

    //--------------------------------------------------
    // free memory
    template <typename R, typename Int, typename Ind> 
        void
        TPPDAConfiguration<R, Int, Ind>::
        free_memory()
        {
            m_values.clear() ;
        }

} // namespace tppda

#endif // TPPDA_CONFIGURATION_TPPDACONFIGURATION_TPP


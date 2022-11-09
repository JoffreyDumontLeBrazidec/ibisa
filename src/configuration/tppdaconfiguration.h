
/*--------------------------------------------------
 * /tppda/configuration/
 * tppdaconfiguration.h
 *--------------------------------------------------
 * author  : alban.farchi@enpc.fr
 * created : 2018/10/3
 *--------------------------------------------------
 *
 * Definition of class TPPDAConfiguration
 *
 */

#ifndef TPPDA_CONFIGURATION_TPPDACONFIGURATION_H
#define TPPDA_CONFIGURATION_TPPDACONFIGURATION_H

#include <tppda/include/tools/tools.h>
#include <tppda/include/linalg/linalg.h>

namespace tppda 
{

    //--------------------------------------------------
    // class TPPDAConfiguration
    template <typename R = double, typename Int = int, typename Ind = int>
        class TPPDAConfiguration
        {

            public:
                //-------------------------
                // typedefs
                using Real          = R ;
                using Integer       = Int ;
                using Index         = Ind ;
                using IndexList     = std::list<Index> ;
                using String        = std::string ;
                using StringList    = std::list<String> ;
                using Values        = std::map<String, String> ;
                using Vector        = StandardVector<Real, Index> ;
                using IntVector     = StandardVector<Integer, Index> ;
                using IndVector     = StandardVector<Index, Index> ;
                using Matrix        = StandardMatrix<Real, Index> ;
                using IntMatrix     = StandardMatrix<Integer, Index> ;
                using IndMatrix     = StandardMatrix<Index, Index> ;
                using IFStream      = IFileStream<Index> ;
                using OFStream      = OFileStream<Index> ;

                //-------------------------
                // constructor
                TPPDAConfiguration(int argc, char * argv[]) ;

                // added by Joffrey
                TPPDAConfiguration(std::string dir);

                //-------------------------
                // methods
                bool
                    get_bool        (const String & key) const ;
                Index
                    get_index       (const String & key) const ;
                Index
                    get_seed        (const String & key) const ;
                Integer
                    get_integer     (const String & key) const ;
                Real
                    get_real        (const String & key) const ;
                String
                    get_string      (const String & key) const ;
                StringList
                    get_string_list (const String & key) const ;
                IndexList
                    get_index_list  (const String & key) const ;
                void 
                    free_memory     () ;

            private:
                //-------------------------
                // attributes
                Values m_values ;

        }; // class TPPDAConfiguration

} // namespace tppda

#endif // TPPDA_CONFIGURATION_TPPDACONFIGURATION_H


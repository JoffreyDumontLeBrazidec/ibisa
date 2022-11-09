/*--------------------------------------------------
 * /IBISA/include/
 * read.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class read
 * TODO: 
 *
 */


#include <include/read.h>

namespace ibisa
{

    // ----------------------------------
    // readf_inVectorXd
    Eigen::VectorXd readf_inVectorXd 
        (std::string & dir, int sizeVectorXd)
        {
            Eigen::VectorXd         y (sizeVectorXd);
            std::FILE* f            = std::fopen (dir.c_str(), "rb");
            std::fread              (&y[0], sizeof (double), sizeVectorXd, f);
            std::fclose                 (f);
            return y;
        }

    // ----------------------------------
    // readf_inVectorXi
    Eigen::VectorXi readf_inVectorXi
        (std::string & dir, int sizeVectorXi)
        {
            Eigen::VectorXd         vec_double = readf_inVectorXd (dir, sizeVectorXi);
            Eigen::VectorXi         vec_int (sizeVectorXi);
            for (int i=0; i < sizeVectorXi; i ++)
            {
                vec_int[i] = int (vec_double[i]+0.1);
            }
            return vec_int;
        }

    // ----------------------------------
    // readVectorXd_inMatrixXd - matrix filled in rows (row1 then row2 ...)
    Eigen::MatrixXd readVectorXd_inMatrixXd_fillInRow
        (Eigen::VectorXd & vec, int number_rows, int number_cols)
        {            
            Eigen::MatrixXd mat     = Eigen::MatrixXd::Zero (number_rows, number_cols);
            int k   = 0 ;
            for (int index_row = 0; index_row < number_rows ; index_row++)
            {
                for (int index_col = 0 ; index_col < number_cols ; index_col++)
                {   
                    k = index_col + index_row * number_cols;
                    mat (index_row, index_col) = vec[k] ;
                }
            }                        
            return mat;
        }
 
    // ----------------------------------
    // readVectorXd_inMatrixXi - matrix filled in rows (row1 then row2 ...)
    Eigen::MatrixXi readVectorXd_inMatrixXi_fillInRow
        (Eigen::VectorXd & vec, int number_rows, int number_cols)
        {            
            Eigen::MatrixXi mat     = Eigen::MatrixXi::Zero (number_rows, number_cols);
            int k = 0;
            for (int index_row = 0 ; index_row < number_rows ; index_row++)
            {
                for (int index_col = 0; index_col < number_cols; index_col++)
                {
                    k = index_col + index_row * number_cols;
                    mat (index_row, index_col) = int (vec[k]+0.1); // +0.1 used in case of integer (for example 1) saved as an under-approximation (for example 0.99999) 
                }
            }
            return mat;            
        }
   
    // ----------------------------------
    // readVectorXd_inMatrixXd - matrix filled in rows (row1 then row2 ...)
    Eigen::MatrixXd readf_inMatrixXd_fillInRow 
        (std::string dir, int number_rows, int number_cols)
        {
            Eigen::VectorXd vec = ibisa::readf_inVectorXd (dir, number_rows*number_cols);
            Eigen::MatrixXd mat = ibisa::readVectorXd_inMatrixXd_fillInRow (vec, number_rows, number_cols);
            return mat;
        }

    // ----------------------------------
    // readVectorXd_inMatrixXd - matrix filled in cols (col1 then col2 ...)
    Eigen::MatrixXd readf_inMatrixXd_fillInCol 
        (std::string dir, int number_rows, int number_cols)
        {
            Eigen::VectorXd vec         = ibisa::readf_inVectorXd (dir, number_rows*number_cols);
            Eigen::MatrixXd mat         = vec;
            mat.resize                  (number_rows, number_cols);
            return mat;
        }

    // ----------------------------------
    // readVectorXd_inMatrixXi - matrix filled in rows (row1 then row2 ...)
    Eigen::MatrixXi readf_inMatrixXi_fillInRow
        (std::string dir, int number_rows, int number_cols)
        {
            Eigen::VectorXd vec = ibisa::readf_inVectorXd (dir, number_rows*number_cols);
            Eigen::MatrixXi mat = ibisa::readVectorXd_inMatrixXi_fillInRow (vec, number_rows, number_cols);
            return mat;
        }
 
    // ----------------------------------
    // readVectorXd_inMatrixXi - matrix filled in cols (col1 then col2 ...)
    Eigen::MatrixXi readf_inMatrixXi_fillInCol
        (std::string dir, int number_rows, int number_cols)
        {
            Eigen::VectorXd vec         = ibisa::readf_inVectorXd (dir, number_rows*number_cols);
            Eigen::MatrixXd mat         = vec;
            mat.resize                  (number_rows, number_cols);
            Eigen::MatrixXi mat_integer (number_rows, number_cols);
            for (int index_row = 0 ; index_row < number_rows ; index_row++)
            {
                for (int index_col = 0; index_col < number_cols; index_col++)
                {
                    mat_integer (index_row, index_col) = int (mat (index_row, index_col)+0.1); // +0.1 used in case of integer (for example 1) saved as an under-approximation (for example 0.99999) 
                }
            }
            return mat_integer;
        }
   
    // ----------------------------------
    // readlistString_inVectorXd
    Eigen::VectorXd readlistString_inVectorXd
        (std::list<std::string> & listString, int & beg_read, int & size_VectorXd)
        {
            Eigen::VectorXd vec     = Eigen::VectorXd::Zero (size_VectorXd);
            int iterator_in_vec     = 0;

            for (std::list<std::string>::iterator iterator_in_listString = listString.begin(); iterator_in_listString != listString.end(); ++iterator_in_listString)
            {
                if ( (iterator_in_vec - beg_read >= 0) and (iterator_in_vec - beg_read < vec.size()) )
                {
                    vec [iterator_in_vec-beg_read] = std::stod (*iterator_in_listString);
                }
                iterator_in_vec = iterator_in_vec + 1;
            }
            return vec;
        }
    
    // ----------------------------------




} // namespace ibisa











































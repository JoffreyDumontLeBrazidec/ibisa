/*--------------------------------------------------
 * /IBISA/include/
 * read.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class read
 * TODO: 
 *
 */

#ifndef DEF_READ
#define DEF_READ

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // list of read functions
    

    Eigen::VectorXd readf_inVectorXd                    (std::string & dir, int sizeVectorXd); 
    Eigen::VectorXi readf_inVectorXi                    (std::string & dir, int sizeVectorXi);

    // to Eigen::MatrixXd
    Eigen::MatrixXd readVectorXd_inMatrixXd_fillInRow   (Eigen::VectorXd & vec, int number_rows, int number_cols);
    Eigen::MatrixXd readf_inMatrixXd_fillInRow          (std::string dir, int number_rows, int number_cols);
    Eigen::MatrixXd readf_inMatrixXd_fillInCol          (std::string dir, int number_rows, int number_cols);

    // to Eigen::MatrixXi
    Eigen::MatrixXi readVectorXd_inMatrixXi_fillInRow   (Eigen::VectorXd & vec, int number_rows, int number_cols);
    Eigen::MatrixXi readf_inMatrixXi_fillInRow          (std::string dir, int number_rows, int number_cols);
    Eigen::MatrixXi readf_inMatrixXi_fillInCol          (std::string dir, int number_rows, int number_cols);
    
    Eigen::VectorXd readlistString_inVectorXd           (std::list<std::string> & listString, int & beg_read, int & size_VectorXd);



} // namespace ibisa

#endif















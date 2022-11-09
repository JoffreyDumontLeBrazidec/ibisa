/*--------------------------------------------------
 * /IBISA/include/
 * LinearAction.h
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class LinearAction
 * TODO: 
 *
 */

#ifndef DEF_LinearAction
#define DEF_LinearAction

#include <main/basic_modules.h>

namespace ibisa
{
    //----------------------------------------
    // list of LinearAction functions
    int find_position_of_integer_in_vector_of_increasing_integers       
        (const int & integer, Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector);

    Eigen::VectorXi add_integer_into_vector_of_increasing_integers      
        (int & integer, Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector, const int & position_new_integer_in_vector);

    Eigen::VectorXi remove_integer_from_vector_of_increasing_integers   
        (Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector, const int & position_old_integer_in_vector);

    Eigen::MatrixXd remove_col_from_matrix_of_double                    
        (Eigen::MatrixXd & matrix, const int & matrix_number_cols, const int & position_col_to_remove);

    Eigen::MatrixXd add_col_to_matrix_of_double                         
        (Eigen::MatrixXd & matrix, const int & matrix_number_cols, const int & position_col_to_add);


} // namespace ibisa

#endif















/*--------------------------------------------------
 * /IBISA/include/
 * LinearAction.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.fr
 * created : 2020/08
 *--------------------------------------------------
 *
 * Implementation of class LinearAction
 * TODO: 
 *
 */


#include <include/LinearAction.h>

namespace ibisa
{
   
    // ----------------------------------
    // find_position_of_integer_in_vector_of_increasing_integers 
    // this vector is a vector [1 2 3 4 5 6 0 0 0 0 0 0 0 0 0 0 0 0 0] with therefore a real number of integers not corresponding to its size
    int find_position_of_integer_in_vector_of_increasing_integers
        (const int & integer, Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector)
        {
            int index_moving_in_vector = size_vector - 1;
            while (vector_of_increasing_integers [index_moving_in_vector] > integer)
            {
                index_moving_in_vector = index_moving_in_vector - 1;
            }
            index_moving_in_vector = index_moving_in_vector + 1;
            return index_moving_in_vector;
        }


    // ----------------------------------
    // add_integer_into_vector_of_increasing_integers; this vector is a vector [1 2 3 4 5 6 0 0 0 0 0 0 0 0 0 0 0 0 0]
    // position is given by position_new_integer_in_vector
    Eigen::VectorXi add_integer_into_vector_of_increasing_integers
        (int & integer, Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector, const int & position_new_integer_in_vector)
        {   
            for (int index_moving_in_vector = size_vector ; index_moving_in_vector > position_new_integer_in_vector ; index_moving_in_vector --)
            {
                vector_of_increasing_integers [index_moving_in_vector] = vector_of_increasing_integers [index_moving_in_vector-1];
            }
            vector_of_increasing_integers [position_new_integer_in_vector] = integer;
            return vector_of_increasing_integers;
        }
    
    // ----------------------------------
    // remove_integer_from_vector_of_increasing_integers; this vector is a vector [1 2 3 4 5 6 0 0 0 0 0 0 0 0 0 0 0 0 0]
    // position is given by position_old_integer_in_vector
    Eigen::VectorXi remove_integer_from_vector_of_increasing_integers
        (Eigen::VectorXi & vector_of_increasing_integers, const int & size_vector, const int & position_old_integer_in_vector)
        {  
            for (int index_moving_in_vector = position_old_integer_in_vector; index_moving_in_vector < size_vector-1; index_moving_in_vector ++)
            {
                vector_of_increasing_integers [index_moving_in_vector] = vector_of_increasing_integers [index_moving_in_vector+1];
            }
            vector_of_increasing_integers [size_vector-1] = 0;
            return vector_of_increasing_integers;
        }
    
    // ----------------------------------
    // remove_col_from_matrix_of_double = move to the left the part with double of the matrix
    // matrix has a specified number of cols non equal to its max number of cols 
    Eigen::MatrixXd remove_col_from_matrix_of_double
        (Eigen::MatrixXd & matrix, const int & matrix_number_cols, const int & position_col_to_remove)
        {  
            for (int index_moving_col = position_col_to_remove; index_moving_col < matrix_number_cols-1; index_moving_col ++)
            {
                matrix.col (index_moving_col) = matrix.col (index_moving_col+1);
            }
            return matrix;
        }
    
    // ----------------------------------
    // remove_col_from_matrix_of_double = move to the left the part with double of the matrix
    // matrix has a specified number of cols non equal to its max number of cols 
    Eigen::MatrixXd add_col_to_matrix_of_double
        (Eigen::MatrixXd & matrix, const int & matrix_number_cols, const int & position_col_to_add)
        {  
            for (int index_moving_col = matrix_number_cols; index_moving_col > position_col_to_add; index_moving_col --) 
            {
                matrix.col (index_moving_col) = matrix.col (index_moving_col - 1);
            }
            return matrix;
        }
    
    // ----------------------------------

} // namespace ibisa











































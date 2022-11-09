/*--------------------------------------------------
 * /IBISA/
 * cost.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class COST
 *
 */

#include "cost.h"

namespace ibisa 
{

    // -------------------------------------------------------------------------------------
    // Constructeur-Destructeur
    Cost::Cost 
        (Configuration & config, int & temperature)
        {

            m_temperature = temperature ; 

            //-----------------------------------
            // Initialisation of the observations
            cout << "Cost: Initialisation of the observations ... " << endl;
            
            Configuration metcon                = ibisa::download_associated_CIMC (config); 
            std::string dir_y                   = metcon.get_string ("meteo.observations.directory");
            m_number_obs                        = metcon.get_integer ("meteo.observations.number");
            int number_info_obs                 = 4;
            m_y                                 = ibisa::readf_inMatrixXd_fillInRow (dir_y, m_number_obs, number_info_obs);
            m_yS                                = Eigen::VectorXd::Zero (m_number_obs);
                       
            //----------------------------
            // Initialisation of the control variables
            cout << "Cost: Initialisation of the variables ... " << endl;

            std::tie (m_Index, m_Length)        = ibisa::indexation     (config);
            m_x1                                = config.get_real       ("init.long.position");
            m_x2                                = config.get_real       ("init.lag.position");
            
            m_current_number_imp                = config.get_integer    ("search.impulsions.number");
            m_beg_q                             = config.get_integer    ("search.impulsions.beg");
            if (m_Length[2]==0)
            {
                std::list<std::string> outputs      (config.get_string_list ("init.release.value"));
                m_log_q                             = ibisa::readlistString_inVectorXd  (outputs, m_beg_q, m_current_number_imp);           
            }
            if (m_Length[2]>0)
            {
                m_log_q                             = Eigen::VectorXd::Zero (m_Length[2]);
                m_log_qb                            = config.get_real       ("distribution.prior.log_release.value");
            }
            m_r                                 = Eigen::VectorXd::Constant     (config.get_integer ("search.observationerror.number"), config.get_real("init.observationerror.value")); // (size,value)
            m_meteo                             = Eigen::VectorXd::Zero         (config.get_integer ("init.number.meteo")); 
            m_meteo [0]                         = 1;
            m_b                                 = Eigen::VectorXd::Constant     (config.get_integer ("search.regprior.number"), config.get_real ("init.regprior.value")) ;
            std::string dir_labels_regprior     = config.get_string ("data.regprior.labels");
            int number_impulsions_horaires      = metcon.get_integer ("meteo.impulsions.number");
            m_labels_regPriorTerm               = ibisa::readf_inVectorXi (dir_labels_regprior, number_impulsions_horaires);

            m_k                                 = config.get_real ("init.regprior.secondParameter");
            m_scale_prior_on_borders            = config.get_real ("distribution.prior.grid_borders.value");
            m_trans                             = config.get_string ("release.transdimensionality");           

            
            //------------------------------
            // Initialisation of the distributions parameters
            cout << "Cost: init distributions ... " << endl;
            
            m_index_priors              = Eigen::VectorXi                   (config.get_integer ("variables.number.max"));
            m_index_priors [0]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.longitude.name"));
            m_index_priors [1]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.latitude.name"));
            m_index_priors [2]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.log_release.name"));
            m_index_priors [3]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.observationerror.name"));
            m_index_priors [4]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.metweight.name"));
            m_index_priors [5]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.regularisationterm.name"));
            m_index_priors [6]          = ibisa::distribution_indexation    (config.get_string ("distribution.prior.grid_borders.name"));
            m_likelihood                = ibisa::distribution_indexation    (config.get_string("distribution.likelihood"));
            m_yt                        = config.get_real                   ("distribution.threshold");
            m_Markov_Transitions        = ibisa::walk_conditions            (config); 
            
            //------------------------------
            // Initialisation of the error evaluation parameters
            cout << "Cost: init error evaluation parameters ... " << endl;

            m_reduction_number_obs                      = config.get_bool ("data.reduce.obs");
            int number_infos_labels                     = 3;
            std::string dir_labels                      = config.get_string ("data.obs.labels");            
            m_basic_index_observationerror              = ibisa::readf_inMatrixXi_fillInRow (dir_labels, m_number_obs, number_infos_labels);
            m_index_observationerror                    = m_basic_index_observationerror.col (2);
            m_decision_remove_obs                       = config.get_real ("data.reduce.obs.limit");
            m_size_indexing_observationerror            = Eigen::VectorXi::Zero (std::max (m_Length[3],1));
            for (int index_obs = 0; index_obs < m_number_obs; index_obs++)
            {
                m_size_indexing_observationerror [m_index_observationerror[index_obs]] =  m_size_indexing_observationerror [m_index_observationerror[index_obs]] + 1; 
            }
        }   

      
    Cost::~Cost
        (){}
      
    // -------------------------------------------------------------------------------------    
    // Methods
    // Compute the cost associated with the current parameters and the received variables
    double Cost::compute_cost 
        (const Eigen::VectorXd & X, ibisa::observationOperator & U)
        {
            update_variables                        (X);  
            compute_predictions                     (U, "false");
            double J_likelihood                     = ibisa::compute_likelihood (m_likelihood, m_yt, m_y, m_yS, m_r, m_index_observationerror, m_size_indexing_observationerror);            
            
            double J_prior                          = ibisa::compute_prior_on_log_release   (m_index_priors [2], m_current_number_imp, U.release_grid_borders (m_temperature), 
                                                                                                m_log_q, m_b, m_k, m_log_qb, m_Markov_Transitions (2,2), m_Markov_Transitions (2,3), 
                                                                                                m_labels_regPriorTerm, m_trans)
                                                    + ibisa::compute_prior_on_R             (m_index_priors [3], m_r, m_number_obs)
                                                    + ibisa::compute_prior_on_B             (m_index_priors [5], m_b);
            double J_model_prior                    = ibisa::compute_prior_on_grid_borders  (m_index_priors [6], U.number_borders (m_temperature), m_trans, m_scale_prior_on_borders);
            //cout << " J_likelihood " << J_likelihood << " J_prior " << J_prior << endl;
            //cout << " m_y.max() " << m_y.col(3).maxCoeff() << " m_yS.max() " << m_yS.maxCoeff() << " m_yt " << m_yt << endl;

            return J_likelihood + J_prior + J_model_prior;
        }

    // ----------------------------------------------------------
    // update_variables
    void Cost::update_variables 
        (const Eigen::VectorXd & X)
        {            
            if (m_Index[0]!=-1)
            {
                m_x1 = X [m_Index[0]];
                m_x2 = X [m_Index[1]];
            }

            if (m_Index[2]!=-1)
            {
                for (int impulsion = 0; impulsion < m_current_number_imp; impulsion++)
                {
                    m_log_q[impulsion] = X [m_Index[2]+impulsion];
                }
            }

            if (m_Index[3]!=-1)
            {
                for (int index_diag_coeff = 0; index_diag_coeff < m_Length[3]; index_diag_coeff++)
                {
                    m_r[index_diag_coeff] = X [m_Index[3]+index_diag_coeff];
                }
            }

            if (m_Index[4]!=-1)
            {
                for (int member = 0; member < m_Length[4]; member ++ )
                {
                    m_meteo[member] = X [m_Index[4]+member];
                }
            }

            if (m_Index[5]!=-1)
            {
                for (int index_b = 0; index_b < m_Length[5]; index_b ++)
                {
                    m_b[index_b] = X [m_Index[5]+index_b];
                }
            }         
        }

    // ------------------------------------- 
    // Compute predictions at four corners, then do bilinear interpolation to compute real predictions
    void Cost::compute_predictions 
        (const ibisa::observationOperator & U, const bool & update_all)
        {
            Eigen::VectorXd     yS = Eigen::VectorXd::Zero (m_number_obs);
            Eigen::Vector2d     lx1, lx2;
            double              w;
            
            // all meteos are supposed to have same config (here, step = pas)
            double  pas_met     = U.get_pas_met();
            lx1[0]              = std::floor (m_x1 / pas_met) * pas_met; 
            lx1[1]              = std::floor (m_x1 / pas_met) * pas_met + pas_met;
            lx2[0]              = std::floor (m_x2 / pas_met) * pas_met; 
            lx2[1]              = std::floor (m_x2 / pas_met) * pas_met + pas_met; 

            // Loop on the different meteorologies
            // ------------------------------------

            Eigen::MatrixXd unitpred;
            Eigen::MatrixXd current_unitpred;
            for (int k = 0 ; k < 2 ; k++)
            {
                for (int l = 0 ; l < 2 ; l++)
                {
                    w   = (1 - std::abs (lx1[k]-m_x1) / (lx1[1]-lx1[0])) * (1 - std::abs (lx2[l]-m_x2) / (lx2[1]-lx2[0]));
                    if (w>0)
                    {
                        int m = 0;
                        unitpred  = m_meteo[m] * U.get_source (lx1[k], lx2[l], m, m_temperature);
                        for (m = 1; m < std::max (1, m_Length[4]) ; m++)
                        {
                            current_unitpred    = U.get_source (lx1[k], lx2[l], m, m_temperature);  
                            unitpred            = unitpred + m_meteo[m] * current_unitpred;
                        }
                        unitpred = unitpred * 1e-12;

                        # pragma omp parallel for
                        for (int i = 0 ; i < m_number_obs ; i++)
                        {
                            if (update_all == true or m_index_observationerror[i] != m_Length[3] - 1 or m_reduction_number_obs == false)
                            {
                                for (int j = m_beg_q ; j < m_beg_q + m_current_number_imp ; j++)
                                {
                                    int jq      = j - m_beg_q;
                                    yS[i]       = yS[i] + w * unitpred(i,j) * std::exp(m_log_q[jq]);                                    
                                }
                            }
                        }
                    }
                }

            }
            m_yS = yS * 1e-3;  

            // Ru case: predictions are computed in microBq/m3 and are returned to be compared with observations in miliBq/m3
            // attention : il y a une division par 1000 qui était nécessaire dans le cas du Ru. J'ai pris l'habitude du coup de multiplier par 1000
            // mes opérateurs d'observation avant de les utiliser (pour que ce soit équilibré ici)
            // c'est sûrement quelque chose dont on peut se passer, la ligne du dessus est à remplacer par m_yS = yS         
        }

    // -------------------------------------
    // update indexing observationerror
    void Cost::update_indexing_observationerror 
        (const Eigen::VectorXi & index_observationerror, const Eigen::VectorXi & size_indexing_observationerror)
        {
            m_index_observationerror            = index_observationerror ;
            m_size_indexing_observationerror    = size_indexing_observationerror;
        }

    // -------------------------------------
    // update current_number_impulsions
    void Cost::update_number_impusions 
        (const int & current_number_impulsions)
        {
            m_current_number_imp = current_number_impulsions;
        }

    //-------------------------------------- 
    // get predictions
    Eigen::VectorXd Cost::get_predictions 
        ()
        {
            return m_yS;
        }

    // -------------------------------------

} //namespace ibisa


























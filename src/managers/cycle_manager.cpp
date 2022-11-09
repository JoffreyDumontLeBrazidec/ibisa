/*--------------------------------------------------
 * /IBISA/managers/
 * cycle_manager.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class cycle_manager
 * TODO: 
 *
 */

#include <managers/cycle_manager.h>

namespace ibisa
{

    // Constructeur-Destructeur
        Cycle_Manager::Cycle_Manager
            (Configuration & config, const Eigen::Vector2i & seed, Eigen::VectorXd & temperatures)
            : m_rng (seed[0], 0, 1), m_ung (seed[1])
            {
                m_order_print                   = 1;
                m_step                          = std::chrono::high_resolution_clock::now();
                m_last_step                     = std::chrono::high_resolution_clock::now(); 

                m_modprint                      = config.get_integer ("control.modprint"); 
                m_uniformisation                = config.get_integer ("control.uniformisation");
                m_mod_enhance_number_obs        = config.get_integer ("data.enhance.obs");
                m_number_temperatures           = config.get_integer ("init.temperature.number");
                m_burn_in                       = config.get_integer ("control.burn_in");
                m_reduction_number_obs          = config.get_bool    ("data.reduce.obs");
                m_tri_obs                       = config.get_bool    ("data.tri.general.obs");

                
                m_temperatures                  = temperatures;                
                m_number_temperatures           = m_temperatures.size();
                m_inverse_temperatures.resize   (m_number_temperatures);
                
                for (int index_temp(0) ; index_temp < m_number_temperatures ; index_temp++)
                {
                    m_inverse_temperatures [index_temp] = 1 / m_temperatures [index_temp] ;
                }
                
                m_possible_walks    = {"mono-dim", "mono-dim", "mono-dim"};
                m_trans             = config.get_string ("release.transdimensionality");

                if (m_trans == "move")
                {                                                            
                    m_possible_walks.push_back ("move");
                }                                    
                
                if (m_trans == "full")                                                    
                {                                                                        
                    m_possible_walks.push_back ("move");
                    m_possible_walks.push_back ("birth");
                    m_possible_walks.push_back ("death");
                }
                
                if (m_tri_obs == true)
                {
                    m_possible_walks.push_back ("hyper");
                }

                m_update_acceptance_ratio = std::max (m_modprint, 100);

            }

        Cycle_Manager::~Cycle_Manager
            ()
            {}

        // methodes
        // --------

        // ----------------------
        // modular_print
        void Cycle_Manager::modular_print 
            (const int & cycle_num, ibisa::MarkovState & X, ibisa::observationOperator & U, 
             const Eigen::MatrixXd & last_within_acceptance_ratio, const Eigen::MatrixXd & last_swap_acceptance_ratio)
            {
                if (cycle_num % m_modprint == 0)
                {
                    m_step          = std::chrono::high_resolution_clock::now ();
                    m_fp_ms         = m_step - m_last_step;
                    m_last_step     = std::chrono::high_resolution_clock::now ();

                    cout    << endl 
                            << "Cycle number = "                << cycle_num 
                            << endl 
                            << "Time for this cycle is: "       << m_fp_ms.count() / m_modprint 
                            << endl
                            << " Acceptance ratio on the " << m_update_acceptance_ratio  << " last hits is: " << last_within_acceptance_ratio.col(0).mean()
                            << " move : "  << last_within_acceptance_ratio.col(1).mean()
                            << " birth : " << last_within_acceptance_ratio.col(2).mean()
                            << " death : " << last_within_acceptance_ratio.col(3).mean()
                            << " hyper : " << last_within_acceptance_ratio.col(4).mean()
                            << " and swap acceptance ratios are : ";
                    for (int temp = 0 ; temp < m_number_temperatures - 1 ; temp ++)
                    {
                        cout << last_swap_acceptance_ratio.col(temp).mean() << "  ";
                    }
                    X.print (m_order_print, U); 
                }
            }
    
        
        // ----------------------------
        // reduce_or_enhance_set_obs
        void Cycle_Manager::reduce_or_enhance_set_obs
            (const int & cycle_num, ibisa::MarkovState & X, ibisa::StandardState & sX, ibisa::observationOperator & U)
            {
                if (m_reduction_number_obs == true)
                {                    
                    // number obs. reduction
                    if (cycle_num == m_burn_in)
                    {
                        X.order_reduce_set_obs      ();                        
                        sX.set_cost_high            ();
                    }
 
                    // number obs. enhancement
                    if (cycle_num > m_burn_in and cycle_num % m_mod_enhance_number_obs == 0) 
                    {
                        bool indicator_model_change = X.order_enhance_set_obs (U);
                        if (indicator_model_change == true)
                        {                        
                            sX.set_cost_high    (); 
                        }
                    }
                    Eigen::VectorXi index_observationerror          = X.get_index_observationerror();
                    Eigen::VectorXi size_indexing_observationerror  = X.get_size_indexing_observationerror();
                    sX.copy_indexing_obs (index_observationerror, size_indexing_observationerror);
                }
            }
        
        // ----------------------------
        // update_variances
        void Cycle_Manager::update_variances
            (const int & cycle_num, double & mean_within_acceptance_ratios, ibisa::MarkovState & Xp)
            {
                int beg_update = 0;
                bool activate_update_variance = false;
                if (cycle_num % 100 == 0 and cycle_num > beg_update and activate_update_variance == true)
                {
                    Xp.update_adaptively_variances (mean_within_acceptance_ratios);
                }
            }

        // ----------------------------
        // Walking step ; at this point, X and sX have the same state
        int Cycle_Manager::walk 
            (const int & cycle_num, ibisa::MarkovState & X, ibisa::observationOperator & U) 
            {                
                m_index_walk_choice                 = std::floor (m_ung.rand (m_possible_walks.size()));

                // Before burn-in : only mono-dim walks are accepted
                if (cycle_num < m_burn_in)
                {
                    m_index_walk_choice = 0; 
                }
                X.walk                             (m_possible_walks [m_index_walk_choice], U);
                X.compute_cost                     (U);
                
                int col_within_acceptance_ratio;
                if (m_possible_walks [m_index_walk_choice] == "mono")
                    col_within_acceptance_ratio = 0;
                if (m_possible_walks [m_index_walk_choice] == "move")
                    col_within_acceptance_ratio = 1;
                if (m_possible_walks [m_index_walk_choice] == "birth")
                    col_within_acceptance_ratio = 2;
                if (m_possible_walks [m_index_walk_choice] == "death")
                    col_within_acceptance_ratio = 3;
                if (m_possible_walks [m_index_walk_choice] == "hyper")
                    col_within_acceptance_ratio = 4;
                return col_within_acceptance_ratio;
            }
        
        // ----------------------------
        // metropolis hastings step
        Eigen::VectorXd Cycle_Manager::metroplis_hastings_step                   
            (ibisa::MarkovState & X, ibisa::StandardState & sX, ibisa::observationOperator & U)            
            {
                Eigen::VectorXd u                           (m_number_temperatures);
                Eigen::VectorXd log_alpha                   (m_number_temperatures);
                Eigen::VectorXd within_acceptance_ratio     = Eigen::VectorXd::Zero (m_number_temperatures); 

                if (m_possible_walks [m_index_walk_choice] != "hyper")
                {
                    # pragma omp parallel for
                    for (int t = 0; t < m_number_temperatures; t ++)
                    {               
                        u[t]            = m_ung.rand();
                        log_alpha[t]    = X.get_cost_t (t) + X.get_cost_transition_t (t) - sX.get_cost_t (t);
                    
                        if (m_inverse_temperatures[t] * (- log_alpha[t]) > std::log(u[t]))
                        {   
                            sX.copy_t                   (t, X.get_state_t (t), X.get_cost_t (t), X.get_current_number_impulsions_t (t));
                            within_acceptance_ratio[t] = 1;          
                            if (m_trans != "false")     
                            {                                                             
                                U.update_base_or_proposal (t, within_acceptance_ratio[t]);
                            }
                        }                 
                        else                    
                        {       
                            X.copy_t                    (sX, t);
                            within_acceptance_ratio[t]  = 0;
                            if (m_trans != "false")  
                            {                     
                                U.update_base_or_proposal (t, within_acceptance_ratio[t]);  
                            }                                                                                                                                                                                                                    
                        }
                    }
                }
                
                if (m_possible_walks [m_index_walk_choice] == "hyper")
                {                       
                    u[0]            = m_ung.rand();
                    log_alpha[0]    = X.get_cost_t (0) + X.get_cost_transition_t (0) - sX.get_cost_t (0);
                    if (- log_alpha[0] > std::log(u[0]))
                    {   
                        Eigen::VectorXi index_observationerror          = X.get_index_observationerror();
                        Eigen::VectorXi size_indexing_observationerror  = X.get_size_indexing_observationerror();
                        sX.copy_indexing_obs                            (index_observationerror, size_indexing_observationerror);
                        within_acceptance_ratio[0]                      = 1;
                    }                 
                    else                    
                    {       
                        Eigen::VectorXi index_observationerror          = sX.get_index_observationerror();
                        Eigen::VectorXi size_indexing_observationerror  = sX.get_size_indexing_observationerror();                       
                        X.copy_indexing_obs                             (index_observationerror, size_indexing_observationerror);
                        within_acceptance_ratio[0]                      = 0;
                    }
                }
                
                return within_acceptance_ratio;
            }
        
        // ----------------------------
        // Swapping Temperatures step 
        Eigen::VectorXd Cycle_Manager::swap_temperatures_step 
            (ibisa::MarkovState & X, Eigen::VectorXd & swap_acceptance_ratio, ibisa::observationOperator & U)
            {
                Eigen::VectorXd u (m_number_temperatures);
                for (int t = m_number_temperatures - 1 ; t > 0 ; t --)
                {
                    u[t] = m_ung.rand();
                    if (std::log(u[t]) < (m_inverse_temperatures[t-1] - m_inverse_temperatures[t]) * (X.get_cost_t (t-1) - X.get_cost_t (t)))
                    {
                        X.swap_states               (t-1,t);
                        swap_acceptance_ratio[t-1]  = 1;
                        if (m_trans != "false")
                        {
                            U.swap_R_temperatures (t-1, t);
                        }
                    }
                    else
                    {
                        swap_acceptance_ratio[t-1] = 0;
                    }
                }
                return swap_acceptance_ratio;
            }
        
        // ----------------------------
        // uniformise
        void Cycle_Manager::uniformise
            (ibisa::MarkovState & X, ibisa::StandardState & sX, int & cycle_num, ibisa::observationOperator & U)
            {
                X.uniformise_observationerror();
                X.uniformise_regprior();
                
            // due to r variance uniformisation : need to recompute the cost which does not correspond anymore to the source variables x 
            if (cycle_num % m_uniformisation == 0)
                {
                    X.compute_cost  (U);
                }
                sX.copy (X.get_state (), X.get_cost (), X.get_current_number_impulsions ()) ;
            }
        
        // ----------------------------
        // update_MS_cycle_number
        void Cycle_Manager::update_MS_cycle_number 
            (ibisa::MarkovState & X, int & cycle_num)
            {
                X.update_cycle_number(cycle_num);
            }
        
        // ----------------------------       

} // namespace ibisa











































/*--------------------------------------------------
 * /IBISA/state/
 * initiate_state.cpp
 *--------------------------------------------------
 * author  : joffreydumont@hotmail.Fr
 * created : 2019/04
 *--------------------------------------------------
 *
 * Implementation of class initiate_state
 * TODO: 
 *
 */


#include <state/initiate_state.h>


namespace ibisa
{

    Eigen::MatrixXd 
        initiate_state 
        (Configuration & config, const Eigen::VectorXi & Index, const Eigen::VectorXi & Length, const Eigen::VectorXd & temperatures, const Eigen::MatrixXd & Markov_Transitions,
                        const Eigen::Vector2i & seed) 
        {
 
            int number_temperatures     = temperatures.size(); 
            Eigen::MatrixXd m_state     = Eigen::MatrixXd::Zero (number_temperatures, Length.sum());
            tppda::GRNG<double,int>     rng (seed[0], 0, 1);
            tppda::URNG<double,int>     ung (seed[1]);


            // Longitude and latitude
            // -----------------------------------------------------

            if (config.get_bool("chain.search.location") == true)
            {
                cout << "MarkovState::Init location variable ... " << endl;
                if (config.get_string ("init.location.choice") == "set")
                {
                    for (int t = 0; t < number_temperatures; t++)
                    {
                        m_state (t, Index[0]) = config.get_real ("init.long.position");
                        m_state (t, Index[1]) = config.get_real ("init.lag.position");
                    }
                }

                if (config.get_string("init.location.choice") == "random")
                {
                    for (int t = 0 ; t < number_temperatures ; t ++)
                    {
                        m_state (t, Index[0]) = Markov_Transitions (0,2) + ung.rand (1) * ( Markov_Transitions (0,3) - Markov_Transitions (0,2) ) ;
                        m_state (t, Index[1]) = Markov_Transitions (1,2) + ung.rand (1) * ( Markov_Transitions (1,3) - Markov_Transitions (1,2) ) ;
                    }
                }
            }
            
            // Log Releases rates
            // -----------------------------------------------------

            if (config.get_bool("chain.search.release") == true)
            {
                cout << "MarkovState::Init release variable ... " << endl;

                if (config.get_string ("init.release.choice") == "set")
                {
                    std::list<std::string> outputs (config.get_string_list ("init.release.value"));
                    for (int t = 0 ; t < number_temperatures ; t ++)
                    {
                        int ba = 0;
                        for (std::list<std::string>::iterator it = outputs.begin(); it != outputs.end(); ++it)
                        {
                            m_state (t, Index[2]+ba)    = std::stod (*it);
                            ba                          = ba + 1 ;
                        }
                    }
                }
                
                if (config.get_string("init.release.choice") == "random")
                {
                    for (int t = 0 ; t < number_temperatures ; t ++)
                    {
                        for (int i = 0 ; i < Length[2] ; i++)
                        {
                            //m_state ( t , Index[2] + i ) = Markov_Transitions(2,2)  + ung.rand(1) * ( Markov_Transitions(2,3)  - Markov_Transitions(2,2)  ) ;
                            m_state (t, Index[2] + i) = Markov_Transitions(2,2);
                        }
                    }
                }
            }

            // Error Covariance matrix diagonal coefficient 
            // ------------------------------------------------------------------

            if (config.get_bool ("chain.search.observationerror")==true)
            {

                cout << "MarkovState::Init observation error variance term ... " << endl;

                if (config.get_string ("init.observationerror.choice") == "set")
                {
                    for (int i = 0 ; i < Length[3] ; i++)
                    {
                        for (int t = 0 ; t < number_temperatures ; t++)
                        {
                            m_state (t, Index[3]+i) = config.get_real ("init.observationerror.value");
                        }                                                                                
                    }                                                                                                                               
                }
                
                if (config.get_string("init.observationerror.choice") == "random")                
                {                                                              
                    for (int i = 0 ; i < Length[3] ; i++)
                    {                                  
                        for (int t = 0 ; t < number_temperatures ; t ++)                 
                        {
                            m_state (t, Index[3]+i) = Markov_Transitions(3,2) + ung.rand(1) * (Markov_Transitions(3,3) - Markov_Transitions(3,2)) ;
                        }                                                                                                                                                                
                    }                     
                }         
            }

            // Meteo
            // The whole part here should only be used in the case of several meteorologies
            // --------------------------------------------------------------------------

            bool interpolation_meteo = config.get_bool("init.meteo.interpolation");
            if (config.get_bool("chain.search.met") == true)
            {        
                cout << "MarkovState::Init meteo variable ... " << endl;            

                if (config.get_string ("init.meteo.choice") == "set")
                {                                                                  
                    std::list<std::string> outputs_meteo(config.get_string_list("init.meteo.value"));                                                                                                 
                    for (int t = 0 ; t < number_temperatures ; t ++)                                                                                                 
                    {                                                                  
                        int ba = 0;                 
                        for (std::list<std::string>::iterator it = outputs_meteo.begin() ; it != outputs_meteo.end() ; ++it)
                        {                                                                                                                          
                            m_state (t, Index[4]+ba) = std::stod (*it);                     
                            ba = ba + 1 ;                                                                            
                        }                                                                                                                                                                                                        
                        double sum_weight = m_state.block (t, Index[4], 1, Length[4]).sum();
                        for (int i = 0 ; i < Length[4] ; i ++)
                        {                                                              
                            m_state (t , Index[4] + i) = m_state (t , Index[4] + i) / sum_weight;                     
                        }                                                                                                 
                    }
                }
                if (config.get_string ("init.meteo.choice") == "random")
                {            
                    double wmet_tot;                                                         
                    for (int t = 0 ; t < number_temperatures ; t ++)
                    {
                        wmet_tot = 0;                 
                        for (int i = 0 ; i < Length[4] ; i++)
                        {
                            m_state (t, Index[4] + i)   = Markov_Transitions(4,2) + ung.rand(1) * (Markov_Transitions(4,3) - Markov_Transitions(4,2)) ;
                            wmet_tot                    = wmet_tot + m_state (t, Index[4] + i) ;
                        }

                        // all weights are initialised randomly                     
                        if (interpolation_meteo == true)
                        {                                                        
                            for (int i = 0 ; i < Length[4] ; i ++)                     
                            {                                                                 
                                m_state (t, Index[4]+i) = m_state (t , Index[4]+i) / wmet_tot ;                         
                            }
                        }                                                                   
                        // false "random" initialisation but it really does not matter where it is initialised in this case                                    
                        if (interpolation_meteo == false)
                        {
                            for (int i = 0 ; i < Length[4] ; i ++)
                            {                        
                                m_state (t, Index[4]+i) = 0;                         
                            }                                                               
                            m_state (t, Index[4]) = 1 ;                         
                        }                     
                    }
                }
            }
 
            // Regularisation covariance matrix diagonal coefficient
            // ------------------------------------------------------------------

            if (config.get_bool ("chain.search.regprior") == true)
            {

                cout << "MarkovState::Init Regularisation prior term ... " << endl;

                if (config.get_string ("init.regprior.choice") == "set")
                {
                    for (int i = 0 ; i < Length[5] ; i++)
                    {
                        for (int t = 0 ; t < number_temperatures ; t++)
                        {
                            m_state (t, Index[5]+i) = config.get_real ("init.regprior.value");
                        }                                                                                
                    }                                                                                                                               
                }
                
                if (config.get_string ("init.regprior.choice") == "random")
                {                                                              
                    for (int i = 0 ; i < Length[5] ; i++)
                    {                                  
                        for (int t = 0 ; t < number_temperatures ; t ++)                 
                        {
                            m_state (t , Index[5]+i) = Markov_Transitions(5,2) + ung.rand(1) * (Markov_Transitions(5,3) - Markov_Transitions(5,2)) ;
                        }                                                                                                                                                                
                    }                     
                }         
            }
           
            // -----------------

            return m_state;
        }


} // namespace ibisa











































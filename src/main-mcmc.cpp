/*-------------------------------------------------
* /IBISA/
*--------------------------------------------------
* author  : joffreydumont@hotmail.fr
* created : 2019/04
*--------------------------------------------------
*
* Implementation of LANCEUR
* TODO: 
*
*/


#include <main/basic_modules.h>
#include "main/paralleltempering.h"

int main (int argc, char *argv[])
{
    (void)argc;
    
    ibisa::PTalgo   PT (argv[1]);
    PT.run          ();   
}











/*
 *   Spread of parapoxvirus
 *  
 *  Red squirrels are in decline in the UK and as such there is a considerable 
 *  effort to save them. However, a disease called parapoxvirus is especially 
 *  perilous to them. A group of biologists are investigating the spread of 
 *  the disease through the squirrel population.
 *
 *  Based on the model given by the biologists, the current programme was
 *  created to simulate the red squirrel's environment. The programme has been 
 *  parallelised using the actor pattern.	
 *
 *  To compile the code the Makefile provided should be used
 *  To run the program, after compiling (make) you should run the command make run
 *  To run the program in a back end of a server a sample .pbs file is provided
 *  
 * ----------------------------------------------------------------------------------
 * 
 *  Project : 	Spread of parapoxvirus
 *                        
 *  Programming Language : C
 *
 *  Parallel design pattern : Actor pattern
 *                           
 *  Date : 06.04.2018
 *
 *  Author: B116437
 *
 * ----------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <mpi.h>
#include <math.h>

#include "squirrel-functions.h"
#include "pool.h"
#include "actor_struct.h"
#include "actor_Setup.h"
#include "entities.h"
#include "work.h"

int main(int argc, char **argv)
{
	
	// Create the actor framework
	// and send the initialisation and work functions of the 
	// model. Work function is the function that 
	// is going to be implemented by the actors.
	
	// Squirrel model
	actor_pool(argc, argv, squirrelModel_initialise, squirrelModel_work, 30);

	return 0;
}

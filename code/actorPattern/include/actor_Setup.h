/*
 *  These functions are used to set up the pool and initialise the 
 *  framework 
 *  ----------------------------------------------------------------------------------
 */



#ifndef ACTOR_SETUP_H
#define ACTOR_SETUP_H

#include <stdio.h>
#include <mpi.h>

#include "pool.h"
#include "actor_Work.h"
#include "actor_struct.h"

// Initialise the MPI environment
void actor_mpiInit(int argc, 
		   char **argv, 
		   int *myRank, 
		   MPI_Comm *comm, 
		   int *size);

// Create the pool, invoke initialise and work 
// functions of the actors
void actor_pool(int argc, char **argv, 
		void* (*initialise)(void (*a)),
		void (*work)(void (*b), void (*c)),
		int initialActorsNum);


#endif // ACTOR_SETUP_H

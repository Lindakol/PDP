/*
 * This is the function that every actor executes. 
 * It invokes the right work and initialise function 
 * depending on the model.
 *
 * ----------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <mpi.h>

#include "pool.h"
#include "actor_struct.h"

void actorCode(void* (*initialise)(void (* a)), 
	       void (*work)(void (*b), void (*c)), 
	       MPI_Comm comm);

/*
 *  This struct is the struct used by the framework
 *  Every data structure that will be used to implement 
 *  actors should inherit this struct. It holds information 
 *  about the process' rank, parent and children
 *  -----------------------------------------------------------------
 * 
 */


#ifndef ACTOR_STRUCT_H
#define ACTOR_STRUCT_H

#include <mpi.h>
#include "defs.h"

typedef struct actor_struct actor_struct;

struct actor_struct{
	// Process rank
	int myRank;
	// Actor id
	int actorId;
	// Parent process
	int parentId;
	// Children process(es)
	int childrenNumber;
	MPI_Request *childrenRequests;
	int *childrenId;
};

// This function returns the rank of the actor
int actor_getRank(actor_struct *ac);	

// Initialise the actor
void actor_Init(actor_struct*, int, int, int);

#endif //ACTOR_STRUCT_H

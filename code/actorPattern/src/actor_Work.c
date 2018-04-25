#include <stdio.h>
#include <malloc.h>
#include "actor_Work.h"
#include "actor_struct.h"
#include "entities.h"

void actorCode(void* (*initialise)(void (*a)), void (*work)(void (*b), void (*c)), MPI_Comm comm)
{	
	actor_struct *actor = malloc(sizeof(actor_struct));
	int actorStatus = 1, ierr;
	void * s;
	while (actorStatus){
		int myRank, parentId;
		
		// Actor's rank
		MPI_Comm_rank(comm, &myRank);
		// Actor's parent
		parentId = getCommandData();
		
		// Initialise actor
		actor_Init(actor, myRank, parentId, myRank);
		
		// Call the initialise function for the actor
		// It is implemented by the model
		s = initialise(actor);

		// Call the work function. This is implemented
		// by the specific model
		work(s, s);
		
		// If I had children wait for them to die before I die
		actor_struct *a = malloc(sizeof(actor_struct));
		a = (actor_struct *)s;
		
		int die = shouldWorkerStop();

		if (die == 0){
			if (a->parentId == 0){
				// I am done and about to die.	
				ierr = MPI_Ssend(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
				if (ierr != MPI_SUCCESS)
					printf("INITIAL ACTOR: I couldnt send to MASTER\n");
			}
		}	
		// I am a children
		if (a->parentId != 0){
			ierr = MPI_Ssend(NULL, 0, MPI_INT, a->parentId, 0, MPI_COMM_WORLD);
			if (ierr != MPI_SUCCESS)
				printf("CHILD_ACTOR: I couldnt send to MY PARENT\n");


		}
		actorStatus = workerSleep();
	}
}


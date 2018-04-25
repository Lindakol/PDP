#include <mpi.h>
#include "actor_Setup.h"


void actor_mpiInit(int argc, 
		   char **argv, 
		   int *myRank, 
		   MPI_Comm *comm, 
		   int *size)
{
	
	MPI_Init(&argc, &argv); 
        *comm = MPI_COMM_WORLD;
      	// Return info about errors
	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);   
	// My rank
	MPI_Comm_rank(*comm, myRank);
	// Number of processes in the communicator
        MPI_Comm_size(*comm, size);
}


// Actor pattern framework set up
void actor_pool(int argc, 
		char **argv,
		void* (*initialise)(void (*a)),
		void (*work)(void (*b), void(*s)), 
		int initialNumActors)
{
	
	int size, myRank;
	MPI_Comm comm;

	// Initialise MPI environment
	actor_mpiInit(argc, argv, &myRank, &comm, &size);	

	// Initialise the pool of actors
	int statusCode = processPoolInit();
	
	// I am a plain actor
	if (statusCode == 1){
		actorCode(initialise, work, comm);
	}
	// Master
	// Creates the initial generation of actors
	else if (statusCode == 2){
		int i, activeActors = 0, actorsMaxNum = size, returnCode=0;
		int initialActorMessage[initialNumActors];
		MPI_Request initialActorsRequests[initialNumActors];
		MPI_Status initialActorsStatus[initialNumActors];
		// for all the processes available -1 (except for master)
		for (i = 0; i < initialNumActors-1; i++){
			// Start the actor and give an ID
			int actorPid = startWorkerProcess();
			MPI_Irecv(NULL, 
				  0, 
				  MPI_INT, 
				  actorPid, 
				  0, 
				  MPI_COMM_WORLD, 
				  &initialActorsRequests[i]);
			activeActors++;
			actorsMaxNum--;
		}
		int message[2], flag = 0;
		MPI_Request req;
		MPI_Status st;
		MPI_Irecv(message, 2, MPI_INT, MPI_ANY_SOURCE, ACTOR_TAG, MPI_COMM_WORLD, &req);	

		// The loop that master holds to keep the actors
		int masterStatus = masterPoll();
		while (masterStatus){ 	 
			masterStatus = masterPoll();
			if (masterStatus == 0){
				for (i = 0; i < initialNumActors-1; i++)
					MPI_Cancel(&initialActorsRequests[i]);
				MPI_Cancel(&req);
				break;}

			for (i = 0; i < initialNumActors-1; i++){
				// Check to see if the actors are done
				if (initialActorsRequests[i] != MPI_REQUEST_NULL){
					MPI_Test(&initialActorsRequests[i], 
						 &returnCode, 
						 MPI_STATUS_IGNORE);
					if (returnCode==1){
						 activeActors--;
						 actorsMaxNum++;
					}
				}
			}
			// TEST THAT SOMEONE NEEDS ACTORS OR THAT SOMEONE KNOWS THAT ACTORS DIED
			MPI_Test(&req, &flag, &st);
			if (flag == 1){
				MPI_Request reqSend;
				flag = 0;
				if (message[0] == CHILD){
					int temp_message = 0;
					if (actorsMaxNum > 0){
						actorsMaxNum--;
						temp_message = 1;
					}
					MPI_Isend(&temp_message, 1, MPI_INT, message[1], MASTER_TAG, MPI_COMM_WORLD, &reqSend);
				}
				else if (message[0] == DEAD_ACTOR){
					actorsMaxNum += message[1];
				}
				MPI_Wait(&reqSend, &st);
				MPI_Irecv(message, 2, MPI_INT, MPI_ANY_SOURCE, ACTOR_TAG, MPI_COMM_WORLD, &req);
				
			}

			// No more active actors. Leave the loop
			if (activeActors == 0){
				MPI_Cancel(&req);
				break;			
			}

		} 
	}


	
	// Terminate the pool
	processPoolFinalise();

	// Finalise MPI
	MPI_Finalize();

}

int actor_Check(){
	return  shouldWorkerStop();
}

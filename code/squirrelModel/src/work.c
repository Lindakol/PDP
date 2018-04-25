#include "work.h"
// All the defined numbers
#include "defs.h"
//#include <mpi.h>
#include <malloc.h>



void *squirrelModel_initialise(void (* a))
{

	void *b;
	// Dereference as an actor
	actor_struct actor = *((actor_struct *)a);
	
	int myRank;
	myRank = actor.myRank;	

	// Time keeper
	if (myRank == 1){
		b = malloc(sizeof(timeKeeper));
		timeKeeperInit(&actor, b);
	}
	// Land cell
	else if (myRank <= 17){
		b = malloc(sizeof(landCell));
		// Return as type land cell
		landCellInit(&actor, b);
	}
	else if (myRank == 18){
	}
	// Squirrels
	else if ((myRank < 30 && actor.parentId == 0) || actor.parentId != 0){
		b = malloc(sizeof(squirrel));
		// Initialise squirrel environment  
		squirrelInit(&actor, b);
		// Return as type squirrel
	}
	return b;
}

void squirrelModel_work(void (*a), void(*b))
{
		
	// Dereference as actor to find rank
	actor_struct *ac = malloc(sizeof(actor_struct));
	ac = ((actor_struct *)a);

	int myRank = ac->myRank;
	int i = 0, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	// I am the timeKeeper
	if (myRank == 1 ){ 

		// Dereference as a timekeeper
		timeKeeper *t = malloc(sizeof(timeKeeper));
		t = ((timeKeeper *)b);
		timeKeeperCheckState(t, &i);
		
	}
	// I am land cell
	else if (myRank <= 17){ 
		
	
		// Dereference as land cell
		landCell *lc = malloc(sizeof(landCell));
		lc = ((landCell *)b);
	
		MPI_Request requestTime, request, req;
		MPI_Status statusTime, status;
		int timeMessage[2], message[2], flagSquirrel = 0, flagTime = 0, flagSquirrelM = 0, ierr;
		
		ierr = MPI_Irecv(timeMessage, 2, MPI_INT, 1, TIMEKEEPER_TAG, MPI_COMM_WORLD, &requestTime);// Rank1: timekeeper	
		if (ierr != MPI_SUCCESS)
			printf("LANDCELL: I couldn't receive from timekeeper\n");

		ierr = MPI_Irecv(message, 2, MPI_INT, MPI_ANY_SOURCE, SQUIRREL_TAG, MPI_COMM_WORLD, &request);	
		if (ierr != MPI_SUCCESS)
			printf("LANDCELL: I couldn't receive from squirrel\n");

		while(1){
		
			int die = shouldWorkerStop();
			if (die == 1){
				MPI_Cancel(&request);
				MPI_Cancel(&requestTime);
				MPI_Cancel(&req);
				break;
			}

			MPI_Test(&requestTime, &flagTime, &statusTime);
			if (flagTime == 1){
				if (timeMessage[0] == TWO_MONTHS){
					lc->infectionLevel = 0;
				}
				else if (timeMessage[0] == THREE_MONTHS){
					lc->populationInfluxIndex = 0;
					lc->populationInflux[0] = 0;
				}
				else if (timeMessage[0] == MONTH){
					// Every month the land cell sends its status (population influx + infection level)
					// Population influx -> 2 months
					// Infection level -> 3 months
					int mess[3], temp;
					temp = lc->populationInflux[0] + lc->populationInflux[1] + lc->populationInflux[2];
					mess[0] = lc->populationInflux[0] + lc->populationInflux[1] + lc->populationInflux[2];
					mess[1] = lc->infectionLevel;
					mess[2] = lc->actor.myRank;
					// Send info to timekeeper to print them
					ierr = MPI_Ssend(mess, 3, MPI_INT, 1, LANDCELL_TAG, MPI_COMM_WORLD);	
					if (ierr != MPI_SUCCESS)
						printf("LANDCELL: I couldn't send to timekeeper\n");

					lc->populationInfluxIndex++;
					lc->populationInflux[lc->populationInfluxIndex] = 0;
				}
				flagTime = 0;
				ierr = MPI_Irecv(timeMessage, 2, MPI_INT, 1, TIMEKEEPER_TAG, MPI_COMM_WORLD, &requestTime);
				if (ierr != MPI_SUCCESS)
					printf("LANDCELL: I couldn't receive from timekeeper\n");
			}

			MPI_Test(&request, &flagSquirrel, &status);
			if (flagSquirrel == 1){
				flagSquirrel = 0;
                                landCellCheckState(lc, &request, status, message, &req);
			
			}
		}

	}
	// I am master squirrel
	else if (myRank == 18){

		int initialSquirrels = 10, i, size, ierr;	// 19 - 29
		int flag = 0, flagTime =0, timeMessage[2];
		MPI_Request request, requestTime, request2[100];
		MPI_Status st, stTime;
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		int die;

		ierr = MPI_Irecv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, SQUIRREL_TAG, MPI_COMM_WORLD, &request);
		if (ierr != MPI_SUCCESS)
			printf("SQUIRREL MASTER: I couldnt recieve from initial squirrel\n");


		ierr = MPI_Irecv(timeMessage, 2, MPI_INT,1, TIMEKEEPER_TAG, MPI_COMM_WORLD, &requestTime);
		if (ierr != MPI_SUCCESS)
			printf("SQUIRREL MASTER: I couldnt recieve from timekeeper\n");


		while (1){
			die = shouldWorkerStop();
			MPI_Test(&requestTime, &flagTime, &stTime);
			if (flagTime == 1 || die == 1){
				for (i = 19; i < size; i++){ 
					ierr = MPI_Isend(NULL, 0, MPI_INT, i, 127,MPI_COMM_WORLD, &request2[i]);
					if (ierr != MPI_SUCCESS)
						printf("SQUIRREL MASTER: I couldnt recieve from timekeeper\n");
				}
				break;
			}
		}

		while(1){

			MPI_Test(&request, &flag, &st);
			if (flag == 1){
				flag = 0;
				initialSquirrels--;
				ierr = MPI_Irecv(NULL, 0, MPI_INT, MPI_ANY_SOURCE, SQUIRREL_TAG, MPI_COMM_WORLD, &request);
				if (ierr != MPI_SUCCESS)
					printf("SQUIRREL MASTER: I couldnt recieve from initial squirrel\n");

			}
			if (initialSquirrels == 0){
				MPI_Cancel(&request);
				break;
			}
		}
		if ( die == 0)
			// Ask master to shut down the pool (squirrels dead or time is off)
			shutdownPool();
	}
	// I am squirrel
	else if ((myRank < 30 && ac->parentId == 0) || ac->parentId != 0){
		
		// Dereference as a squirrel
		squirrel *sq = malloc(sizeof(squirrel));
		sq = ((squirrel *)b);

		MPI_Request requestTime, request, request2, req, request1, reqTime, r;
		MPI_Status statusTime, status, statusTimekeeper;
		int timeMessage[2], recvMessage[3], flagTime = 0, m[2], ierr, flag = 0, flagTimekeeper;
		
		ierr = MPI_Irecv(recvMessage, 3, MPI_INT, MPI_ANY_SOURCE, LANDCELL_TAG, MPI_COMM_WORLD, &request);
		if (ierr != MPI_SUCCESS)
			printf("SQUIRREL: I couldnt recieve from land cell\n");

		ierr = MPI_Irecv(NULL, 0, MPI_INT, 18, 127, MPI_COMM_WORLD, &request2);
		if (ierr != MPI_SUCCESS)
			printf("SQUIRREL : I couldnt recieve from squirrel master\n");

		while(1){

			MPI_Test(&request2, &flag, &status);
			if (flag == 1|| sq->dead == 1 ){
				// Cancel requests
				MPI_Cancel(&request);
				MPI_Cancel(&request2);
				break;
			}
		  	squirrelCheckState(sq, &request, recvMessage, &request1);
			squirrelUpdate(sq);
			
		}

		if (sq->actor.childrenNumber > 0){

			// Wait for all my children to die.
			MPI_Waitall(sq->actor.childrenNumber, sq->actor.childrenRequests, MPI_STATUS_IGNORE);		
			int message[2];
			message[0] = DEAD_ACTOR;
			message[1] = sq->actor.childrenNumber;
			// Tell master that these actors died
			ierr = MPI_Isend(message, 2, MPI_INT, 0, ACTOR_TAG, MPI_COMM_WORLD, &req); 
			if (ierr != MPI_SUCCESS)
				printf("SQUIRREL: I couldnt send to MASTER who died\n");

		}

		if (sq->actor.parentId == 0){	
			ierr = MPI_Ssend(NULL, 0, MPI_INT, 18, SQUIRREL_TAG, MPI_COMM_WORLD);
			if (ierr != MPI_SUCCESS)
				printf("SQUIRREL: I couldnt send to SQUIRREL MASTER\n");

		}
	}
	
}	

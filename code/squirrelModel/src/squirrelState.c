#include "squirrelState.h"

void squirrelCheckState(squirrel *sq, MPI_Request *request, int recvMessage[3],MPI_Request *request1)
{
	int sum = 0, i, j=0, flag = 0, ierr;
	MPI_Status status;
		
	// Inform the appropriate cell that I am infected
	int message[2];
	
	if (sq->healthCondition == 1){
		message[0] = INFECTED;	// Indicator that I am infected
	}
	else
		message[0] = HEALTHY;
	message[1] = sq->actor.myRank;
	
	ierr = MPI_Isend(message, 2, MPI_INT, sq->gridCell + 2, SQUIRREL_TAG, MPI_COMM_WORLD, request1); 
	if (ierr != MPI_SUCCESS)
		printf("SQUIRREL: I couldnt send to my cell\n");


	int reproduceProb;	
	if (sq->populationInfluxIndex == 50){
		for ( j = 0; j < 50; j++)
			sum += sq->populationInflux[j];
		reproduceProb = willGiveBirth((float)(sum), &sq->state);
		
		if (reproduceProb == 1){
			int sendM[2], respond;
			sendM[0] = CHILD;
			sendM[1] = sq->actor.myRank;
			MPI_Status st;
			
			MPI_Ssend(sendM, 2, MPI_INT, 0, ACTOR_TAG, MPI_COMM_WORLD);
			MPI_Recv(&respond, 1, MPI_INT, 0, MASTER_TAG, MPI_COMM_WORLD, &st);
			
			// If there are available processes create a child
			if (respond == 1){
				int childPid; 
				childPid = startWorkerProcess();
				MPI_Irecv(NULL, 0, MPI_INT, childPid, 0, MPI_COMM_WORLD, &(sq->actor.childrenRequests[sq->actor.childrenNumber]));
				if (ierr != MPI_SUCCESS)
					printf("SQUIRREL: I couldnt recieve from child squirrel\n");
				sq->actor.childrenId[sq->actor.childrenNumber] = childPid;
				sq->actor.childrenNumber++;		
			}
		}
	}

	sum = 0;
	if (sq->healthCondition == 0){	
		// Based on the new information I update my status
		for (i = 0; i < 50; i++)
			sum += sq->infectionArray[i];
		sq->healthCondition = willCatchDisease(sum/50.0, &sq->state);
	}
	else{
		if (sq->stepsToDeath == 50)
			sq->dead = willDie(&(sq->state));
		else
			sq->stepsToDeath++;
	}
	MPI_Test(request, &flag, &status);
	if (flag == 1){
		if (sq->infectionArrayIndex == 50){
			sq->infectionArrayIndex = 0;
			sq->populationInfluxIndex = 0;
		}
		sq->infectionArray[sq->infectionArrayIndex] = recvMessage[2];			
		sq->infectionArrayIndex++;

		sq->populationInflux[sq->populationInfluxIndex] = recvMessage[1];
		sq->populationInfluxIndex++;

		MPI_Irecv(recvMessage, 3, MPI_INT, MPI_ANY_SOURCE, LANDCELL_TAG, MPI_COMM_WORLD, request);	

	}


}


void squirrelUpdate(squirrel *sq)
{	
	// Update the position of the squirrel
	squirrelStep(sq->position[0], sq->position[1], &(sq->position[0]), &(sq->position[1]), &sq->state);
	sq->steps++;
	sq->gridCell = getCellFromPosition(sq->position[0], sq->position[1]);

}

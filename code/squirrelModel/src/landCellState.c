#include "landCellState.h"

void landCellCheckState(landCell *lc, 
			MPI_Request *request, 
			MPI_Status status, 
			int message[2], 
			MPI_Request *request1)
{	
	int ierr;
	// Another squirrel was here
	lc->populationInflux[lc->populationInfluxIndex]++;
	if (message[0] == INFECTED)
		//Another infected squirrel was here
		lc->infectionLevel++;
	ierr = MPI_Irecv(message, 2, MPI_INT, MPI_ANY_SOURCE, SQUIRREL_TAG, MPI_COMM_WORLD, request);
	if (ierr != MPI_SUCCESS)
		printf("LANDCELL: I couldn't receive from Squirrel\n");


	// Send to that squirrel the Infection level
	int sendMessage[3];
	// population Influx level
	int tmp_populationInflux = 0;
	int i;
	for (i = 0; i < 3; i++)
		tmp_populationInflux += lc->populationInflux[i];
	sendMessage[0] = lc->actor.myRank;
	sendMessage[1] = tmp_populationInflux;
	sendMessage[2] = lc->infectionLevel; 
	MPI_Isend(sendMessage, 3, MPI_INT, message[1], LANDCELL_TAG, MPI_COMM_WORLD, request1);
	if (ierr != MPI_SUCCESS)
		printf("LANDCELL: I couldn't send to squirrel\n");

}

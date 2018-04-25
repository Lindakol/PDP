#include "timeKeeperState.h"


void timeKeeperCheckState(timeKeeper *t, int *i)
{
		
	int size, j, ierr;
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	while (1){
		int die;
		MPI_Request request[16], recvReq[16];
		die = shouldWorkerStop(); 
			if (die == 1){
				for (j = 2; j < 18; j ++){
					MPI_Cancel(&request[j]);
					MPI_Cancel(&recvReq[j]);
				}
				break; 
			}

		if (t->days == MONTH){ // && t->months < 3){
			t->days = 0;
			t->months++;
			printf("=================================================================================\n");
			printf("                                MONTH %d\n", t->months);
			printf("=================================================================================\n");  
			int message[2];			
 			message[0] = MONTH;
			message[1] = t->actor.myRank;
			
			MPI_Status recSt[16], st, stSq;
			for (j = 2 ; j < 18; j++){
				ierr = MPI_Isend(message, 2, MPI_INT, j, TIMEKEEPER_TAG, MPI_COMM_WORLD, &request[j]);
				if (ierr != MPI_SUCCESS)
					printf("TIMEKEEPER: I couldnt inform everyone for the month\n");
			}	
			// Receive from land cells to print
			int recvMessage[48];	// 3*16
			
			int k = 0;
			for (j = 2; j < 18; j++){
				ierr = MPI_Irecv(&recvMessage[3*(j-2)], 3, MPI_INT, j, LANDCELL_TAG, MPI_COMM_WORLD, &recvReq[j]);
				if (ierr != MPI_SUCCESS)
					printf("TIMEKEEPER: I couldnt inform everyone for the month\n");
			}

			int messagesNum = 0;
			int flagRec[16];
			for (k = 2; k < 18; k++)
				flagRec[k] = 0;
			while (messagesNum < 16){
				for (k = 2; k < 18; k++){
					MPI_Test(&recvReq[k], &flagRec[k], &recSt[k]);
					if (flagRec[k] == 1){
						flagRec[k] = 0;
						messagesNum++;
					}
				}
			}
			for (j = 0; j < 16; j++)
				printf("LAND CELL ID: %d, populationInflux:%d, infectionLevel:%d\n", recvMessage[3*j+2], recvMessage[3*j+0], recvMessage[3*j+1]);

			die = shouldWorkerStop(); 
			if (die == 1){ 
				for (j = 2; j < 18; j ++)
					MPI_Cancel(&request[j]);
					MPI_Cancel(&recvReq[j]);
				break; 
			}
		}


		if (t->months == 24){
			int message[2];
			message[0] = FINAL_DAY;
			message[1] = t->actor.myRank;
			ierr = MPI_Ssend(message, 2, MPI_INT, 18 , TIMEKEEPER_TAG, MPI_COMM_WORLD);
			if (ierr != MPI_SUCCESS)
				printf("TIMEKEEPER: I couldnt inform everyone for the end of time\n");

			int die = shouldWorkerStop();
			while (die == 0){ 
				die = shouldWorkerStop();
			}
			for (j = 2; j < 18; j++){
				MPI_Cancel(&request[j]);
				MPI_Cancel(&recvReq[j]);
			}
			break; 
		}

		
		else if (t->months == 3){
			int message[2];
 			message[0] = THREE_MONTHS;
			message[1] = t->actor.myRank;
			for (j = 2 ; j < 18; j++){
				MPI_Isend(message, 2, MPI_INT, j, TIMEKEEPER_TAG, MPI_COMM_WORLD, &request[j]);
			if (ierr != MPI_SUCCESS)
				printf("TIMEKEEPER: I couldnt inform everyone for 3 months\n");
			}
				
			die = shouldWorkerStop(); 
			if (die == 1){ 
				for (j = 2; j < 18; j ++){
					MPI_Cancel(&request[j]);
					MPI_Cancel(&recvReq[j]);
				}
				break; 
			}
		}
		else if (t->months == 2){
			int message[2];			
 			message[0] = TWO_MONTHS;
			message[1] = t->actor.myRank;
			for (j = 2 ; j < 18; j++){
				MPI_Isend(message, 2, MPI_INT, j, TIMEKEEPER_TAG, MPI_COMM_WORLD, &request[j]);
			if (ierr != MPI_SUCCESS)
				printf("TIMEKEEPER: I couldnt inform everyone for 2 months\n");
			}

			die = shouldWorkerStop(); 
			if (die == 1){ 
				for (j = 2; j < 18; j ++){
					MPI_Cancel(&request[j]);
					MPI_Cancel(&recvReq[j]);
				}
				break; 
			}
		}
		
		t->days++;

			
	}
		
}

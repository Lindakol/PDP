#include "entities.h"


void squirrelInit(actor_struct *a, squirrel* sq)
{
	
	// Initialise the actor part
	sq->actor.myRank = a->myRank;
	sq->actor.parentId = a->parentId;
	sq->actor.actorId = a->actorId;
	sq->actor.childrenNumber = 0;
	sq->actor.childrenRequests = (MPI_Request*) malloc(200*sizeof(MPI_Request));
	sq->actor.childrenId = (int*) malloc(200*sizeof(int));

	long seed = - 1 - sq->actor.myRank;
	// Initialise the squirrel model part
	
	// Initial position
	initialiseRNG(&seed);
	sq->state = seed;
	squirrelStep(0,0,&(sq->position[0]), &(sq->position[1]), &(sq->state));

	sq->steps = 0;
	sq->gridCell = getCellFromPosition(sq->position[0], sq->position[1]);
	
	// Initialise state
	// Four random squirrel start the expansion of th disease
	if ((a->myRank == 18 || a->myRank == 19 || a->myRank == 21 || a->myRank == 23) && a->parentId == 0)
		sq->healthCondition = 1;
	else
		sq->healthCondition = 0;
	sq->stepsToDeath = 0;
	sq->dead = 0;

	// Initialise the infection array 
	int i;
	for (i = 0; i < 50; i++){
		sq->infectionArray[i] = 0;
		sq->populationInflux[i] = 0;
	}
	sq->infectionArrayIndex = 0;
	sq->populationInfluxIndex = 0;
}

void landCellInit(actor_struct *a, landCell* lc){
	// Actor
	lc->actor.myRank = a->myRank;
	lc->actor.parentId = a->parentId;
	lc->actor.actorId = a->actorId;
	lc->actor.childrenNumber = 0;

	// Land Cell
	lc->cellId = a->myRank;
	lc->populationInflux[0] = 0;
	lc->populationInflux[1] = 0;
	lc->populationInflux[2] = 0;
	lc->populationInfluxIndex = 0;
	lc->infectionLevel = 0;
}

void timeKeeperInit(actor_struct *a, timeKeeper* t){
	// Actor 
	t->actor.myRank = a->myRank;
	t->actor.parentId = a->parentId;
	t->actor.actorId = a->actorId;
	t->actor.childrenNumber = 0;
	
	// TIMEKEEPER
	t->days = 0;
	t->months = 0;
}




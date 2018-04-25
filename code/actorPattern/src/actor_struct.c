#include "actor_struct.h"

int actor_getRank(actor_struct *ac){
	return ac->myRank;
}

void actor_Init(actor_struct *actor, int myRank, int parentId, int actorId)
{
	actor->myRank = myRank;
	actor->parentId = parentId;
	actor->actorId = actorId;		
}
	

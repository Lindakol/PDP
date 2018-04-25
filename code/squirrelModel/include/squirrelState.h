/* ----------------------------------------------------------------------------------
 * 
 * This function is the main function executed by the squirrels
 *
 * ----------------------------------------------------------------------------------
*/


#include "entities.h"
#include "pool.h"

void squirrelCheckState(squirrel *sq, MPI_Request *r, int m[3], MPI_Request *r1);
void squirrelUpdate(squirrel *sq);



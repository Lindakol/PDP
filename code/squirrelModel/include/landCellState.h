/* ----------------------------------------------------------------------------------
 * 
 * This function is the main function executed by the land cells
 *
 * ----------------------------------------------------------------------------------
*/

#include "entities.h"
#include "defs.h"


void landCellCheckState(landCell *, 
			MPI_Request *r, 
			MPI_Status s, 
			int message[2], 
			MPI_Request *req);

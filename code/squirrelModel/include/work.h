/* ----------------------------------------------------------------------------------
 * 
 * These are the model specific initialisation and work functions 
 *
 * ----------------------------------------------------------------------------------
*/


#ifndef WORK_H
#define WORK_H


#include "work.h"
#include "entities.h"
#include "actor_struct.h"
#include "squirrelState.h"
#include "landCellState.h"
#include "timeKeeperState.h"
#include "actor_Setup.h"

void* squirrelModel_initialise(void (*a));
void squirrelModel_work(void (*a), void(*b));

#endif // WORK_H

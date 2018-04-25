/* ----------------------------------------------------------------------------------
 * 
 * Entities
 *  
 * The current file includes all the entities needed in the 
 * biologist's model, implemented as structs.
 * 
 * Squirrel 
 * Land cells
 * Timkeeper
 * ----------------------------------------------------------------------------------
 */

#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdio.h>
#include <malloc.h>

#include "squirrel-functions.h"
#include "actor_struct.h"
#include "defs.h"

typedef struct squirrel squirrel;
typedef struct landCell landCell;
typedef struct timeKeeper timeKeeper;

struct squirrel{

	// Actor related part of squirrel
	actor_struct actor;
	
	// Seed needed for the functions
	long state;
	// Number of steps taken. Needed for the 
	// probabilities
	int steps;
	// The cell that squirrel exists
	int gridCell;	
	// 2D model (x,y)
	float position[2];
	// Is the squirrel sick with parapoxvirus?
	int healthCondition;  // 0 - No, 1 - Yes
	// Reproduce probability
	int populationInflux[50];
	int populationInfluxIndex;



	// Probability to catch the disease
	//float diseaseProb;
	// Probability to die
	// An infected squirrel will live for 50 steps
	// and then they die with a fixed probability of 1/6
	// for each step
	int stepsToDeath;
	int dead;

	// This array contains the infection level of the 
	// last 50 steps
	int infectionArray[50];
	int infectionArrayIndex; // It is used so that we can keep track of the last 50 steps 


};

struct landCell{

	// Actor related part of land cell
	actor_struct actor;

	// 2D model
	// The biologists have a function to
	// transform a (x,y) position to cell number
	int cellId;
	// Total number of squirrels stepped 
	// in the cell the last 3 months
	int populationInflux[3];
	int populationInfluxIndex;
	
	// Total number of infected squirrels 
	// that have been in the cell the last 2 months
	int infectionLevel;
};

struct timeKeeper{
	actor_struct actor;
	// Updates the number of days that have passed
	int days;
	int months;
};	

// Initialise functions
void squirrelInit(actor_struct *actor, squirrel *sq);
void landCellInit(actor_struct *actor, landCell* lc);
void timeKeeperInit(actor_struct *actor, timeKeeper* t);



#endif // ENTITIES_H

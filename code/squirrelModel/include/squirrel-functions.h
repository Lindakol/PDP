#ifndef _HELPER_FUNCTIONS_H
#define _HELPER_FUNCTIONS_H

// To be standard compliant we define our own version of M_PI
#define M_PI (3.14159265358979323846264338327950288)



void initialiseRNG(long *);

void squirrelStep(float, float, float *, float *, long *);

int willGiveBirth(float, long *);

int willCatchDisease(float, long *);

int willDie(long *);

int getCellFromPosition(float, float);

#endif

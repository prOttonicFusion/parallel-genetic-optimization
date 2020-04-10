/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#ifndef GENETICS_H
#define GENETICS_H
#include "individ.hpp"
#include <iostream>

/**
 * Merge the genome of two individs to produce offspring
 * This operation replaces the parents with two children
 **/
void breedIndivids(int indexToBreed1, int indexToBreed2, Individ population[], float xpos[], float ypos[], int popSize, int Ncities);

void mutateIndivid();

float distanceBetweenCities(int index, float xpos[], float ypos[]);

#endif
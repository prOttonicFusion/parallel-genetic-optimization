/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#ifndef GENETICS_H
#define GENETICS_H
#include "individ.hpp"
#include <iostream>
#include <random>

/**
 * Merge the genome of two individs to produce offspring
 * This operation replaces the 2 least fittest individs with the new offspring
 **/
Individ breedIndivids(Individ parent1, Individ parent2, float xpos[], float ypos[], int popSize, int Ncities);

void mutateIndivid(Individ individ, int Ncities, std::mt19937 rng);

float distanceBetweenCities(int index, float xpos[], float ypos[]);

#endif
/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#ifndef GENETICS_H
#define GENETICS_H
#include "individ.hpp"
#include <iostream>

void mutateIndivid();

/**
 * Populates fittest[] with the Nindivids fittest individs in population
 **/
void getFittestIndivids(Individ population[], int popSize, Individ fittest[], int Nindivids);

void mutateIndivid();

#endif
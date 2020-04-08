/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#include <iostream>

/**
 * Class for storing one solution
 * Route should contain the indices of the cities in the order they 
 * are visited
 **/
class Individ
{
  private:
  public:
  Individ(int route[], float xpos[], float ypos[], int Ncities);
  float distance;
  int *route;
};

void mutateIndivid();
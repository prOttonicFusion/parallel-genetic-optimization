/********************************************************************
 * Functions related to genetic operations
 *******************************************************************/

#include "genetics.hpp"
#include <algorithm>
#include <iostream>

float distanceBetweenCities(int index1, int index2, float xpos[], float ypos[])
{
  int xdiff = (xpos[index1] - xpos[index2]);
  int ydiff = (ypos[index1] - ypos[index2]);
  return xdiff * xdiff + ydiff * ydiff;
}

void breedIndivids(int indexToBreed1, int indexToBreed2, Individ population[], float xpos[], float ypos[], int popSize, int Ncities)
{
  /** 
     * Generate 2 children genoms heuristically:
     * - Take first city of one parent
     * - The Nth city in the child is chosen from the parents' Nth genes by checking 
     *   which of the two is closer to the child's N-1th city
     * - If the closer city is already found in the child, choose the other
     **/

  int childRoutes[2][Ncities];
  Individ parent1 = population[indexToBreed1];
  Individ parent2 = population[indexToBreed2];

  childRoutes[0][0] = parent1.route[0];
  childRoutes[0][1] = parent1.route[0];

  for (int k = 0; k < 2; k++)
    for (int i = 1; i < Ncities; i++)
    {
      int nextCity;
      if (std::find(childRoutes[k], childRoutes[k] + i, parent1.route[i]) != childRoutes[k] + i)
      {
        // Found parent1.route[i] in childRoutes
        nextCity = parent2.route[i];
      }
      else if (std::find(childRoutes[k], childRoutes[k] + i, parent2.route[i]) != childRoutes[k] + i)
      {
        // Found parent2.route[i] in childRoutes
        nextCity = parent1.route[i];
      }
      else
      {
        // Neither was found --> choose closest
        int dist1 = distanceBetweenCities(childRoutes[k][i - 1], parent1.route[i], xpos, ypos);
        int dist2 = distanceBetweenCities(childRoutes[k][i - 1], parent2.route[i], xpos, ypos);
        if (dist1 < dist2)
        {
          nextCity = parent1.route[i];
        }
        else
        {
          nextCity = parent2.route[i];
        }
      }
      childRoutes[k][i] = nextCity;
    }

    population[indexToBreed1].setRoute(childRoutes[0], xpos, ypos, Ncities);
    population[indexToBreed2].setRoute(childRoutes[1], xpos, ypos, Ncities);
}
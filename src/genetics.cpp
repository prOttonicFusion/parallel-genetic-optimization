/********************************************************************
 * Functions related to genetic operations
 *******************************************************************/

#include "genetics.hpp"

////////////////////// Individ-class functions //////////////////////

/**
 * Individ constructor
 **/
Individ::Individ(int route[], float xpos[], float ypos[], int Ncities)
{
  this->route = route;

  // Calculate the distance
  float d = 0.0, xdiff, ydiff;
  for (int i = 1; i < Ncities; i++)
  {
    xdiff = (xpos[route[i]] - xpos[route[i - 1]]);
    ydiff = (ypos[route[i]] - ypos[route[i - 1]]);
    d += xdiff * xdiff + ydiff * ydiff;
  }
  this->distance = d;
}

/**
 * Return the route as a string
 **/
std::string Individ::getRouteAsString(std::string cityNames[], int Ncities)
{
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    int index = this->route[i];
    routeStr += cityNames[index];
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}
/********************************************************************
 * Functions for the Individ class defined in individ.hpp
 *******************************************************************/

#include "city.hpp"
#include "individ.hpp"
#include <iostream>
#include <stdio.h>

////////////////////// Individ-class functions //////////////////////

/**
 * Individ constructor (no params)
 **/
Individ::Individ()
{
}

/**
 * Individ constructor (with params)
 **/
Individ::Individ(int route[], City cities[], int Ncities)
{
  this->init(route, cities, Ncities);
}

/**
 * Iinitialize new Individ
 **/
void Individ::init(int route[], City cities[], int Ncities)
{
  // Allocate & init route array
  this->route = new int[Ncities];
  this->setRoute(route, cities, Ncities);
}

/**
 * Set the individs solution (route) & calculate the distance
 **/
void Individ::setRoute(int route[], City cities[], int Ncities)
{
  // Copy route to Individ's internal route
  for (int i = 0; i < Ncities; i++)
  {
    this->route[i] = route[i];
  }

  // Calculate the distance
  float d = 0.0, xdiff, ydiff;
  for (int i = 1; i < Ncities; i++)
  {
    xdiff = (cities[route[i]].xpos - cities[route[i - 1]].xpos);
    ydiff = (cities[route[i]].ypos - cities[route[i - 1]].ypos);
    d += xdiff * xdiff + ydiff * ydiff;
  }
  xdiff = (cities[route[0]].xpos - cities[route[Ncities - 1]].xpos);
  ydiff = (cities[route[0]].ypos - cities[route[Ncities - 1]].ypos);
  d += xdiff * xdiff + ydiff * ydiff;
  this->distance = d;
}

/**
 * Individ destructor
 **/
// Individ::~Individ()
// {
//   delete[] route; // Clear memory allocated for the array
//   route = NULL;   // Clear pointer
// }

/**
 * Return the route as a string
 **/
std::string Individ::getRouteAsString(City cities[], int Ncities)
{
  std::string routeStr;
  for (int i = 0; i < Ncities; i++)
  {
    int index = this->route[i];
    routeStr += cities[index].name;
    if (i < Ncities - 1) routeStr += "-";
  }
  return routeStr;
}

///////////////////// Individ operator overloads ////////////////////

// >
bool operator>(const Individ &i1, const Individ &i2)
{
  return i1.distance > i2.distance;
}

// <
bool operator<(const Individ &i1, const Individ &i2)
{
  return i1.distance < i2.distance;
}

// ==
bool operator==(const Individ &i1, const Individ &i2)
{
  return (i1.distance == i2.distance && i1.route == i2.route);
}

// !=
bool operator!=(const Individ &i1, const Individ &i2)
{
  return !(i1 == i2);
}
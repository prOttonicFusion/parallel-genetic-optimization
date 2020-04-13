/********************************************************************
 * @file      individ.cpp
 * @author    O. Lindblom
 * 
 * Functions for the Individ class defined in individ.hpp
 * 
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 * 
 *******************************************************************/

#include "individ.hpp"
#include "city.hpp"
#include <iostream>
#include <stdio.h>

Individ::Individ() {}

Individ::Individ(int route[], City cities[], int Ncities)
{
  this->init(route, cities, Ncities);
}

void Individ::init(int route[], City cities[], int Ncities)
{
  // Allocate & init route array
  this->route = new int[Ncities];
  this->setRoute(route, cities, Ncities);
}

void Individ::setRoute(int route[], City cities[], int Ncities)
{
  // Copy route to Individ's internal route
  for (int i = 0; i < Ncities; i++)
    this->route[i] = route[i];

  // Calculate the lenght of the route
  // including the distance from last city back to first
  float l = 0.0, xdiff, ydiff;
  for (int i = 1; i < Ncities; i++)
  {
    xdiff = (cities[route[i]].xpos - cities[route[i - 1]].xpos);
    ydiff = (cities[route[i]].ypos - cities[route[i - 1]].ypos);
    l += xdiff * xdiff + ydiff * ydiff;
  }
  xdiff = (cities[route[0]].xpos - cities[route[Ncities - 1]].xpos);
  ydiff = (cities[route[0]].ypos - cities[route[Ncities - 1]].ypos);
  l += xdiff * xdiff + ydiff * ydiff;
  this->routeLength = l;
}

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

/**
 * Operator overloadings
 */

// >
bool operator>(const Individ &i1, const Individ &i2)
{
  return i1.routeLength > i2.routeLength;
}

// <
bool operator<(const Individ &i1, const Individ &i2)
{
  return i1.routeLength < i2.routeLength;
}

// ==
bool operator==(const Individ &i1, const Individ &i2)
{
  return (i1.routeLength == i2.routeLength && i1.route == i2.route);
}

// !=
bool operator!=(const Individ &i1, const Individ &i2)
{
  return !(i1 == i2);
}
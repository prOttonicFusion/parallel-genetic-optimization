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
  float l = 0.0;
  for (int i = 1; i < Ncities; i++)
    l += distanceBetweenCities(cities[route[i]], cities[route[i - 1]]);

  l += distanceBetweenCities(cities[route[Ncities - 1]], cities[route[0]]);

  this->routeLength = l;
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
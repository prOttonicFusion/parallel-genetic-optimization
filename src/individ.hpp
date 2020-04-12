/********************************************************************
 * Header file for individ.cpp. Contains the Individ-class
 *******************************************************************/

#ifndef INDIVID_H
#define INDIVID_H
#include <iostream>
#include "city.hpp"

/**
 * Class for storing one solution to the problem, i.e. one route
 * and a corresponding total distance
 **/
class Individ
{
  public:
  float distance; // The squared total distance travelled
  int *route;     // An array containin the visited cities' indices in visiting order
  Individ();      // Constructor
  Individ(int route[], City cities[], int Ncities);
  // ~Individ();     // Desturctor
  void init(int route[], City cities[], int Ncities);
  void setRoute(int route[], City cities[], int Ncities);
  std::string getRouteAsString(City cities[], int Ncities);

  // Operator overloads
  friend bool operator>(const Individ &i1, const Individ &i2);
  friend bool operator<(const Individ &i1, const Individ &i2);
  friend bool operator==(const Individ &i1, const Individ &i2);
  friend bool operator!=(const Individ &i1, const Individ &i2);
};

#endif
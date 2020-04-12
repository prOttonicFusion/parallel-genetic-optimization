/********************************************************************
 * Header file for individ.cpp. Contains the Individ-class
 *******************************************************************/

#ifndef INDIVID_H
#define INDIVID_H
#include <iostream>

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
  Individ(int route[], float xpos[], float ypos[], int Ncities);
  // ~Individ();     // Desturctor
  void init(int route[], float xpos[], float ypos[], int Ncities);
  void setRoute(int route[], float xpos[], float ypos[], int Ncities);
  std::string getRouteAsString(std::string cityNames[], int Ncities);

  // Operator overloads
  friend bool operator>(const Individ &i1, const Individ &i2);
  friend bool operator<(const Individ &i1, const Individ &i2);
  friend bool operator==(const Individ &i1, const Individ &i2);
  friend bool operator!=(const Individ &i1, const Individ &i2);
};

#endif
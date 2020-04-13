/********************************************************************
 * Header file for individ.cpp. Contains the Individ-class
 *******************************************************************/

#ifndef INDIVID_H
#define INDIVID_H
#include "city.hpp"
#include <iostream>

/**
 * Class for storing one solution to the problem, i.e. one route
 * and the corresponding total route length
 * 
 */
class Individ
{
  public:
  float routeLength; // The squared total length of the route
  int *route;     // An array containin the visited cities' indices in visiting order

  /**
   * Construct a new Individ object
   */
  Individ();

  /**
   * Construct a new Individ object
   * 
   * @param route [in] Int array of city indices
   * @param cities [in] The actual cities as City struct array
   * @param Ncities [in] Number of cities in route
   */
  Individ(int route[], City cities[], int Ncities);

  /**
   * Initialize Individ with route. Same as setRoute, but
   * also allocates the route array
   * 
   * @param route [in] Int array of city indices
   * @param cities [in] The actual cities as City struct array
   * @param Ncities [in] Number of cities in route
   */
  void init(int route[], City cities[], int Ncities);

  /**
   * Set the route of the Individ object & calculate the route length
   * 
   * @param route [in] Int array of city indices
   * @param cities [in] The actual cities as City struct array
   * @param Ncities [in] Number of cities in route
   */
  void setRoute(int route[], City cities[], int Ncities);

  /**
   * Get the route as a string, i.e. all city names on a single line 
   * 
   * @param cities [in] The cities as City struct array
   * @param Ncities [in] Number of cities in route
   * @return std::string 
   */
  std::string getRouteAsString(City cities[], int Ncities);

  /**
   * Operator overloads
   */
  friend bool operator>(const Individ &i1, const Individ &i2);
  friend bool operator<(const Individ &i1, const Individ &i2);
  friend bool operator==(const Individ &i1, const Individ &i2);
  friend bool operator!=(const Individ &i1, const Individ &i2);
};

#endif
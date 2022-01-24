/********************************************************************
 * @file      individ.hpp
 * @author    O. Lindblom
 *
 * The Individ class
 *
 *******************************************************************/

#ifndef INDIVID_H
#define INDIVID_H
#include "city.hpp"
#include <iostream>
#include <vector>

/**
 * Class for storing one solution to the problem, i.e. one route
 * and the corresponding total route length
 *
 */
class Individ
{
  public:
  int Ncities;            ///< Number of cities along a route
  float routeLength;      ///< The squared total length of route travelled
  std::vector<City> route; ///< An array containin the visited cities' indices in visiting order

  /**
   * Construct a new blank Individ object
   */
  Individ();

  /**
   * Construct a new Individ object from the provided route
   *
   * @param route [in] Vector of City structs
   */
  Individ(const std::vector<City> &route);

  /**
   * Initialize Individ with route. Same as setRoute, but also resize the route vector
   *
   * @param route [in] Vector of City structs
   */
  void init(const std::vector<City> &route);

  /**
   * Set the route of the Individ object & re-calculate the route length
   *
   * @param route [in] Vector of City structs
   */
  void setRoute(const std::vector<City> &route);

  /**
   * Operator overloadings
   */
  friend bool operator>(const Individ &i1, const Individ &i2);
  friend bool operator<(const Individ &i1, const Individ &i2);
  friend bool operator==(const Individ &i1, const Individ &i2);
  friend bool operator!=(const Individ &i1, const Individ &i2);
};

#endif
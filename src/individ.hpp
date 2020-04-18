/********************************************************************
 * @file      individ.hpp
 * @author    O. Lindblom
 *
 * Header file for individ.cpp. Contains the Individ-class
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
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
  std::vector<int> route; ///< An array containin the visited cities' indices in visiting order

  /**
   * Construct a new Individ object
   */
  Individ();

  /**
   * Construct a new Individ object
   *
   * @param route [in] Int vector of city indices
   * @param cities [in] The actual cities as City struct vector
   * @param Ncities [in] Number of cities in route
   */
  Individ(std::vector<int> route, std::vector<City> cities, int Ncities);

  /**
   * Initialize Individ with route. Same as setRoute, but
   * also allocates the route array
   *
   * @param route [in] Int vector of city indices
   * @param cities [in] The actual cities as City struct vector
   * @param Ncities [in] Number of cities in route
   */
  void init(std::vector<int> route, std::vector<City> cities, int Ncities);

  /**
   * Set the route of the Individ object & calculate the route length
   *
   * @param route [in] Int vector of city indices
   * @param cities [in] The actual cities as City struct vector
   * @param Ncities [in] Number of cities in route
   */
  void setRoute(std::vector<int> route, std::vector<City> cities, int Ncities);

  /**
   * Operator overloads
   */
  friend bool operator>(const Individ &i1, const Individ &i2);
  friend bool operator<(const Individ &i1, const Individ &i2);
  friend bool operator==(const Individ &i1, const Individ &i2);
  friend bool operator!=(const Individ &i1, const Individ &i2);
};

#endif
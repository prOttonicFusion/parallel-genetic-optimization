/********************************************************************
 * @file      city.hpp
 * @author    O. Lindblom
 *
 * Header file for declaration of City struct
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#ifndef CITY_H
#define CITY_H
#include <iostream>
#include <vector>

/**
 * Data structure for storing all properties of a single city (gene)
 * These are: (string) City.name, (float) City.xpos and (float) City.ypos
 *
 */
struct City
{
  std::string name;
  float xpos;
  float ypos;
};

/**
 * Get the squared distance between two cities
 *
 * @param city1 [in] The first city
 * @param city2 [in] The second city
 * @return float Distance between city1 & city2
 */
float distanceBetweenCities(const City &city1, const City &city2);

/**
 * Convert a route in array-of-indices form to a string of city names
 *
 * @param route [in] Route as an array of indices pointing to elements in the cities array
 * @param cities [in] The cities as City struct array
 * @param Ncities [in] Number of cities along a route
 * @return std::string Route in string form
 */
std::string getRouteAsString(const std::vector<int> &route, const std::vector<City> &cities);

#endif
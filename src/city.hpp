/********************************************************************
 * @file      city.hpp
 * @author    O. Lindblom (prOttonicFusion)
 *
 * The City struct and related functions
 *
 *******************************************************************/

#ifndef CITY_H
#define CITY_H
#include <iostream>
#include <vector>

/**
 * Data structure for storing all properties of a single city (gene)
 * These are: (int) City.id, (float) City.xpos and (float) City.ypos
 *
 */
struct City
{
  int id;
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
 * @param route [in] Route as a vector of City structs
 * @return std::string Route in string form
 */
std::string getRouteAsString(const std::vector<City> &route);

#endif
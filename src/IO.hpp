/********************************************************************
 * @file      IO.hpp
 * @author    O. Lindblom
 *
 * Header file for IO.cpp
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#ifndef IO_H
#define IO_H
#include "city.hpp"
#include "genetics.hpp"
#include "individ.hpp"
#include <iostream>
#include <vector>

/**
 * Prints usage information to stdout
 *
 * @param programName  [in] The name of the program exetuable as listed in argv[0] of the main()
 */
void printUsageInfo(char *programName);

/**
 * Parse an input file named 'input.dat' and print error message if one occurs
 *
 * @param populationSize  [in/out]
 * @param eliteFraction  [in/out]
 * @param migrationSize  [in/out]
 * @param migrationPeriod  [in/out]
 * @param mutationProbability  [in/out]
 * @param tournamentSize  [in/out]
 * @return true If parse succeeded
 * @return flase If an error occured
 */
bool parseInputFile(int &populationSize, float &eliteFraction, int &migrationSize,
                    int &migrationPeriod, float &mutationProbability, int &tournamentSize);

/**
 * Parse a xyz-file into an array of City structs
 *
 * @param [in] inputFile Path to a input file in xyz-format
 * @param [out] Ncities Number of cities in the input file
 * @param [in] cities Cities as structs in an array
 * @return true If parse succeeded
 * @return flase If an error occured
 */
bool parseXYZFile(std::string &coordFile, int &Ncities, std::vector<City> &cities);

/**
 * Write a string to the output file
 *
 * @param [in] outputString Content to write to the file
 * @param [in] overWrite Should the file be overwritten?
 * @return true If write succeeded
 * @return flase If an error occured
 */
bool writeToOutputFile(const std::string &outputString, bool overWrite = false);

/**
 * Write a formatted entry to the output file. The entry has the following format:
 * Generation <Gen number>:
 * Length: <lenght of shortest route>
 * Route_indices: <route as indices pointing to the cities array>
 * Route_string: <route with city names>
 *
 * @param generation [in] Current generation number
 * @param fittest [in] The fittest individual
 * @param bestRouteStr [in] Best route as a string of city names
 * @param cities [in] Cities as structs in an array
 * @param overWrite [in] Should we overwrite the file?
 * @return true If write succeeded
 * @return flase If an error occured
 */
bool writeToOutputFile(int generation, const Individ &fittest, const std::vector<City> &cities,
                       bool overWrite = false);

/**
 * Write a formatted data entry to screen. The entry has the format:
 * Generation <Gen number>
 * Shortest route = <lenght of shortest route>
 *
 * @param generation [in] Current generation number
 * @param bestRouteLen [in] Length of the best route
 */
void writeToScreen(const int &generation, const float &bestRouteLen);

#endif
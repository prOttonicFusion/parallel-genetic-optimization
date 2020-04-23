/********************************************************************
 * @file      IO.cpp
 * @author    O. Lindblom
 *
 * Functions for handling input & output
 *
 * @date      2020-04-13
 * @copyright Copyright (c) 2020
 *
 *******************************************************************/

#include "IO.hpp"
#include "city.hpp"
#include "genetics.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool parseXYZFile(std::string &inputFile, int &Ncities, std::vector<City> &cities)
{
  std::ifstream infile(inputFile);
  std::string line;

  // On error
  if (!infile) return false;

  // Read number of cities
  std::getline(infile, line);
  std::istringstream iss(line);
  if (!(iss >> Ncities)) return false;

  // Initialize city vector accordingly
  cities.resize(Ncities);

  // Skip comment line
  std::getline(infile, line);

  // Read rest of file line-by-line
  int i = 0;
  while (std::getline(infile, line))
  {
    std::istringstream iss(line);
    std::string name;
    float x, y;

    // Attempt to parse line
    if (!(iss >> name >> x >> y))
    {
      break;
    }

    cities[i].name = name;
    cities[i].xpos = x;
    cities[i].ypos = y;

    i++;
  }
  return true;
}

bool writeToOutputFile(const std::string &outputString, bool overWrite)
{
  std::ofstream outputFile;
  if (overWrite)
  {
    outputFile.open("output.dat");
  }
  else
  {
    outputFile.open("output.dat", std::ios_base::app);
  }
  outputFile << outputString << std::endl;
  outputFile.close();
}

bool writeToOutputFile(int generation, const Individ &fittest, const std::vector<City> &cities, bool overWrite)
{
  std::ostringstream stringStream;
  std::string bestRouteStr = getRouteAsString(fittest.route, cities);
  stringStream << "Generation " << generation << ":" << std::endl;
  stringStream << "Length: " << fittest.routeLength << std::endl;
  stringStream << "Route_indices: ";
  for (int i = 0; i < fittest.route.size(); i++)
    stringStream << fittest.route[i] << " ";
  stringStream << std::endl;
  stringStream << "Route_string: " << bestRouteStr << std::endl;
  writeToOutputFile(stringStream.str(), overWrite);
}

void writeToScreen(const int &generation, const float &bestRouteLen)
{
  std::cout << "Generation " << generation << ": " << std::endl;
  std::cout << "  Shortest route = " << bestRouteLen << std::endl;
}
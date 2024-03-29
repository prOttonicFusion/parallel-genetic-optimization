/********************************************************************
 * @file      IO.cpp
 * @author    O. Lindblom (prOttonicFusion)
 *
 * Functions for handling input & output
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

// Prints usage information to stdout
void printUsageInfo(char *programName)
{
  std::cerr << "Usage: " << programName << " <coordFile> <maxGen> [wrtToScreen] [wrtToFile]"
            << std::endl;
  std::cerr << "  coordFile:   A xyz-file containing the city coordinates" << std::endl;
  std::cerr << "  maxGen:     The maximum number of generations to allow" << std::endl;
  std::cerr << "  wrtToScreen: Write results to screen every this many iterations. Default: 1"
            << std::endl;
  std::cerr << "  wrtToFile:   Write results to file every this many iterations. Default: 1"
            << std::endl;
}

// Parse an input file named 'input.dat'
bool parseInputFile(int &populationSize, float &eliteFraction, int &migrationSize,
                    int &migrationPeriod, float &mutationProbability, int &tournamentSize)
{
  std::ifstream infile("input.dat");

  std::string keyword, line, comment;
  float value;

  // Skip comment line
  std::getline(infile, line);

  int counter = 0;
  while (std::getline(infile, line))
  {
    std::istringstream stream(line);
    stream >> keyword >> value;

    if (keyword == "populationSize")
      populationSize = (int)value;
    else if (keyword == "eliteFraction")
      eliteFraction = value;
    else if (keyword == "migrationSize")
      migrationSize = (int)value;
    else if (keyword == "migrationPeriod")
      migrationPeriod = (int)value;
    else if (keyword == "mutationProbability")
      mutationProbability = value;
    else if (keyword == "tournamentSize")
      tournamentSize = (int)value;
    else
    {
      std::cerr << "Error: Unknown input file keyword '" << keyword << "'" << std::endl;
      return false;
    }
    counter++;
  }

  if (counter != 6)
  {
    std::cerr << "Error: Invalid number of input keywords: " << counter << std::endl;
    return false;
  }
  return true;
}

// Parse a xyz-file into an array of City structs
bool parseXYZFile(std::string &coordFile, int &Ncities, std::vector<City> &cities)
{
  std::ifstream infile(coordFile);
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

    cities[i].id = i;
    cities[i].xpos = x;
    cities[i].ypos = y;

    i++;
  }
  return true;
}

// Write a string to the output file 'output.dat'
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
  return true;
}

// Write a formatted entry to the output file 'output.dat'
bool writeToOutputFile(int generation, const Individ &fittest, const std::vector<City> &cities,
                       bool overWrite)
{
  std::ostringstream stringStream;
  std::string bestRouteStr = getRouteAsString(fittest.route);
  stringStream << "Generation " << generation << ":" << std::endl;
  stringStream << "Length: " << fittest.routeLength << std::endl;
  stringStream << "Route_indices: ";
  for (int i = 0; i < fittest.route.size(); i++)
    stringStream << fittest.route[i].id << " ";
  stringStream << std::endl;
  stringStream << "Route_string: " << bestRouteStr << std::endl;
  return writeToOutputFile(stringStream.str(), overWrite);
}

// Write a formatted data entry to stdout
void writeToScreen(const int &generation, const float &bestRouteLen)
{
  std::cout << "Generation " << generation << ": " << std::endl;
  std::cout << "  Shortest route = " << bestRouteLen << std::endl;
}
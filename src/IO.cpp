/********************************************************************
 * Functions for handling input & output
 *******************************************************************/
#include "city.hpp"
#include "IO.hpp"
#include "genetics.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool parseXYZFile(std::string inpuFile, int &Ncities, City *&cities)
{
  std::ifstream infile(inpuFile);
  std::string line;

  // On error
  if (!infile) return false;

  // Read number of cities
  std::getline(infile, line);
  std::istringstream iss(line);
  if (!(iss >> Ncities)) return false;

  // Initialize city array accordingly
  cities = new City[Ncities];

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


bool writeToOutputFile(std::string outputString, bool overWrite)
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

bool writeToOutputFile(int iterCount, int bestRoute[], std::string bestRouteStr, float bestRouteLen, int Ncities, bool overWrite)
{
  std::ostringstream stringStream;
  stringStream << "Iteration " << iterCount << ":" << std::endl;
  stringStream << "length: " << bestRouteLen << std::endl;
  stringStream << "routeIndices: ";
  for (int i = 0; i < Ncities; i++)
    stringStream << bestRoute[i] << " ";
  stringStream << std::endl;
  stringStream << "routeString: " << bestRouteStr << std::endl;
  writeToOutputFile(stringStream.str(), overWrite);
}

void writeToScreen(int iterCount, std::string bestRouteStr, float bestRouteLen)
{
  std::cout << "Iteration " << iterCount << ": " << std::endl;
  std::cout << "  Shortest route = " << bestRouteLen << std::endl;
  //std::cout << bestRouteStr << std::endl;
}
/********************************************************************
 * Functions for handling input & output
 *******************************************************************/
#include "IO.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * A function for reading in city coordinates from a xyz-file of the
 * format: 
 * <number of cities>
 * comment line
 * <name> <X> <Y>
 * ...
 * 
 **/
bool parseXYZFile(int &Ncities, std::string *&cityNames, float *&xpos, float *&ypos)
{
  std::ifstream infile("cityCoordinates.xyz");
  std::string line;

  // On error
  if (!infile) return false;

  // Read number of cities
  std::getline(infile, line);
  std::istringstream iss(line);
  if (!(iss >> Ncities)) return false;

  // Initialize arrays accordingly
  cityNames = new std::string[Ncities];
  xpos = new float[Ncities];
  ypos = new float[Ncities];

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

    cityNames[i] = name;
    xpos[i] = x;
    ypos[i] = y;

    i++;
  }
  return true;
}

/**
 * A function for saving calculation output to a file
 * 
 **/
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

/**
 * A function for printing calculation output to the screen/stdout
 * 
 **/
void writeToScreen(std::string outputString)
{
    std::cout << outputString << std::endl;
}
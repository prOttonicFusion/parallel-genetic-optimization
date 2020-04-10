/********************************************************************
 * Header file for individ.cpp. Contains the Individ-class
 *******************************************************************/

#include <iostream>

/**
 * Class for storing one solution
 **/
class Individ
{
  public:
  float distance; // The squared total distance travelled
  int *route;     // An array containin the visited cities' indices in visiting order
  Individ();      // Constructor
  ~Individ();     // Desturctor
  void init(int route[], float xpos[], float ypos[], int Ncities);
  void setRoute(int route[], float xpos[], float ypos[], int Ncities);
  std::string getRouteAsString(std::string cityNames[], int Ncities);
};

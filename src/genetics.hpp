/********************************************************************
 * Header file for genetics.cpp
 *******************************************************************/

#include <iostream>

/**
 * Class for storing one solution
 **/
class Individ
{
  private:
  public:
  float distance; // The squared total distance travelled
  int *route;     // An array containin the visited cities' indices in visiting order
  Individ();
  Individ(int route[], float xpos[], float ypos[], int Ncities);
  void setRoute(int route[], float xpos[], float ypos[], int Ncities);
  ~Individ();
  std::string getRouteAsString(std::string cityNames[], int Ncities);
};

void mutateIndivid();
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################################################
# @file      plotRoute.py
# @author    O. Lindblom
#
# A script for visualizing the results of a TSP simulation
#
# @date      2020-04-13
# @copyright Copyright (c) 2020
#####################################################################

import sys
import numpy as np
from matplotlib import pyplot as plt

####################### Load calculation data #######################
if len(sys.argv) != 3:
    print(
        "Usage: {} <coordinate-file> <TSP_GA-output>".format(sys.argv[0]))
    exit()

coordinateFile = sys.argv[1]
outputFile = sys.argv[2]
cities = np.genfromtxt(coordinateFile, skip_header=2, dtype=None,
                       names=[
                           'name', 'xpos', 'ypos'], encoding=None)
names = [c[0] for c in cities]
xpos = [c[1] for c in cities]
ypos = [c[2] for c in cities]

# Read routes from outputFile
iterCounts = []
routes = []
routeLengths = []
with open(outputFile) as f:
    lines = f.readlines()
    for line in lines:
        if ('Iteration' in line):
            spltdLine = line.split()
            iterCounts.append(int(spltdLine[1][0:-1]))

        if ('routeIndices' in line):
            spltdLine = line.split()
            newRoute = [int(i) for i in spltdLine[1:]]
            routes.append(newRoute)

        if ('length:' in line):
            spltdLine = line.split()
            routeLengths.append(float(spltdLine[1]))

# Generate plottable route


def plottableRoute(index):
    routeXCoords = [xpos[i] for i in routes[index]]
    routeYCoords = [ypos[i] for i in routes[index]]
    routeXCoords.append(xpos[routes[index][0]])
    routeYCoords.append(ypos[routes[index][0]])
    return routeXCoords, routeYCoords


############################ Draw figures ############################
# Optimal route plot
plt.figure(num=1, figsize=[8, 5])
plt.xlabel('x-position')
plt.ylabel('y-position')

routeX, routeY = plottableRoute(1)
plt.plot(routeX, routeY, ':')
plt.scatter(xpos, ypos)

for i, txt in enumerate(names):
    plt.annotate(txt, (xpos[i], ypos[i]))

plt.savefig('routes.png')
plt.show()

# Convergence plot
plt.figure(num=2, figsize=[8, 5])
plt.xlabel('Iteration count')
plt.ylabel('Length of shortest route')

plt.plot(iterCounts, routeLengths)

plt.savefig('convergence.png')
plt.show()

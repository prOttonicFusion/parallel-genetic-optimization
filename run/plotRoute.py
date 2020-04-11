#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################################################
#
# A script for visualizing the results of a TSP simulation
#
#####################################################################

import os
import numpy as np
from matplotlib import pyplot as plt

####################### Load calculation data #######################
coordinateFile = 'cityCoordinates.xyz'
outputFile = 'output.dat'
cities = np.genfromtxt(coordinateFile, skip_header=2, dtype=None, names = ['name','xpos','ypos'], encoding=None)
names = [c[0] for c in cities]
xpos = [c[1] for c in cities]
ypos = [c[2] for c in cities]

# TODO: Read route from outputFile

############################ Draw figure ############################
plt.figure(num=1, figsize=[8, 5])
plt.xlabel('x-position')
plt.ylabel('y-position')

plt.scatter(xpos, ypos)

for i, txt in enumerate(names):
    plt.annotate(txt, (xpos[i], ypos[i]))

plt.savefig('routes.png')
plt.show()
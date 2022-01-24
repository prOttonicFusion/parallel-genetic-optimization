#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#####################################################################
# @file      generateCities.py
# @author    O. Lindblom (prOttonicFusion)
# 
# A script for generating random city coordinates
# 
#####################################################################

import sys
import numpy as np

if len(sys.argv) == 4:
    print(
        "Usage: {} <output-file-name> <number-of-cities>".format(sys.argv[0]))
    exit()

outputFile = sys.argv[1]
Ncities = int(sys.argv[2])
worldDims = [0, 2000, 0, 2000]   # [xlow, xhigh, ylow, yhigh]
randPos = np.random.rand(Ncities, 2)

with open(outputFile, 'w') as f:
    f.write("{}\n".format(Ncities))
    f.write("This line is a comment\n")
    for i in range(Ncities):
        randX = randPos[i][0]*(worldDims[1]-worldDims[0]) + worldDims[0]
        randY = randPos[i][1]*(worldDims[3]-worldDims[0]) + worldDims[2]
        f.write("{} {} {}\n".format("City"+str(i), randX, randY))

# run/

This directory contains the input and output of a successful execution of the program. 

## Contents
```
run/
├── cityCoordinates.xyz -- Coordinates for the cities in xyz-format 
├── generateCities.py ---- Script for creating a random coordinate file
├── input.dat ------------ Input parameters for the genetic algorithm
├── output.dat ----------- Output of the program
└── plotRoute.py --------- Script for plotting calculation output
```


## Run instructions:
The program is executed by running: 
```bash
mpiexec -n <N> ../src/tsp_ga <coordinate-file> <Ngenerations> [toScreenInt] [toFileInt]
```

with `<N>` being the desired number of cores, `<coordinate-file>` the city coordinate file and `<Ngenerations>` the total number of generations. E.g.
```bash
mpiexec -n 4 ../src/tsp_ga cityCoordinates.xyz 1000 10 1
```
Additional calculation parameters, such as the population size can be adjusted in the 
`input.dat` file. The output, written to `output.dat`, can then be visualized using:
```bash
python3 plotRoute.py cityCoordinates.xyz output.dat
```
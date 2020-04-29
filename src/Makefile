#####################################################################
# @file      Makefile
# @author    O. Lindblom
# 
# Makefile for the Parallel Genetic Algortihm TSP code
# 
# @date      2020-04-13
# @copyright Copyright (c) 2020
#####################################################################

# Compiler
CC=mpic++   # Open MPI + GCC
#CC=mpicc    # Intel

# Optimization level
OPT=-O2 

# Executable name
EXE=tsp_ga

${EXE}: individ.o genetics.o IO.o city.o parallel.o main.o
	${CC} main.o parallel.o IO.o city.o individ.o genetics.o ${OPT} -o ${EXE}

city.o:
	${CC} -c city.cpp ${OPT}

IO.o:
	${CC} -c IO.cpp ${OPT}

individ.o:
	${CC} -c individ.cpp ${OPT}

genetics.o:
	${CC} -c genetics.cpp ${OPT}

parallel.o:
	${CC} -c parallel.cpp ${OPT}

main.o:
	${CC} -c main.cpp ${OPT}

clean:
	rm -f *.o

clean-all:
	rm -f *.o ${EXE}
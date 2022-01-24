#####################################################################
# @file      Makefile
# @author    O. Lindblom
# 
# Makefile for the Parallel Genetic Algortihm TSP code
# 
#####################################################################

# Compiler
CC=mpic++   # Open MPI + GCC
#CC=mpicc    # Intel

# Optimization level
OPT=-O2 

# Compiler flags
CCOPT= ${OPT} -std=c++11

# Executable name
EXE=tsp_ga

${EXE}: individ.o genetics.o IO.o city.o parallel.o population.o main.o
	${CC} main.o parallel.o population.o IO.o city.o individ.o genetics.o ${CCOPT} -o ${EXE}

city.o:
	${CC} -c city.cpp ${CCOPT}

IO.o:
	${CC} -c IO.cpp ${CCOPT}

individ.o:
	${CC} -c individ.cpp ${CCOPT}

genetics.o:
	${CC} -c genetics.cpp ${CCOPT}

parallel.o:
	${CC} -c parallel.cpp ${CCOPT}

population.o:
	${CC} -c population.cpp ${CCOPT}

main.o:
	${CC} -c main.cpp ${CCOPT}

clean:
	rm -f *.o

clean-all:
	rm -f *.o ${EXE}

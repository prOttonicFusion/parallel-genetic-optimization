# Compiler
CMP=mpic++

# Optimization level
OPT=-O2 

tsp_ga: individ.o genetics.o IO.o main.o
	${CMP} main.o IO.o individ.o genetics.o ${OPT} -o tsp_ga

IO.o:
	${CMP} -c IO.cpp ${OPT}

individ.o:
	${CMP} -c individ.cpp ${OPT}

genetics.o:
	${CMP} -c genetics.cpp ${OPT}

main.o:
	${CMP} -c main.cpp ${OPT}
	
clean:
	rm -f *.o

clean-all:
	rm -f *.o tsp_ga
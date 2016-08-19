CC = g++
FLAGS = -Ofast

LIBS = -L/usr/local/lib -lgsl -lgslcblas -lstdc++ -lm
INCLUDE = -I./source/include
OBJECTS = source/Conudl.o source/fileIO.o source/calcRates.o source/interactiveInput.o source/discLimit.o source/exclusionLimit.o source/nuFlux.o source/detectorFunctions.o source/formfactorSI.o source/monteCarlo.o source/likelihood.o source/nuRate.o source/SMrate.o source/BSMrate.o

default: Conudl

Conudl: $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

source/%.o: source/%.cpp
	$(CC) $< $(FLAGS) $(INCLUDE) -c -o $@

clean:
	-rm source/*.o
	-rm -f ./Conudl

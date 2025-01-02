all : solver

CCC = g++
CCFLAGS = -O3 -MMD -std=c++11 -g
LIBS= -lm
TARGETDIR=.
CPPFLAGS += \
         -I.

OBJS2 = \
	$(TARGETDIR)/keras_model.o \
	$(TARGETDIR)/Calculos.o \
	$(TARGETDIR)/RedNeuronal.o \
	$(TARGETDIR)/Kitral.o \
	$(TARGETDIR)/CommandLine.o \
	$(TARGETDIR)/Incompatibilidades.o \
	$(TARGETDIR)/Solver.o \
	$(TARGETDIR)/GreedyBaseline.o \
	$(TARGETDIR)/RandomizedGreedy.o \
	$(TARGETDIR)/GreedyPorAfinidad.o \
	$(TARGETDIR)/BruteForce.o \
	$(TARGETDIR)/Parsing.o \
	$(TARGETDIR)/Escritura.o \
	$(TARGETDIR)/Parametros.o \
	$(TARGETDIR)/Logging.o \
	$(TARGETDIR)/AnytimeRecorder.o \
	$(TARGETDIR)/main.o



$(TARGETDIR)/solver: $(OBJS2)
	$(CCC)  $(CCFLAGS) -o $(TARGETDIR)/solver $(OBJS2)

$(TARGETDIR)/keras_model.o: keras_model.h keras_model.cc
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c keras_model.cc -o $(TARGETDIR)/keras_model.o

$(TARGETDIR)/Logging.o: Logging.hpp Logging.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Logging.cpp -o $(TARGETDIR)/Logging.o

$(TARGETDIR)/AnytimeRecorder.o: AnytimeRecorder.hpp AnytimeRecorder.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c AnytimeRecorder.cpp -o $(TARGETDIR)/AnytimeRecorder.o

$(TARGETDIR)/Incompatibilidades.o: Incompatibilidades.hpp Incompatibilidades.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Incompatibilidades.cpp -o $(TARGETDIR)/Incompatibilidades.o

$(TARGETDIR)/Escritura.o: Escritura.hpp Escritura.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Escritura.cpp -o $(TARGETDIR)/Escritura.o

$(TARGETDIR)/Parsing.o: Parsing.hpp Parsing.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Parsing.cpp -o $(TARGETDIR)/Parsing.o

$(TARGETDIR)/Parametros.o: Parametros.hpp Parametros.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Parametros.cpp -o $(TARGETDIR)/Parametros.o

$(TARGETDIR)/CommandLine.o: CommandLine.hpp CommandLine.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c CommandLine.cpp -o $(TARGETDIR)/CommandLine.o

$(TARGETDIR)/RandomizedGreedy.o: RandomizedGreedy.hpp RandomizedGreedy.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c RandomizedGreedy.cpp -o $(TARGETDIR)/RandomizedGreedy.o

$(TARGETDIR)/GreedyBaseline.o: GreedyBaseline.hpp GreedyBaseline.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c GreedyBaseline.cpp -o $(TARGETDIR)/GreedyBaseline.o

$(TARGETDIR)/GreedyPorAfinidad.o: GreedyPorAfinidad.hpp GreedyPorAfinidad.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c GreedyPorAfinidad.cpp -o $(TARGETDIR)/GreedyPorAfinidad.o

$(TARGETDIR)/BruteForce.o: BruteForce.hpp BruteForce.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c BruteForce.cpp -o $(TARGETDIR)/BruteForce.o

$(TARGETDIR)/Solver.o: Solver.hpp Solver.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Solver.cpp -o $(TARGETDIR)/Solver.o

$(TARGETDIR)/Calculos.o:  Calculos.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Calculos.cpp -o $(TARGETDIR)/Calculos.o

$(TARGETDIR)/main.o: main.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c main.cpp -o $(TARGETDIR)/main.o

$(TARGETDIR)/Kitral.o: Kitral.hpp Kitral.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Kitral.cpp -o $(TARGETDIR)/Kitral.o

$(TARGETDIR)/RedNeuronal.o: RedNeuronal.hpp RedNeuronal.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c RedNeuronal.cpp -o $(TARGETDIR)/RedNeuronal.o


.PHONY : clean
clean:
	$(RM) \
	$(TARGETDIR)/solver \
	$(TARGETDIR)/Incompatibilidades.o \
		$(TARGETDIR)/Incompatibilidades.d \
	$(TARGETDIR)/main.o \
		$(TARGETDIR)/main.d \
		$(TARGETDIR)/Escritura.o \
			$(TARGETDIR)/Escritura.d \
	$(TARGETDIR)/CommandLine.o \
		$(TARGETDIR)/CommandLine.d \
	$(TARGETDIR)/RedNeuronal.o \
		$(TARGETDIR)/RedNeuronal.d \
	$(TARGETDIR)/keras_model.o \
		$(TARGETDIR)/keras_model.d \
	$(TARGETDIR)/RandomizedGreedy.o \
		$(TARGETDIR)/RandomizedGreedy.d \
	$(TARGETDIR)/GreedyBaseline.o \
		$(TARGETDIR)/GreedyBaseline.d \
	$(TARGETDIR)/AnytimeRecorder.o \
		$(TARGETDIR)/AnytimeRecorder.d \
	$(TARGETDIR)/GreedyPorAfinidad.o \
		$(TARGETDIR)/GreedyPorAfinidad.d \
	$(TARGETDIR)/GreedyPorAfinidad.o \
		$(TARGETDIR)/GreedyPorAfinidad.d \
	$(TARGETDIR)/BruteForce.o \
		$(TARGETDIR)/BruteForce.d \
	$(TARGETDIR)/Parametros.o \
		$(TARGETDIR)/Parametros.d \
	$(TARGETDIR)/Parsing.o \
		$(TARGETDIR)/Parsing.d \
	$(TARGETDIR)/Logging.o \
		$(TARGETDIR)/Logging.d \
	$(TARGETDIR)/Solver.o \
		$(TARGETDIR)/Solver.d \
	$(TARGETDIR)/Calculos.o \
		$(TARGETDIR)/Calculos.d \
	$(TARGETDIR)/Kitral.o \
		$(TARGETDIR)/Kitral.d

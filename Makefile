MAC_OPT = "-I/opt/X11/include"
CXX = g++
CXXFLAGS = -g -Wall -O2 -MMD -std=c++0x -L/vault/code/builds/leveldb -lleveldb -lz -lpthread $(MAC_OPT)
OBJECTS = InformationSet.o InformationSetManager.o MemoryISM.o Game.o cfr.o
DEPENDS = ${OBJECTS:.o=.d}

kuhn : ${OBJECTS} KuhnGame.o train_kuhn.o
	${CXX} ${OBJECTS} KuhnGame.o train_kuhn.o -o kuhn ${CXXFLAGS}

clean:
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

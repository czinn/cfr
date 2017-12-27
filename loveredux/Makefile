MAC_OPT = "-I/opt/X11/include"
CXX = g++
CXXFLAGS = -g -Wall -O2 -MMD -std=c++0x -L/vault/code/builds/leveldb -lleveldb -lz -lpthread $(MAC_OPT)
OBJECTS = ../InformationSet.o ../InformationSetManager.o ../LevelDBISM.o ../Game.o ../cfr.o LoveLetterGame.o
DEPENDS = ${OBJECTS:.o=.d}

all : love

love : ${OBJECTS} train_love.o strat_love.o
	${CXX} ${OBJECTS} train_love.o -o love ${CXXFLAGS}
	${CXX} ${OBJECTS} strat_love.o -o strat ${CXXFLAGS}

clean:
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

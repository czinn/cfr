MAC_OPT = "-I/opt/X11/include"
CXX = g++
CXXFLAGS = -g -Wall -O2 -MMD -std=c++0x -L/vault/code/builds/leveldb -lleveldb -lz -lpthread $(MAC_OPT)
OBJECTS = ../InformationSet.o ../InformationSetManager.o ../LevelDBISM.o ../Game.o ../cfr.o LoveLetterGame.o train_love.o
DEPENDS = ${OBJECTS:.o=.d}

all : love

love : ${OBJECTS}
	${CXX} ${OBJECTS} -o love ${CXXFLAGS}

clean:
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

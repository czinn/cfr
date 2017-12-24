MAC_OPT = "-I/opt/X11/include"
CXX = g++
CXXFLAGS = -g -Wall -O2 -MMD -std=c++0x -L/vault/code/builds/leveldb -lleveldb -lz -lpthread $(MAC_OPT)
OBJECTS = InformationSet.o InformationSetManager.o LevelDBISM.o Game.o cfr.o
DEPENDS = ${OBJECTS:.o=.d}

all : kuhn love

kuhn : ${OBJECTS} KuhnGame.o train_kuhn.o
	${CXX} ${OBJECTS} KuhnGame.o train_kuhn.o -o kuhn ${CXXFLAGS}

love : ${OBJECTS} LoveLetterGame.o train_love.o strat_love.o
	${CXX} ${OBJECTS} LoveLetterGame.o train_love.o -o love ${CXXFLAGS}
	${CXX} ${OBJECTS} LoveLetterGame.o strat_love.o -o love_strat ${CXXFLAGS}

clean:
	rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}

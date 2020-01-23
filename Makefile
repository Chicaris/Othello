OBJS = cPosition.o cOthelloBoard.o cPlayer.o cGame.o othello.o
HFILES = cPosition.h cOthelloBoard.h cPlayer.h cGame.h
CC = g++
CFLAGS = -Wall -std=c++11 -g
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS+= -DDEBUG
endif
PROFILE ?= 0
ifeq ($(PROFILE), 1)
	CFLAGS+= -DPROFILE
endif
MINMAX ?= 0
ifeq ($(MINMAX), 1)
	CFLAGS+= -DMINMAX
endif
MAX_IA_LEVEL ?= 0
ifneq ($(MAX_IA_LEVEL), 0)
	CFLAGS+= -DMAX_IA_LEVEL=$(MAX_IA_LEVEL)
endif

%.o: %.cpp $(HFILES)
	$(CC) $(CFLAGS) -c -o $@ $< 

all: projet

projet: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o projet

projetgraph: CFLAGS+= -DGRAPH
projetgraph: $(OBJS) graphic.o
	$(CC) $(CFLAGS) $(OBJS) graphic.o -lpthread -lglut -lGL -lGLU -o projetgraph

projet.zip: projet
	zip -l projet.zip ./*.cpp ./*.h ./Makefile

clean:
	rm -f *.o
	rm -f projet projetgraph
	rm -f *.zip


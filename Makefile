
CC=g++
PROF = -pg
CXXFLAGS= -g $(PROF) -O2  --std=c++11 -Wall -pedantic -o boggle
LDFLAGS= -g $(PROF) -O2 --std=c++11 -pthread

all: main 

main:  boggleplayer.o boggleutil.o

boggleplayer.o: boggleutil.h baseboggleplayer.h boggleplayer.h

boggleutil.o: boggleutil.h

clean:
	rm -f main *.o core*

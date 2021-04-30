all: Phase2Main.o RBTree.o
	g++ Phase2Main.o RBTree.o -o Phase2

phase1Main.o: Phase2Main.cpp
	g++ -c Phase2Main.cpp

RBTree.o: RBTree.cpp 
	g++ -c RBTree.cpp

clean:
	rm *.o output
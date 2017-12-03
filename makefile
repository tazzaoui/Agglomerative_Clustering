CC=g++
CFLAGS=--std=c++11 -Wall -pedantic
OBJ=cluster kdtree test part1 testing p1

part1: kdtree cluster p1
	$(CC) $(CFLAGS) cluster kdtree p1 -o part1
test: kdtree cluster testing
	$(CC) $(CFLAGS) cluster kdtree testing -o test
kdtree: KDTree.cpp KDTree.hpp
	$(CC) $(CFLAGS) -c KDTree.cpp -o kdtree
testing: Test.cpp
	$(CC) $(CFLAGS) -c Test.cpp -o testing
cluster: Cluster.cpp Cluster.hpp
	$(CC) $(CFLAGS) -c Cluster.cpp -o cluster
p1:	Part1.cpp
	$(CC) $(CFLAGS) -c Part1.cpp -o p1
clean:
	rm $(OBJ)

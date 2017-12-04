CC=g++
CFLAGS=--std=c++11 -Wall -pedantic
OBJ=cluster kdtree test euclid testing euclidian pair pairwise

euclidian: kdtree cluster euclid
	$(CC) $(CFLAGS) cluster kdtree euclid -o euclidian
pairwise: kdtree cluster pair
	$(CC) $(CFLAGS) cluster kdtree pair -o pairwise
test: kdtree cluster testing
	$(CC) $(CFLAGS) cluster kdtree testing -o test
kdtree: KDTree.cpp KDTree.hpp
	$(CC) $(CFLAGS) -c KDTree.cpp -o kdtree
testing: Test.cpp
	$(CC) $(CFLAGS) -c Test.cpp -o testing
cluster: Cluster.cpp Cluster.hpp
	$(CC) $(CFLAGS) -c Cluster.cpp -o cluster
euclid:	Euclidian.cpp
	$(CC) $(CFLAGS) -c Euclidian.cpp -o euclid
pair:	Pairwise.cpp
	$(CC) $(CFLAGS) -c Pairwise.cpp -o pair	
clean:
	rm $(OBJ)

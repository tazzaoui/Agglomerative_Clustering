#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>

typedef struct node Node;
struct node{
  double* point; /* Pointer to k doubles */
  Node* right;
  Node* left;
};

class KDTree{
  
private:
  size_t size;
  size_t dim;
  Node* root;
  /*Helper functions for Node manipulation*/
  static Node* get_node(double* p, size_t dim);
  static Node* insert_node(Node* root, double* point,
			   size_t dim, size_t depth = 0);
public:
  KDTree(size_t dim);
  bool insert(double* point, size_t depth = 0);
  bool same_points(double* p1, double* p2);
  Node find(double* p, size_t depth = 0);
};

#endif

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
  /*Helper statics for Node manipulation*/
  static bool same_points(double* a, double* b, size_t dim);
  static Node* get_node(double* p, size_t dim);
  static void insert_node(Node** root, double* point,
			   size_t dim, size_t depth = 0);
  static bool find_node(Node* root, double* p, size_t dim, size_t depth = 0);
  static void output_in_order(Node*, size_t);
  static void print_point(double* p, size_t dim);
  static void clear_tree(Node** root);
public:
  KDTree(size_t dim);
  ~KDTree();
  bool insert(double* point, size_t depth = 0);
  bool contains(double* p, size_t depth = 0);
  void print(void);
};


#endif

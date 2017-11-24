#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <limits.h>
#include <algorithm>

typedef struct node Node;
struct node{
  double* point; /* Pointer to k doubles */
  Node* right;
  Node* left;
};

class KDTree{
  
private:
  size_t num_elements;
  size_t dim;
  Node* root;
  /*Helper statics for Node manipulation*/
  static bool same_points(double* a, double* b, size_t dim);
  static Node* get_node(double* p, size_t dim);
  static Node* insert_node(Node* root, double* point,
			   size_t dim, size_t depth = 0);
  static Node* min_node(Node*, Node*, Node*, size_t);
  static Node* delete_node(Node* root, double* point, size_t dim, size_t depth = 0);
  static Node* find_min(Node* root, size_t dim, size_t qdim, size_t depth = 0);
  static void copy_points(double* src, double* dest, size_t dim);
  static void output_in_order(Node*, size_t);
  static Node* find_node(Node* root, double* p, size_t dim, size_t depth = 0);
  static void print_point(double* p, size_t dim);
  static void clear_tree(Node* root);
  static bool  contains_point(Node* root, double* point, size_t dim, size_t depth = 0);
  
public:
  KDTree(size_t dim);
  ~KDTree();
  bool insert(double* point);
  bool remove(double* point, size_t depth = 0);
  bool contains(double* p);
  double get_min(size_t dim, size_t depth = 0);
  void print(void);
  size_t size()const;
};

#endif


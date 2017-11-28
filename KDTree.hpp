#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <vector>
#include "Cluster.hpp"

typedef struct node Node;
struct node{
  node(Cluster &c);
  Cluster c;
  Node* right;
  Node* left;
};


class KDTree{
  
private:
  size_t num_elements;
  size_t dim;
  Node* root;
  /*Helper statics for Node manipulation*/
  static Node* get_node(Cluster &c);
  static Node* insert_node(Node* root, Cluster &c,
			   size_t dim, size_t depth = 0); 
  static Node* min_node(Node*, Node*, Node*, size_t);
  static Node* delete_node(Node* root, Cluster c, size_t dim, size_t depth = 0);
  static Node* find_min(Node* root, size_t dim, size_t qdim, size_t depth = 0);
  static void output_in_order(Node*, size_t);
  static Node* find_node(Node* root, Cluster c, size_t dim, size_t depth = 0);
  static void clear_tree(Node* root);
  static bool contains_cluster(Node* root, Cluster c, size_t dim, size_t depth=0);
  static bool contains_point(Node* root, double* point, size_t dim, size_t depth);
  static Cluster nearest(Node* root, Cluster* c, Cluster* min, size_t dim, size_t depth=0);
  static Cluster best_match(Node* root, Cluster* c, Cluster* min, size_t dim, size_t depth=0);
  static Cluster get_first_not_eq(Node* root, Cluster& c);
public:
  KDTree(size_t dim);
  ~KDTree();
  bool insert(const Cluster c); 
  bool remove(const Cluster c); 
  bool contains(const Cluster c); 
  Cluster get_min(size_t dim);
  Cluster nearest_neighbor(Cluster c);
  Cluster find_best_match(Cluster c);
  void print(void);
  size_t size()const;
};

#endif


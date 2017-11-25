#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <vector>

typedef struct cluster{
  std::vector<double*> data; /* Data points */
  double* point; /* Classificatoin point e.g. centroid */
}Cluster;

typedef struct node Node;
struct node{
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
  static bool same_points(double* a, double* b, size_t dim);
  static Node* get_node(Cluster cluster, size_t dim);
  static Node* insert_node(Node* root, Cluster c,
			   size_t dim, size_t depth = 0); 
  static Node* min_node(Node*, Node*, Node*, size_t);
  static Node* delete_node(Node* root, Cluster c, size_t dim, size_t depth = 0);
  static Node* find_min(Node* root, size_t dim, size_t qdim, size_t depth = 0);
  static void copy_points(double* src, double* dest, size_t dim);
  static void output_in_order(Node*, size_t);
  static Node* find_node(Node* root, Cluster c, size_t dim, size_t depth = 0);
  static void print_point(double* p, size_t dim);
  static void print_cluster(Cluster c, size_t dim);
  static void clear_tree(Node* root);
  static bool contains_point(Node* root, double* point, size_t dim, size_t depth = 0);
  static bool contains_cluster(Node* root, Cluster c, size_t dim, size_t depth = 0); 
  static bool same_clusters(Cluster &a, Cluster& b, size_t dim);
  static void copy_clusters(Cluster &src, Cluster &dest, size_t dim);
public:
  KDTree(size_t dim);
  ~KDTree();
  bool insert(Cluster c); 
  bool remove(Cluster c); 
  bool contains(Cluster c); 
  Cluster get_min(size_t dim); 
  void print(void);
  size_t size()const;
};

#endif


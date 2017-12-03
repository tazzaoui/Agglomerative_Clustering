#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <queue>
#include "Cluster.hpp"

typedef struct node Node;
struct node{
  node(const Cluster c);
  node(const Node*);
  Node* copy_tree(Node*);
  Cluster c;
  Node* right;
  Node* left;
};

class KDTree{
private:
  size_t num_elements;
  size_t dim;
  Node* root;

  /*======================Helper Statics For Node Manipulation================*/

  /* Add a new node containing a copy of the cluster passed in, into the tree.
     Return a pointer to the root of the new tree.*/
  static Node* append(Node*, const Cluster&, size_t depth=0);

  /*Calculate minimum of three Nodes, where:
    Node a < Node b iff a.centroid[dimension] <  b.centroid[dimension]*/
  static Node* min_node(Node*, Node*, Node*, size_t);

  /*Delete the node whose cluster is equal to the cluster passed in (if it exists)
    from the tree. Return a pointer the new tree*/
  static Node* remove_node(Node*, const Cluster&, size_t, size_t=0);

  /*Search for and return a pointer to the Node with the minimum value at centroid[qdim]*/
  static Node* find_min(Node*, size_t, size_t, size_t=0);

  /*In order traversal of the tree to print out each the cluster at each node*/
  static void in_order_traversal(Node*, size_t);

  /*Traverse the tree in search for the node that contains the cluster passed in.
    Return a pointer to the Node if it exists, NULL otherwise...*/
  static Node* search(Node*, const Cluster &c, size_t, size_t=0);

  /*Post-order traversal to delete every node from the tree - free up heap space*/
  static void clear_tree(Node*);

  /*Traverse the tree in search for the node that contains the cluster passed in.
    Return true if found, false otherwise*/
  static bool contains_cluster(Node*, const Cluster&, size_t, size_t=0);

  /*Traverse the tree in search for the node that contains the cluster that contains the point
    passed in.Return true if found, false otherwise*/
  static bool contains_point(Node*, double*, size_t, size_t=0);

  /*Return the Cluster that is nearest but not equal to the cluster in question*/
  static Cluster get_best_match(Node*, const Cluster&);

  /*Return the Cluster that is nearest but not equal to the cluster in question*/
  static Cluster get_best_pairwise_match(Node*, const Cluster&);

  /*Return the Cluster that is nearest (possibly equal) to the cluster in question*/
  static Cluster nearest_node(Node*, const Cluster*, Cluster*, size_t, size_t=0);

public:
  KDTree(size_t dim);
  ~KDTree();
  bool insert(const Cluster&); 
  bool remove(const Cluster&); 
  bool contains(const Cluster&) const; 
  Cluster get_min(size_t dim)const;
  Cluster nearest_neighbor(const Cluster&)const;
  Cluster find_best_match(const Cluster &)const;
  Cluster pairwise_best_match(const Cluster&)const;
  void print(void)const;
  size_t size(void)const;

  class CompareDistance{public: bool operator()(pair<double, Node*>a, pair<double, Node* >b){return a.first > b.first;};};
  
  typedef priority_queue < pair<double, Node*>,
			   vector<pair<double, Node*>>,
			   CompareDistance> dmin_heap;
};

#endif


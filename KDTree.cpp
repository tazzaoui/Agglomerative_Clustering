#include<iostream>
#include "KDTree.hpp"

using namespace std;

static Node* get_node(double* p, size_t dim){
  Node* temp = new Node;

  if(temp != NULL){
    temp->point = new double[dim];

    if(temp->point != NULL){
      for(size_t i = 0; i < dim; ++i)
	temp->point[i] = p[i];
    }else{
      delete temp; temp = NULL;
    }

    temp->right = NULL;
    temp->left = NULL;
  }
  
  return temp;
}

static void insert_node(Node** root, double* point,
		  size_t dim, size_t depth = 0){
  // Empty Tree...
  if(*root == NULL)
    *root = get_node(point, dim);
  
  // Spacial dimension (x,y,z.. in R^k) allignment
  size_t d = dim % depth; 

  // Traverse to correct position...
  if((*root)->point[d] > point[d])
    insert_node(& ((*root)->left), point, d+1);
  else
    insert_node(& ((*root)->right), point, d+1);
}

bool is_same(Node* a, Node* b, size_t dim){
  for(size_t i = 0; i < dim; ++i)
    if(a->point[i] != b->point[i])
      return false;
  return true;
}

KDTree::KDTree(size_t dim){
  this->size = 0;
  this->dim = dim;
  this->root = NULL;
}

bool KDTree::insert(double* point, size_t depth){
  insert_node(&(this->root), point, this->dim, depth);
  return this->root == NULL;
}



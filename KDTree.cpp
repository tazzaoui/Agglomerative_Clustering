#include "KDTree.hpp"

using namespace std;

Node* KDTree::get_node(double* p, size_t dim){
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

void KDTree::insert_node(Node** root, double* point,
		  size_t dim, size_t depth){
  // Empty Tree...
  if(*root == NULL)
    *root = get_node(point, dim);
  
  else{
    // Spacial dimension (x,y,z.. in R^k) allignment
    size_t d = depth % dim; 

    // Traverse to correct position...
    if((*root)->point[d] > point[d])
      insert_node(& ((*root)->left), point, dim, d+1);
    else
      insert_node(& ((*root)->right), point, dim, d+1);
  }
}

bool KDTree::same_points(double* a, double* b, size_t dim){
  for(size_t i = 0; i < dim; ++i)
    if(a[i] != b[i])
      return false;
  return true;
}

bool KDTree::find_node(Node* root, double* p, size_t dim, size_t depth){
  if(root == NULL)
    return false;

  if(same_points(root->point, p, dim))
    return true;

  size_t new_depth = depth % dim;

  if(root->point[new_depth] > p[new_depth])
    return find_node(root->left, p, dim, depth + 1);
  else
    return find_node(root->right, p, dim, depth + 1);
}

void KDTree::output_in_order(Node* root, size_t dim){
  if(root == NULL)
    return;
  output_in_order(root->left, dim);
  print_point(root->point, dim);
  output_in_order(root->right, dim);
}

void KDTree::clear_tree(Node** root){
  if(*root == NULL)
    return;

  clear_tree(&(*root)->left);
  clear_tree(&(*root)->right);

  delete [] (*root)->point;
  delete *root;
}

double KDTree::locate_min(Node* root, size_t dim, size_t qdim, size_t depth){
  if(root == NULL)
    return INT_MAX;

  size_t new_depth = depth % dim;
 
  if(new_depth == qdim)
    return (root->left == NULL) ? root->point[qdim] : locate_min(root->left, dim, qdim, depth+1);
  else{
    return  min(root->point[qdim],
	       min(locate_min(root->right, dim, qdim, depth+1),
		   locate_min(root->left, dim, qdim, depth+1)));
  }
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

bool KDTree::contains(double* point, size_t depth){
  return find_node(this->root, point, this->dim, depth);
}

void KDTree::print(){
  output_in_order(this->root, this->dim);
}

void KDTree::print_point(double* p, size_t dim){
  cout << "{";
  for(size_t i = 0; i < dim; ++i){
    cout << p[i];
    if(i != dim -1 ) cout << ",";
  }
  cout  << "}" << endl;
}

double KDTree::find_min(size_t qdim, size_t depth){
  return locate_min(this->root, this->dim, qdim, depth);
}

KDTree::~KDTree(){
  clear_tree( & this->root);
}

#include "KDTree.hpp"

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

Node* KDTree::insert_node(Node* root, double* point,
			  size_t dim, size_t depth){
  if(root == NULL)
    root = get_node(point, dim);
  else {
    size_t new_depth = depth % dim; 
    if(root->point[new_depth] > point[new_depth])
      root->left = insert_node(root->left, point, dim, depth + 1);
    else
      root->right = insert_node(root->right, point, dim, depth + 1);
  } 

  return root;
}

Node* KDTree::min_node(Node* a, Node* b, Node* c, size_t qdim){
  Node* res = a;
  if(b != NULL && b->point[qdim] < res->point[qdim])
    res = b;
  if(c != NULL && c->point[qdim] < res->point[qdim])
    res = c;
  return res;
}

Node* KDTree::find_min(Node* root, size_t dim, size_t qdim, size_t depth){
  if(root == NULL)
    return NULL;

  size_t new_depth = depth % dim;

  if(new_depth == qdim){
    if(root->left == NULL)
      return root;
    return find_min(root->left, dim, qdim, depth + 1);
  }
  return min_node(root,
		  find_min(root->left, dim, qdim, depth + 1),
		  find_min(root->right, dim, qdim, depth + 1), qdim);
}

void KDTree::copy_points(double* src, double* dest, size_t dim){
  for(size_t i = 0; i < dim; ++i)
     dest[i] = src[i];
}

Node* KDTree::delete_node(Node* root, double* point, size_t dim, size_t depth){
  if (root == NULL) return NULL;
 
  int new_depth = depth % dim;
  
  if (same_points(root->point, point, dim)){

    if (root->right != NULL){
      Node *min = find_min(root->right,dim,new_depth,0);
      copy_points(min->point, root->point, dim);
      root->right = delete_node(root->right, min->point, dim, depth+1);
    }else if (root->left != NULL){
      Node *min = find_min(root->left, dim, new_depth, 0);
      copy_points(min->point, root->point,dim);
      root->left = delete_node(root->left, min->point, dim, depth+1);
    }else{
      delete [] root->point;
      delete root;
      root = NULL;
      return NULL;
    }
    return root;
  }
  
  if (point[new_depth] < root->point[new_depth])
    root->left = delete_node(root->left, point, dim, depth+1);
  else
    root->right = delete_node(root->right, point, dim, depth+1);
  return root;
}

bool KDTree::same_points(double* a, double* b, size_t dim){
  for(size_t i = 0; i < dim; ++i)
    if(a[i] != b[i])
      return false;
  return true;
}

Node* KDTree::find_node(Node* root, double* p, size_t dim, size_t depth){
  if(root == NULL)
    return NULL;

  if(same_points(root->point, p, dim))
    return NULL;

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

void KDTree::clear_tree(Node* root){
  if(root == NULL)
    return;

  clear_tree(root->left);
  clear_tree(root->right);

  delete [] root->point;
  delete root;
}

bool KDTree::contains_point(Node* root, double* point, size_t dim, size_t depth){
  if(root == NULL)
    return false;

  if(same_points(root->point, point, dim)){
    return true;
  }
  size_t new_depth = depth % dim;

  if(root->point[new_depth] > point[new_depth])
    return contains_point(root->left, point, dim, depth+1);
 
  return contains_point(root->right, point, dim, depth+1);
}

KDTree::KDTree(size_t dim){
  this->num_elements = 0;
  this->dim = dim;
  this->root = NULL;
}


bool KDTree::insert(double* point){
  this->root = insert_node(this->root, point, this->dim);
  if(this->root) this->num_elements++;
  return this->root == NULL;
}

bool KDTree::remove(double* point, size_t depth){
  if(this->num_elements <= 0)
    return false;
  else this->num_elements--;
  this->root = delete_node(this->root, point, this->dim, depth);
  return true;
}
bool KDTree::contains(double* point){
  return contains_point(this->root, point, this->dim);
}

void KDTree::print(){
  output_in_order(this->root, this->dim);
}

void KDTree::print_point(double* p, size_t dim){
  std::cout << "{";
  for(size_t i = 0; i < dim; ++i){
    std::cout << p[i];
    if(i != dim -1 ) std::cout << ",";
  }
  std::cout  << "}" << std::endl;
}


size_t KDTree::size()const{
  return this->num_elements;
}

double KDTree::get_min(size_t qdim, size_t depth){
  Node* temp = find_min(this->root, this->dim, qdim, depth);
  return temp->point[qdim];
}

KDTree::~KDTree(){
  clear_tree(this->root);
}

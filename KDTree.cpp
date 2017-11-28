#include "KDTree.hpp"
#include "Cluster.hpp"
#include <math.h>

node::node(Cluster &c){
  this->c = c;
  this->left = NULL;
  this->right = NULL;
}

Node* KDTree::get_node(Cluster &cluster){  
  Node* temp = new Node(cluster);
  return temp;
}

Node* KDTree::insert_node(Node* root, Cluster &c,
			  size_t dim, size_t depth){
  if(root == NULL)
    root = get_node(c);
  else {
    size_t new_depth = depth % dim;
    if(root->c.get_point()[new_depth] > c.get_point()[new_depth])
      root->left = insert_node(root->left, c, dim, depth + 1);
    else
      root->right = insert_node(root->right, c, dim, depth + 1);
  } 
  return root;
}

Node* KDTree::min_node(Node* a, Node* b, Node* c, size_t qdim){
  Node* cur_min = a;
  if(b != NULL && b->c.get_point()[qdim] < cur_min->c.get_point()[qdim])
    cur_min = b;
  if(c != NULL && c->c.get_point()[qdim] < cur_min->c.get_point()[qdim])
    cur_min = c;
  return cur_min;
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

Node* KDTree::delete_node(Node* root, Cluster c, size_t dim, size_t depth){
  if (root == NULL) return NULL;

  int new_depth = depth % dim;
  
  if (root->c == c){
    if (root->right != NULL){
      Node *min = find_min(root->right,dim,new_depth,0);
      root->c = min->c;
      root->right = delete_node(root->right, min->c, dim, depth+1);
    }else if (root->left != NULL){
      Node *min = find_min(root->left, dim, new_depth, 0);
      root->c = min->c;
      root->right = delete_node(root->left, min->c, dim, depth+1);
    }else{
      delete root;
      return NULL;
    }
    return root;
  }
  
  if (c.get_point()[new_depth] < root->c.get_point()[new_depth])
    root->left = delete_node(root->left, c, dim, depth+1);
  else
    root->right = delete_node(root->right, c, dim, depth+1);
  return root;
}

Node* KDTree::find_node(Node* root, Cluster c, size_t dim, size_t depth){
  if(root == NULL)
    return NULL;

  if(root->c == c)
    return root;

  size_t new_depth = depth % dim;

  if(root->c.get_point()[new_depth] > c.get_point()[new_depth])
    return find_node(root->left, c, dim, depth + 1);
  else
    return find_node(root->right, c, dim, depth + 1);
}

void KDTree::output_in_order(Node* root, size_t dim){
  if(root == NULL)
    return;
  output_in_order(root->left, dim);
  std::cout << root->c << std::endl;
  std::cout << std::endl;
  output_in_order(root->right, dim);
}

void KDTree::clear_tree(Node* root){
  if(root == NULL)
    return;

  clear_tree(root->left);
  clear_tree(root->right);

  delete root;
}

bool KDTree::contains_point(Node* root, double* point, size_t dim, size_t depth){
  if(root == NULL)
    return false;

  bool same_pt = true;
  for(size_t i = 0; i < dim; ++i)
    if(point[i] != root->c.get_point()[i])
      same_pt = false;
  if(same_pt) return true;
  
  size_t new_depth = depth % dim;

  if(root->c.get_point()[new_depth] > point[new_depth])
    return contains_point(root->left, point, dim, depth+1);
 
  return contains_point(root->right, point, dim, depth+1);
}

bool KDTree::contains_cluster(Node* root, Cluster c, size_t dim, size_t depth){
  if(root == NULL)
    return false;

  if(root->c == c)
    return true;

  size_t new_depth = depth % dim;
  
  if(root->c.get_point()[new_depth] > c.get_point()[new_depth])
    return contains_cluster(root->left, c, dim, depth+1);
 
  return contains_cluster(root->right, c, dim, depth+1);
}

KDTree::KDTree(size_t dim){
  this->num_elements = 0;
  this->dim = dim;
  this->root = NULL;
}

bool KDTree::insert(Cluster c){
  this->root = insert_node(this->root, c, this->dim);
  if(this->root) this->num_elements++;
  return this->root == NULL;
}

bool KDTree::remove(Cluster c){
  if(this->num_elements <= 0)
    return false;
  else this->num_elements--;
  this->root = delete_node(this->root, c, this->dim, 0);
  return true;
}

bool KDTree::contains(const Cluster c){
  return contains_cluster(this->root, c, this->dim);
}

void KDTree::print(){
  output_in_order(this->root, this->dim);
}


Cluster KDTree::best_match(Node* root, Cluster* c, Cluster* min, size_t dim, size_t depth){
  if(root != NULL){
    /* This is to avoid the trivial case of nearest neighbor */
    if(root->c == *c){
      Cluster curent_min_cluster = *min;
      Cluster left_min_cluster = best_match(root->left, c, min, dim, depth);
      Cluster right_min_cluster = best_match(root->right, c, min, dim, depth);

      double current_min = c->euclidian_distance(curent_min_cluster);
      double left_min  = c->euclidian_distance(left_min_cluster);
      double right_min = c->euclidian_distance(right_min_cluster);
      double final_min = std::min(current_min, std::min(left_min, right_min));
      
      if(final_min == current_min)
	return curent_min_cluster;
      if(final_min == left_min)
	return left_min_cluster;
      else
	return right_min_cluster;
    }else{
      size_t qdim = depth % dim;
      double d = c->get_point()[qdim] - min->get_point()[qdim];
      Node* near =  d <= 0 ? root->left : root->right;
      Node* far =  d <= 0 ? root->right : root->left;
      *min = best_match(near, c, min, dim, depth+1);
      if(pow(d,2) < c->euclidian_distance(*min))
	*min = best_match(far, c, min, dim, depth + 1);
      if(c->euclidian_distance(root->c) <
	 c->euclidian_distance(*min))
	*min = root->c;
    }
  }
  return *min;
}
Cluster KDTree::nearest(Node* root, Cluster* c,
			Cluster* min, size_t dim, size_t depth){
  if(root != NULL){
    size_t qdim = depth % dim;
    double d = c->get_point()[qdim]- min->get_point()[qdim];
    Node* near =  d <= 0 ? root->left : root->right;
    Node* far =  d <= 0 ? root->right : root->left;
    *min = nearest(near, c, min, dim, depth+1);
    if(pow(d,2) < c->euclidian_distance(*min))
      *min = nearest(far, c, min, dim, depth + 1);
    if(c->euclidian_distance(root->c) <
       c->euclidian_distance(*min))
      *min = root->c;
    }
  
  return *min;
}

Cluster KDTree::nearest_neighbor(Cluster c){
  Cluster near = c;
  Cluster a = nearest(this->root, &c, &near, this->dim, 0);
  return a;
}

Cluster KDTree::get_first_not_eq(Node* root, Cluster& c){
  if(root != NULL){
    if(!(root->c == c)) return root->c;
    else if(!(root->left->c == c)) return root->left->c;
    else return root->right->c;
  }
  return c;
}
Cluster KDTree::find_best_match(Cluster c){
  Cluster near = get_first_not_eq(this->root, c);
  Cluster a = best_match(this->root, &c, &near, this->dim, 0);
  return a;
}

size_t KDTree::size()const{
  return this->num_elements;
}

Cluster KDTree::get_min(size_t qdim){
  Node* temp = find_min(this->root, this->dim, qdim, 0);
  return temp->c;
}

KDTree::~KDTree(){
  clear_tree(this->root);
}

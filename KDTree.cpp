#include "KDTree.hpp"
#include <math.h>

Node* KDTree::get_node(Cluster cluster, size_t dim){
  Node* temp = new Node;
  if(temp != NULL){
    temp->c.point = new double[dim];
    if(temp->c.point != NULL){
      for(size_t i = 0; i < dim; ++i)
	temp->c.point[i] = cluster.point[i];
      }
      else{
	delete temp; return NULL;
      }
    temp->c.data = std::vector<double*>();
    for(size_t i = 0; i < cluster.data.size() ;++i){
      temp->c.data.push_back(new double[dim]);
      for(size_t j = 0; j < dim; ++j)
	temp->c.data.at(i)[j] = cluster.data.at(i)[j];
    }
    temp->right = NULL;
    temp->left = NULL;
  }
  return temp;
}

Node* KDTree::insert_node(Node* root, Cluster c,
			  size_t dim, size_t depth){
  if(root == NULL)
    root = get_node(c, dim);
  else {
    size_t new_depth = depth % dim; 
    if(root->c.point[new_depth] > c.point[new_depth])
      root->left = insert_node(root->left, c, dim, depth + 1);
    else
      root->right = insert_node(root->right, c, dim, depth + 1);
  } 

  return root;
}

Node* KDTree::min_node(Node* a, Node* b, Node* c, size_t qdim){
  Node* cur_min = a;
  if(b != NULL && b->c.point[qdim] < cur_min->c.point[qdim])
    cur_min = b;
  if(c != NULL && c->c.point[qdim] < cur_min->c.point[qdim])
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

void KDTree::copy_points(double* src, double* dest, size_t dim){
  for(size_t i = 0; i < dim; ++i)
     dest[i] = src[i];
}

void KDTree::copy_clusters(Cluster &src, Cluster &dest, size_t dim){
  dest = src;
  copy_points(src.point, dest.point, dim);
}

bool KDTree::same_clusters(Cluster& a, Cluster& b, size_t dim){
  return (same_data(a.data, b.data, dim) && same_points(a.point, b.point, dim));
}

Node* KDTree::delete_node(Node* root, Cluster c, size_t dim, size_t depth){
  if (root == NULL) return NULL;

  int new_depth = depth % dim;
  
  if (same_clusters(root->c, c, dim)){
    if (root->right != NULL){
      Node *min = find_min(root->right,dim,new_depth,0);
      copy_clusters(min->c, root->c, dim);
      root->right = delete_node(root->right, min->c, dim, depth+1);
    }else if (root->left != NULL){
      Node *min = find_min(root->left, dim, new_depth, 0);
      copy_clusters(min->c, root->c,dim);
      root->right = delete_node(root->left, min->c, dim, depth+1);
    }else{
      for(size_t i = 0; i < c.data.size() ;++i)
        delete[] c.data.at(i);
      delete[] root->c.point;
      delete root;
      return NULL;
    }
    return root;
  }
  
  if (c.point[new_depth] < root->c.point[new_depth])
    root->left = delete_node(root->left, c, dim, depth+1);
  else
    root->right = delete_node(root->right, c, dim, depth+1);
  return root;
}

bool KDTree::same_points(double* a, double* b, size_t dim){
  for(size_t i = 0; i < dim; ++i)
    if(a[i] != b[i])
      return false;
  return true;
}

bool KDTree::same_data(std::vector<double*>& a, std::vector<double*>& b, size_t dim){
  if(a.size() != b.size()) return false;
  for(size_t i = 0; i < a.size(); ++i)
    for(size_t j = 0; j < dim; ++j)
      if(a.at(i)[j] != b.at(i)[j])
	return false;
  return true;
}

Node* KDTree::find_node(Node* root, Cluster c, size_t dim, size_t depth){
  if(root == NULL)
    return NULL;

  if(same_clusters(root->c, c, dim))
    return root;

  size_t new_depth = depth % dim;

  if(root->c.point[new_depth] > c.point[new_depth])
    return find_node(root->left, c, dim, depth + 1);
  else
    return find_node(root->right, c, dim, depth + 1);
}

void KDTree::output_in_order(Node* root, size_t dim){
  if(root == NULL)
    return;
  output_in_order(root->left, dim);
  print_cluster(root->c, dim);
  std::cout << std::endl;
  output_in_order(root->right, dim);
}

void KDTree::clear_tree(Node* root){
  if(root == NULL)
    return;

  clear_tree(root->left);
  clear_tree(root->right);

  for(size_t i = 0; i < root->c.data.size(); ++i)
    delete[] root->c.data.at(i);
  delete[] root->c.point;
  delete root;
}

bool KDTree::contains_point(Node* root, double* point, size_t dim, size_t depth){
  if(root == NULL)
    return false;

  if(same_points(root->c.point, point, dim)){
    return true;
  }
  size_t new_depth = depth % dim;

  if(root->c.point[new_depth] > point[new_depth])
    return contains_point(root->left, point, dim, depth+1);
 
  return contains_point(root->right, point, dim, depth+1);
}

bool KDTree::contains_cluster(Node* root, Cluster c, size_t dim, size_t depth ){
  if(root == NULL)
    return false;

  if(same_clusters(root->c, c, dim))
    return true;

  size_t new_depth = depth % dim;
  
  if(root->c.point[new_depth] > c.point[new_depth])
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

bool KDTree::contains(Cluster c){
  return contains_cluster(this->root, c, this->dim);
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
  std::cout  << "}";
}

void KDTree::print_cluster(Cluster c, size_t dim){
  std::cout << "Data = {";
  for(size_t i = 0; i < c.data.size(); ++i){
    print_point(c.data.at(i), dim);
    if(i != c.data.size() - 1) std::cout << ",";
  }
  std::cout << "}" << std:: endl;
  std::cout << "Centroid: ";
  print_point(c.point, dim);
}

double KDTree::euclidian_distance(double* p1, double* p2, size_t dim){
  double sum_of_squares = 0;
  for(size_t i = 0; i < dim; ++i)
    sum_of_squares = pow(p2[i] - p1[i], 2);
  return sqrt(sum_of_squares);
}

Cluster KDTree::best_match(Node* root, Cluster* c, Cluster* min, size_t dim, size_t depth){
  if(root != NULL){
    /* This is to avoid the trivial case of nearest neighbor */
    if(same_clusters(root->c, *c, dim)){
      Cluster curent_min_cluster = *min;
      Cluster left_min_cluster = nearest(root->left, c, min, dim, depth);
      Cluster right_min_cluster = nearest(root->right, c, min, dim, depth);
      
      double current_min = euclidian_distance(c->point, curent_min_cluster.point, dim);
      double left_min = euclidian_distance(c->point, left_min_cluster.point, dim);
      double right_min = euclidian_distance(c->point, right_min_cluster.point, dim);

      double final_min = std::min(current_min, std::min(left_min, right_min));
      if(current_min == final_min)
	return curent_min_cluster;
      else if(current_min == left_min)
	return left_min_cluster;
      else
	return right_min_cluster;
    }else{
    size_t qdim = depth % dim;
    double d = c->point[qdim] - min->point[qdim];
    Node* near =  d <= 0 ? root->left : root->right;
    Node* far =  d <= 0 ? root->right : root->left;
    *min = nearest(near, c, min, dim, depth+1);
    if(pow(d,2) < euclidian_distance(c->point, min->point, dim))
      *min = nearest(far, c, min, dim, depth + 1);
    if(euclidian_distance(c->point, root->c.point, dim) <
       euclidian_distance(c->point, min->point, dim))
      *min = root->c;
    }
  }
  return *min;
}
Cluster KDTree::nearest(Node* root, Cluster* c,
			Cluster* min, size_t dim, size_t depth){
  if(root != NULL){
    size_t qdim = depth % dim;
    double d = c->point[qdim] - min->point[qdim];
    Node* near =  d <= 0 ? root->left : root->right;
    Node* far =  d <= 0 ? root->right : root->left;
    *min = nearest(near, c, min, dim, depth+1);
    if(pow(d,2) < euclidian_distance(c->point, min->point, dim))
      *min = nearest(far, c, min, dim, depth + 1);
    if(euclidian_distance(c->point, root->c.point, dim) <
       euclidian_distance(c->point, min->point, dim))
      *min = root->c;
    }
  
  return *min;
}

Cluster KDTree::nearest_neighbor(Cluster c){
  Cluster near;
  Cluster a = nearest(this->root, &c, &near, this->dim, 0);
  return a;
}

Cluster KDTree::find_best_match(Cluster c){
  Cluster near;
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

#include "KDTree.hpp"
#include <stack>
#include "Cluster.hpp"
#include <math.h>
#include <functional>

Node::node(Cluster c){
  this->c = Cluster(c);
  this->left = NULL;
  this->right = NULL;
}

Node* KDTree::append(Node* root, const Cluster &c, size_t depth){
  if(root == NULL)
    root = new Node(c);
  else{
    size_t new_depth = depth % c.get_dim();
    if(root->c.get_point()[new_depth] > c.get_point()[new_depth])
      root->left = append(root->left, c, depth + 1);
    else
      root->right = append(root->right, c, depth + 1);
  } return root;
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

Node* KDTree::remove_node(Node* root, const Cluster &c, size_t dim, size_t depth){
  if (root == NULL) return NULL;

  int new_depth = depth % dim;
  
  if (root->c == c){
    if (root->right != NULL){
      Node *min = find_min(root->right,dim,new_depth,0);
      root->c = min->c;
      root->right = remove_node(root->right, min->c, dim, depth+1);
    }else if (root->left != NULL){
      Node *min = find_min(root->left, dim, new_depth, 0);
      root->c = min->c;
      root->right = remove_node(root->left, min->c, dim, depth+1);
    }else{
      delete root;
      return NULL;
    }
    return root;
  }
  
  if (c.get_point()[new_depth] < root->c.get_point()[new_depth])
    root->left = remove_node(root->left, c, dim, depth+1);
  else
    root->right = remove_node(root->right, c, dim, depth+1);
  return root;
}

Node* KDTree::search(Node* root, const Cluster &c, size_t dim, size_t depth){
  if(root == NULL)
    return NULL;
  if(root->c == c)
    return root;
  size_t new_depth = depth % dim;
  if(root->c.get_point()[new_depth] > c.get_point()[new_depth])
    return search(root->left, c, dim, depth + 1);
  else
    return search(root->right, c, dim, depth + 1);
}

void KDTree::in_order_traversal(Node* root, size_t dim){
  if(root == NULL)
    return;
  in_order_traversal(root->left, dim);
  cout << root->c;
  in_order_traversal(root->right, dim);
    
}

void KDTree::clear_tree(Node* root){
  if(root != NULL){
    if(root->left)
      clear_tree(root->left);
    if(root->right)
      clear_tree(root->right);
    delete root;
    root = NULL;
  }
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

bool KDTree::contains_cluster(Node* root,const Cluster &c, size_t dim, size_t depth){
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

bool KDTree::insert(const Cluster &c){
  this->root = append(this->root, c);
  if(this->root) this->num_elements++;
  return this->root == NULL;
}

bool KDTree::remove(const Cluster &c){
  if(this->num_elements <= 0)
    return false;
  this->num_elements--;
  this->root = remove_node(this->root, c, this->dim, 0);
  return true;
}

bool KDTree::contains(const Cluster &c)const{
  return contains_cluster(this->root, c, this->dim);
}

void KDTree::print()const{
  in_order_traversal(this->root, this->dim);
}


Cluster KDTree::get_best_match(Node* root, const Cluster& query){
  if(root == NULL) return query;

  dmin_heap pq;

  pair<double, Node*> best_match(std::numeric_limits<double>::max(), root);

  pq.emplace(0, root);
  size_t depth = 0;
  double dx, d;

  while(!pq.empty()){
    size_t qdim = depth % query.get_dim();
    if(query == pq.top().second->c){
      Node* left = pq.top().second->left;
      Node* right = pq.top().second->right;
      pq.pop();
      if(left != NULL)
	pq.push(make_pair(0, left));
      if(right != NULL)
	pq.push(make_pair(0, right));
      if(pq.empty()) break;
    }
    
    pair<double, Node*>  current = pq.top();
    
    if(current.first >= best_match.first)
      return best_match.second->c;
    
    Node* current_node = current.second;
    d = pow(current_node->c.euclidian_distance(query), 2);
    dx = query.get_point()[qdim] - current_node->c.get_point()[qdim];
    if(d < best_match.first){
      best_match.second = current_node;
      best_match.first  = d;
    }
   
    Node* near = dx <= 0 ? current_node->left : current_node->right;
    Node* far = dx <= 0 ? current_node->right : current_node->left;

    pq.pop();

    if(far != NULL)
      pq.emplace(pow(2, dx), far);
      
    if(near != NULL)
      pq.emplace(0, near);
    
    depth++;
  }
  return best_match.second->c;
}

Cluster KDTree::get_best_pairwise_match(Node* root, const Cluster& query){
  if(root == NULL) return query;

  dmin_heap pq;

  pair<double, Node*> best_match(std::numeric_limits<double>::max(), root);

  pq.emplace(0, root);
  size_t depth = 0;
  double dx, d;

  while(!pq.empty()){
    size_t qdim = depth % query.get_dim();
    if(query == pq.top().second->c){
      Node* left = pq.top().second->left;
      Node* right = pq.top().second->right;
      pq.pop();
      if(left != NULL)
	pq.push(make_pair(0, left));
      if(right != NULL)
	pq.push(make_pair(0, right));
      if(pq.empty()) break;
    }
    
    pair<double, Node*>  current = pq.top();
    
    if(current.first >= best_match.first)
      return best_match.second->c;
    
    Node* current_node = current.second;
    d = pow(current_node->c.pairwise_distance(query), 2);
    dx = query.get_point()[qdim] - current_node->c.get_point()[qdim];
    if(d < best_match.first){
      best_match.second = current_node;
      best_match.first  = d;
    }
   
    Node* near = dx <= 0 ? current_node->left : current_node->right;
    Node* far = dx <= 0 ? current_node->right : current_node->left;

    pq.pop();

    if(far != NULL)
      pq.emplace(pow(2, dx), far);
      
    if(near != NULL)
      pq.emplace(0, near);
    
    depth++;
  }
  return best_match.second->c;
}

Cluster KDTree::nearest_node(Node* root, const Cluster* c,
			     Cluster* min, size_t dim, size_t depth){
  if(root != NULL){
    size_t qdim = depth % dim;
    double d = c->get_point()[qdim]- min->get_point()[qdim];
    Node* near =  d <= 0 ? root->left : root->right;
    Node* far =  d <= 0 ? root->right : root->left;
    *min = nearest_node(near, c, min, dim, depth+1);
    if(pow(d,2) < c->euclidian_distance(*min))
      *min = nearest_node(far, c, min, dim, depth + 1);
    if(c->euclidian_distance(root->c) <
       c->euclidian_distance(*min))
      *min = root->c;
    }
  return *min;
}

Cluster KDTree::nearest_neighbor(const Cluster &c)const{
  Cluster near = c;
  return nearest_node(this->root, &c, &near, this->dim, 0);
}

Cluster KDTree::find_best_match(const Cluster &c) const{
  return get_best_match(this->root, c);
}

Cluster KDTree::pairwise_best_match(const Cluster& c)const{
  return get_best_pairwise_match(this->root, c);
}

size_t KDTree::size()const{
  return this->num_elements;
}

Cluster KDTree::get_min(size_t qdim)const{
  return find_min(this->root, this->dim, qdim, 0)->c;
}

KDTree::~KDTree(){
  clear_tree(this->root);
}

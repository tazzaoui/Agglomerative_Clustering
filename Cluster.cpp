#include "Cluster.hpp"
#include <iostream>
#include <math.h>
#include <limits.h>

Cluster::Cluster(){
  this->dim = 0;
  this->data = NULL;
  this->point = NULL;
}

size_t Cluster::get_dim()const{
  return this->dim;
}

Cluster::Cluster(std::vector<double*> &v, size_t dim){
  this->dim = dim;
  
  this->data = new std::vector<double*>();
 
  for(size_t i = 0; i < v.size(); ++i){
    this->data->push_back(new double[dim]);
    for(size_t j = 0; j < dim; ++j)
      this->data->at(i)[j] = v[i][j];
  }
  this->point = new double[dim];
  get_centroid(this->point, *(this->data), this->dim);  
}

Cluster::Cluster(const Cluster& a){
  this->dim = a.dim;
  
  this->data = new std::vector<double*>();

  for(size_t i = 0; i < a.data->size(); ++i){
    this->data->push_back(new double[dim]);
    for(size_t j = 0; j < dim; ++j)
      this->data->at(i)[j] = (a.data)->at(i)[j];
  }
  
  this->point = new double[dim];
  for(size_t i = 0; i < dim; ++i)
    this->point[i] = a.point[i];
}

Cluster::Cluster(const Cluster& a, const Cluster &b, bool output){
  if(output) cout << "Merging: cluster:" << a << "cluster: " << b << endl;

  this->dim = a.dim;
  this->data = new vector<double*>();
  this->point = new double[dim];

  
  for(size_t i = 0; i < a.data->size(); ++i){
    double* temp = new double[a.dim];
    for(size_t j = 0; j < a.dim; ++j){
      temp[j] = a.data->at(i)[j];
    }
    this->data->push_back(temp);
  }
  
  for(size_t i = 0; i < b.data->size(); ++i){
    double* temp = new double[b.dim];
    for(size_t j = 0; j < b.dim; ++j)
      temp[j] = b.data->at(i)[j];
    this->data->push_back(temp);
  }

  get_centroid(this->point, *(this->data), dim);
}

bool Cluster::operator==(const Cluster& rhs)const{
  if(rhs.data == NULL || this->data->size() != rhs.data->size())
    return false;
  for(size_t i = 0; i < this->data->size(); ++i)
    for(size_t j = 0; j < dim; ++j)
      if(this->data->at(i)[j] != rhs.data->at(i)[j])
	return false;
  return true;
}

Cluster& Cluster::operator=(const Cluster& rhs){
  if(this != &rhs) {
    if(this->data != NULL){
      while(!this->data->empty()){
	delete[] this->data->back();
	this->data->pop_back();
      }
       delete this->data;
    }

    if(this->point != NULL)
       delete[] this->point;
    this->dim = rhs.dim;
    this->data = new vector<double*>();
    for(size_t i = 0; i < rhs.data->size(); ++i){
      this->data->push_back(new double[dim]);
      for(size_t j = 0; j < dim; ++j)
	this->data->at(i)[j] = (rhs.data)->at(i)[j];
    } 
    this->point = new double[dim];
    for(size_t i = 0; i < rhs.dim; ++i)
      this->point[i] = rhs.point[i];
  }  
  return *this;
}

ostream& operator<<(ostream& os, const Cluster& rhs){
  if(rhs.data != NULL){
    //cout << "Data =";
    std::cout << "{";
    for(size_t i = 0; i < rhs.data->size(); ++i){
      Cluster::print_point(rhs.data->at(i), rhs.dim);
      if(i != rhs.data->size() - 1 ) cout << ",";
    }
    std::cout << "}" << endl;
    /*
    cout << "Centroid: ";
    Cluster::print_point(rhs.point, rhs.dim);
    */
  }
  return os;
}

Cluster::~Cluster(){
  delete[] this->point; this->point = NULL;
  while(!this->data->empty()){
    delete[] this->data->back();
    this->data->pop_back();
  }
  delete this->data;
  this->data = NULL;
}


const double* Cluster::get_point() const{
  return this->point;
}

void Cluster::print_point(double* p, size_t dim){
  if(p != NULL){
    std::cout << "{";
    for(size_t i = 0; i < dim; ++i){
      std::cout << p[i];
      if(i != dim -1 ) std::cout << ",";
    }
    cout  << "}";
  }
}


void Cluster::get_centroid(double* centroid, std::vector<double*> &v, size_t dim){
  for(size_t i = 0; i < v.size(); ++i)
    for(size_t j = 0; j < dim; ++j){
      if(i == 0) centroid[j] = v.at(i)[j];
      else centroid[j] += v.at(i)[j];
      if(i == v.size() - 1) centroid[j] /= dim;
    }
}

double Cluster::euclidian_distance(const Cluster& a) const{
  if(this->dim != a.dim) return INT_MAX;
  return point_euclidian_distance(this->point, a.point, this->dim);
}

double Cluster::point_euclidian_distance(double* p1, double* p2, size_t dim){
  double sum_of_squares = 0;
  if(p2 == NULL || p1 == NULL) return INT_MAX;
 
 for(size_t i = 0; i < dim; ++i){
   double x = p1[i] - p2[i];
   sum_of_squares += pow(x, 2);
 } 
 return sqrt(sum_of_squares);
}

double Cluster::pairwise_distance(const Cluster& a) const{
  double sum = 0;
  for(size_t i = 0; i < this->data->size(); ++i)
    for(size_t j = 0; j < a.get_data()->size(); ++j)
      sum += point_euclidian_distance(this->data->at(i), a.data->at(j), this->dim); 
  return sum /= (this->data->size() * a.data->size());
}

const vector<double*>* Cluster::get_data()const{
  return this->data;
}

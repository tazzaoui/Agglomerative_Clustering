#ifndef CLUSTER
#define CLUSTER
#include <iostream>
#include <vector>
using namespace std;

class Cluster{
private:
  vector<double*>* data; 
  double* point; 
  size_t dim;
  void get_centroid(double* centroid, std::vector<double*> &v, size_t dim);
  static void print_point(double*, size_t dim);
  static double point_euclidian_distance(double* p1, double* p2, size_t dim);
  
public:
  Cluster(void);
  Cluster(std::vector<double*>& data, size_t dim);
  Cluster(const Cluster&, const Cluster&, bool=false);
  Cluster(const Cluster&);
  
  const double* get_point(void)const;
  const vector<double*>* get_data(void)const;
  size_t get_dim() const;
  Cluster& operator=(const Cluster& rhs);
  bool operator==(const Cluster& rhs)const;
  friend ostream& operator<<(ostream&, const Cluster&);
  double euclidian_distance(const Cluster&) const;
  double pairwise_distance(const Cluster&) const;
  ~Cluster();
};
#endif

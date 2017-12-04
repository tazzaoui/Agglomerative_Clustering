#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <utility>
#include <math.h>
#include "KDTree.hpp"

using namespace std;

Cluster merge_clusters(Cluster& a, Cluster &b, size_t dim);

class CompareDist{public: bool operator()(pair<double, pair<Cluster, Cluster> >a, pair<double, pair<Cluster, Cluster> >b){return a.first > b.first;};};

int main(int argc, char* argv[]){
/* Place vectors from stdin into one intial cluster*/
  int unsigned numClusters;
  double input;
  vector<double> v;
  vector<double*> vp;
  vector<Cluster> clusts = vector<Cluster>(0);
  string line;
  priority_queue < pair<double, pair<Cluster, Cluster> >,
		   vector<pair<double, pair<Cluster, Cluster>>>,
		   CompareDist> min_heap;

  stringstream convert(argv[1]);
  
  if(!(convert >> numClusters)){
    cout << "Invalid number of clusters!";
    exit(1);
  }

  getline(cin, line);
  istringstream is(line);
  
  while(is >> input)
    v.push_back(input);
  
  const size_t dimension = v.size();
  KDTree k(dimension);
  
  vp.push_back(v.data());
  Cluster c = Cluster(vp, dimension);
  
  k.insert(c);
  clusts.push_back(c);
  
  v.clear();
  vp.clear();
  
  do{
    if (cin >> input){
      v.push_back(input);
    }
    if(cin.peek() == '\n'){
      if(v.size() != dimension){
	cout << v.size();
	cout << "FATAL ERROR: DIMENSIONS VARY! TERMINATING..." << endl;
	exit(1);
      }
      vp.push_back(v.data());
      Cluster current = Cluster(vp, dimension);
      k.insert(current);
      clusts.push_back(current);
    
      v.clear();
      vp.clear();
    }
  }while(cin && cin.peek() != EOF);


  // Can't have more clusters than intial vectors
  if(numClusters <= 0 || numClusters > k.size()){
    cout << "Invalid number of clusters!" << endl;
    exit(1);
  }
  
  
  /* KD TREE SHOULD BE POPULATED WITH THE INPUT AT THIS POINT...*/
  for(size_t t = 0; t < clusts.size(); ++t){
    Cluster b = k.find_best_match(clusts.at(t));
    min_heap.emplace(clusts.at(t).euclidian_distance(b),make_pair(Cluster(clusts.at(t)), Cluster(b)));
  }
  
  while(k.size() > numClusters){
    pair <Cluster, Cluster> x  = min_heap.top().second;
    
    if(! k.contains(x.first)){min_heap.pop();} // A was already clustered
    else if(! k.contains(x.second)){
      //B is invalid, find best match for A
      x.second = k.find_best_match(x.first);
      min_heap.pop();
      min_heap.push(make_pair(x.first.euclidian_distance(x.second),
			      make_pair(Cluster(x.first), Cluster(x.second))));
    }else{
      Cluster c = Cluster(x.first, x.second);
      k.insert(c);
      k.remove(x.first);
      k.remove(x.second);
      Cluster d = k.find_best_match(c);
      min_heap.pop();
      min_heap.emplace(c.euclidian_distance(d), make_pair(Cluster(c), Cluster(d)));
    }
  }

  k.print();
  return 0;
}

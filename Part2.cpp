#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <utility>
#include <math.h>
#include "KDTree.hpp"

using namespace std;

class CompareDist{public: bool operator()(pair<double, pair<Cluster, Cluster> >a, pair<double, pair<Cluster, Cluster> >b){return a.first > b.first;};};

int main(int argc, char* argv[]){
/* Place vectors from stdin into one intial cluster*/
  int unsigned numClusters;
  double input;
  vector<double> v;
  vector<double*> vp;
  vector<Cluster> clusts = vector<Cluster>(0);
  string line;

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
  
  Cluster A = clusts.at(0);
  Cluster B = k.pairwise_best_match(A);
    
  /* KD TREE SHOULD BE POPULATED WITH THE INPUT AT THIS POINT...*/
  while(k.size() > numClusters){
    Cluster C = k.pairwise_best_match(B);
    if(A == C){
      Cluster temp = Cluster(A, B, true);
      k.insert(temp); 
      k.remove(A);
      k.remove(B);
      B = k.pairwise_best_match(A);
    }else{
      A = B;
      B = C;
    }
  }
  cout << "Resulting clusters:" << endl; 
  k.print();
  return 0;
}

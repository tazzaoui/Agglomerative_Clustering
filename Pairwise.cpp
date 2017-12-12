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
  int unsigned numClusters = 1;
  double input;
  vector<double> v;
  vector<double*> vp;
  vector<Cluster> clusts = vector<Cluster>(0);
  string line;

  vector<string> cmdArgs = vector<string>(argv, argv + argc);
  
  for(size_t i = 0; i < cmdArgs.size(); ++i){
    if(argc < 2 || cmdArgs[i] == "-h" || cmdArgs[i] == "--help"){
      cout << "Usage: euclid [-h] [-c CLUSTERS]" << endl;
      cout << "-h, --help:     show this help message and exit." << endl;
      cout << "-c, --clusters: set the desired number of clusters." 
	   << " Must be a number greater than 0 and less than the "
	   << "number of initial vectors provided. Default = 1." << endl;
      exit(0);
    }if(cmdArgs[i] == "-c"|| cmdArgs[i] == "--clusters"){
      if(i + 1 < cmdArgs.size()) numClusters = atoi(cmdArgs[i + 1].c_str());
    }
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

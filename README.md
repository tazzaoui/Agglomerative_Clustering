# Agglomerative Clustering : 
#### A method for producing a hierarchical tree of clusters in a process that  successively merges clusters with each other to produce larger clusters

The program reads k individual clusters from stdin and successively merges the most similar clusters until the desired number of clusters is achieved.

#### Note: We define the distance between two vectors as the standard Euclidian distance. 
### Euclidian implementation:
* The distance between two clusters is the distance between their centroid vectors (vector of component-wise averages).
### Pairwise implementation:
* The distance between two clusters is the sum of the distances between each pair of vectors.

  
In implementing an agglomerative clustering algorithm, we can break down our problem into to main subproblems...
  
    
### Problem One: How do we store our vectors effectively? 
	
We can store each cluster in a multidimensional binary search tree where every node in the tree is an n-dimensional vector. This is analogous to a standard 1-dimensional BST, but with some subtle extensions. The tree is partitioned in such a way that every node (excluding the leaves) splits the n-dimensional space with respect to a given dimension. Concretely, if a node is  y-justified, then the y dimension of the root of its left subtree is less than its y-dimension. The inverse is true for the node of its right subtree. The justification of each level repeats after the levels of the tree exceed the dimensions of our space. In other words, in a tree with 6 levels (63 nodes) containing points in 3 dimensional space, the root will be x-justified, the root’s left and right subtrees (level 2) will be y-justified, and each of their subtrees (level 3) will be z-justified. This process restarts, and the level 3 nodes’ children will be x-justified, their children will be y-justified, and so on. When we insert an element into the tree, we traverse the nodes, comparing the  corresponding dimensions of each node. This structure leads to some interesting properties. Namely, just as we would expect with a binary search tree, we can search, insert, and delete in logarithmic time on average.
  
    
 Now that we have an effective way to store our vectors, we need an efficient method of computing distances...
  
    
### Problem Two: How can we optimize the distance calculations between clusters?

One answer to this question lies in our solution for problem one. The n-dimensional binary search tree turns out to be exceptional when it comes to preforming nearest-neighbor searches. This is due to the locality and of the nearest neighbors in a tree. With this in mind, we can use a reverse priority queue or a min-heap to store a  tuple containing <cluster U, cluster V, distance(U ,V)>. By the min-heap property, we now have a method of storing corresponding clusters by distance in ascending order. 

After these two subproblems are solved, the implementation becomes trivial. Empirical analysis suggests a sub-quadratic, nearly linear runtime (Wilson et al.).

### Resources:
http://www.graphics.cornell.edu/~bjw/IRT08Agglomerative.pdf  
https://www.cs.cmu.edu/~ckingsf/bioinfo-lectures/kdtrees.pdf

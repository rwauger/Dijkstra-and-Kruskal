/*
 * Name: Ryan Auger
 * Date Submitted: 3-28-2019
 * Lab Section:
 * Assignment Name: Graph Algorithms
 */

#pragma once

#include "heap.h"

typedef string node;
// this is a struct that contains a node and its distance for the add_edge
// function and dijkstra's algorithm
typedef struct{
  node current_node;
  int distance;
} nodes;

// this struct contains 2 nodes and the distance between them
// for kruskals algorithm
typedef struct{
  node node1;
  node node2;
  int distance;
} kruskal_edges;

class graphalgs{
  private:
    map<node, vector<nodes>> m;
    map<node, int> distance_table;
    list<kruskal_edges> edge_list;
    list<kruskal_edges> min_span_tree;
    map<node, node> disjoint_set;

  public:
    void add_edge(node, node, int);
    void dijkstra(node);
    void kruskal();
    node find_parent(node);
    void unify(node, node);
};

//adds two nodes that are directly connected and their distances
//all edges will be added before dijkstra and kruskal are called
void graphalgs::add_edge(node node1, node node2, int distance){
  nodes _node;
  kruskal_edges kruskal_edge;

  // this section creates a edge list for kruskal's algorithm
  kruskal_edge.node1 = node1;
  kruskal_edge.node2 = node2;
  kruskal_edge.distance = distance;
  edge_list.push_back(kruskal_edge);

  // the next two sections of code add the nodes and edges to a map
  // going both ways
  _node.current_node = node2;
  _node.distance = distance;
  m[node1].push_back(_node);

  _node.current_node = node1;
  _node.distance = distance;
  m[node2].push_back(_node);
}

//Calculates the minimum distance from the <sourceNode>
//to all other nodes and prints
void graphalgs::dijkstra(node sourceNode) {
  // this creates a min_heap from the heap that was provided
  heap<pair<node, int>> min_heap;
  int i;

  // goes through and initializes all of the nodes values in the distance
  // table to 0
  for (auto i : m) {
    distance_table[i.first] = INT_MAX;
  }

  // I got a some of the idea for the next part from geeks for geeks and
  // also from the ta's explanation of the process of dijkstra's algorithm
  // which lined up with what geeks for geeks was saying

  // the source node will always be 0 so we go ahead and add that to the
  // distance table and min_heap
  min_heap.push(make_pair(sourceNode, 0));
  distance_table[sourceNode] = 0;

  // while the min_heap is not empty we want to stay in this while loop.
  while (!min_heap.empty()) {
    // access the front of the min_heap and store it into a temporary variable
    node current_node = min_heap.front().first;
    // remove the pair from the front of the heap
    min_heap.pop();
    // iterate through the vector of that node we just got from the heap so we
    // can access its adjacent nodes and their distances from the current node
    for (i = 0; (unsigned int)i < m[current_node].size(); i++) {
      // children is the adjacent node of the current node
      node children = m[current_node][i].current_node;
      int distance =  m[current_node][i].distance;

      // check to see if that distance of the current node plus the distance
      // of the adjecent node is less then the distance we have on record
      // for the adjacent node in the distance table and if it is
      // then we go ahead and update that distance in the distance table and
      // push that adjacent node and its updated distance to the min_heap
      if ((distance_table[current_node] + distance) <
        distance_table[children]) {
          distance_table[children] = distance_table[current_node] + distance;
          min_heap.push(make_pair(children, distance_table[children]));
      }
    }
  }

  // we want to remove the source node
  distance_table.erase(sourceNode);

  // the rest is just to print to the terminal
  cout << "Dijkstra (from " << sourceNode << ")" << endl;

  for (auto i : distance_table) {
    cout << i.first << ": " << i.second << endl;
  }
  cout << endl;
}

//Generates a minimum spanning tree and prints
void graphalgs::kruskal() {
  int total_weight = 0;
  edge_list.sort();
  for (auto i : m) {
    disjoint_set[i.first] = i.first;
  }

  for (auto i : edge_list) {
    // this finds the parent of both the nodes that we grabbed from the edge
    // list
    node parent1 = find_parent(i.node1);
    node parent2 = find_parent(i.node2);

    // if parent1 and parent2 are the same then we are trying to draw an edge
    // inside of the same set which means there is a cycle in this graph
    // and we do not want to add to the minimum spanning tree
    if (parent1 == parent2)
      continue;
    else{
      // push onto the minimum spanning tree
      min_span_tree.push_back(i);
      // add the weights of all the edges
      total_weight += i.distance;
      // call to the combine the two sets
      unify(i.node1, i.node2);
    }
  }

  // this is just to print the minimum spanning tree to the terminal
  cout << "Kruskal" << endl;
  for (auto i : min_span_tree) {
    cout << i.node1 << ", " << i.node2 << ", " << i.distance << endl;
  }
  cout << "total weight = " << total_weight << endl;
  cout << endl;
}

// operator overload function for <
// this is a variation of what is on geeks for geeks
bool operator<(kruskal_edges edge_distance1, kruskal_edges edge_distance2) {
  return edge_distance1.distance < edge_distance2.distance;
}

// this function is to find the parent/root of the set which is in a map of
// sets and return it using recursion
node graphalgs::find_parent(node current_node) {
  // Base Case
  if (disjoint_set[current_node] == current_node){
    return current_node;
  }
  //this is where the recursion happens
  node found = find_parent(disjoint_set[current_node]);
  return found;
}

// this function is used to combine two sets into one
void graphalgs::unify(node node1, node node2) {
  node parent1 = find_parent(node1);
  node parent2 = find_parent(node2);

  disjoint_set[parent1] = parent2;
}

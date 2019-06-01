#include <chrono>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "GraphVisualizer.h"
using namespace std;

namespace {
  const double kPi = 3.14159265358979323;
  const double kRepel = 0.001;
  const double kAttract = 0.001;
}

// Convenience function to add edges to the graph.
void AddEdge(std::vector<int>& graph, int node_1, int node_2) {
  graph[node_1] = node_2;
  graph[node_2] = node_1;
}

// Initializes the graph from a text file.
void InitGraph(fstream& graph_file, SimpleGraph& simple_graph) {
  int num_nodes{0};
  std::string line;
  if (graph_file.is_open())
  {
    int i = 0;
    while (std::getline(graph_file,line))
    {
      std::istringstream iss(line);
      int node_1 , node_2;
      // The first line is the total number of nodes in the graph. The rest of the graph file decribes the nodes that
      // are connected.
      if (i == 0) {
        // Get total number of nodes.
        num_nodes = std::stoi(line);
        simple_graph.nodes.resize(num_nodes);
      } else {
        // Adding edges between nodes.
        iss >> node_1 >> node_2;
        Edge new_edge;
        new_edge.start = node_1;
        new_edge.end = node_2;
        simple_graph.edges.push_back(new_edge);
      }      
      i++;
    }
    graph_file.close();
  } else {
    std::cout << "no file." << std::endl;
  }
}

int main() {
  InitGraphVisualizer();
  SimpleGraph simple_graph;

  // Building the adjeacency list based on reading in graph file.
  fstream graph_file;
  graph_file.open("res/63binary-tree");
  InitGraph(graph_file, simple_graph);

  // Timing stuff.
  std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = t1-t0;

  double f_attract{0.0};
  double f_repel{0.0};
  double theta{0.0};

  // Initialize position of nodes in a circle.
  int k = 1;
  for (auto& element : simple_graph.nodes) {
    element.x = cos((2*kPi*k)/simple_graph.nodes.size());
    element.y = sin((2*kPi*k)/simple_graph.nodes.size());
    k++;
  }

  std::cout << "doing stuff" << std::endl;
  while (elapsed <= std::chrono::duration<double>(8.0)) {
    f_attract = 0.0;
    f_repel = 0.0;
    theta = 0.0;

    std::vector<double> delta_x(simple_graph.nodes.size());
    std::vector<double> delta_y(simple_graph.nodes.size());

    // For each pair of nodes calculate the repulsive forces.
    for (size_t i = 0; i < simple_graph.nodes.size(); i++) {
      for (size_t j = 0; j < simple_graph.nodes.size() - 1; j++) {
        if (i != j) {
          f_repel = kRepel / sqrt(std::pow(simple_graph.nodes[i].y - simple_graph.nodes[j].y, 2) +
                    std::pow(simple_graph.nodes[i].x - simple_graph.nodes[j].x, 2));
          theta = atan2(simple_graph.nodes[i].y - simple_graph.nodes[j].y, simple_graph.nodes[i].x -
                  simple_graph.nodes[j].x);
          delta_x[j] -= f_repel*std::cos(theta);
          delta_y[j] -= f_repel*std::sin(theta);
          delta_x[i] += f_repel*std::cos(theta);
          delta_y[i] += f_repel*std::sin(theta);
        }
      } 
    }

    // For each edge calculate the edge attractive forces.
    for (auto& edge : simple_graph.edges) {
      f_attract = kAttract * (std::pow(simple_graph.nodes[edge.start].y - simple_graph.nodes[edge.end].y, 2) +
                  std::pow(simple_graph.nodes[edge.start].x - simple_graph.nodes[edge.end].x, 2));
      theta = atan2(simple_graph.nodes[edge.start].y - simple_graph.nodes[edge.end].y, simple_graph.nodes[edge.start].x
              - simple_graph.nodes[edge.end].x);
      delta_x[edge.end] += f_attract*std::cos(theta);
      delta_y[edge.end] += f_attract*std::sin(theta);
      delta_x[edge.start] -= f_attract*std::cos(theta);
      delta_y[edge.start] -= f_attract*std::sin(theta);
    }

    // Loop through all the nodes and move the x and y positions.
    k = 0;
    for (auto& node : simple_graph.nodes) {
      node.x += delta_x[k];
      node.y += delta_y[k];
      k++;
    }

    DrawGraph(simple_graph);
    t1 = std::chrono::high_resolution_clock::now();
    elapsed = t1-t0;
  }

  std::cout << "Elapsed time: " << elapsed.count() << std::endl;

  return 0;
}
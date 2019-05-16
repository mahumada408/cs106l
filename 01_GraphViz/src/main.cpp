#include <chrono>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GraphVisualizer.h"
using namespace std;

namespace {
  const double kPi = 3.14159265358979323;
}

void Welcome() {
	cout << "Welcome to CS106L GraphViz!" << endl;
	cout << "This program uses a force-directed graph layout algorithm" << endl;
  cout << "to render sleek, snazzy pictures of various graphs." << endl;
  cout << endl;
}

int main() {
	Welcome();

  int num_nodes{0};

  // Reading in graph file.
  std::string line;
  fstream graph_file;
  graph_file.open("res/2line");
  if (graph_file.is_open())
  {
    int i = 0;
    while (std::getline(graph_file,line))
    {
      if (i == 0) {
        num_nodes = std::stoi(line);
      }
      std::cout << line << std::endl;
      i++;
    }
    graph_file.close();
  } else {
    std::cout << "no file." << std::endl;
  }
  graph_file.close();

  std::cout << "Number of nodes: " << num_nodes << std::endl;

  // Initialize position of nodes
  std::vector<Node> nodes(num_nodes);
  int k = 1;
  for (auto element : nodes) {
    element.x = cos((2*kPi*k)/num_nodes);
    element.y = sin((2*kPi*k)/num_nodes);
    std::cout << "Assgined new node position for node: " << k << std::endl;
    k++;
  }
  
  std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = t1-t0;

  std::cout << "doing stuff" << std::endl;
  while (elapsed <= std::chrono::duration<double>(1.0)) {
    t1 = std::chrono::high_resolution_clock::now();
    elapsed = t1-t0;
  }

  
  std::cout << "Elapsed time: " << elapsed.count() << std::endl;

  return 0;
}
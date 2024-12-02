#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

typedef map<int, vector<int>> graph;
void readFile(graph &graph, const string &inputFileName) {
  ifstream inputFile(inputFileName);
  if (!inputFile.is_open()) {
    cerr << "Nie udalo sie otworzyc pliku " << inputFileName << endl;
    exit(1);
  }

  string line;
  while (getline(inputFile, line)) {
    istringstream iss(line);
    int node;
    char colon;
    iss >> node >> colon;

    if (colon != ':') {
      cerr << "Niepoprawny format pliku" << endl;
      exit(1);
    }

    vector<int> connections;
    string neighbors;
    getline(iss, neighbors);
    istringstream neighborsStream(neighbors);
    int neighbor;
    while (neighborsStream >> neighbor) {
      connections.push_back(neighbor);
      if (neighborsStream.peek() == ',') {
        neighborsStream.ignore();
      }
    }

    graph[node] = connections;
  }

  inputFile.close();
}
void writeFile(graph &graph, const string &outputFileName) {
  ofstream outputFile(outputFileName);
  if (!outputFile.is_open()) {
    cerr << "Error: Unable to open file " << outputFileName << endl;
    exit(1);
  }

  for (const auto &pair : graph) {
    outputFile << pair.first << ": ";
    const auto &neighbors = pair.second;
    for (int i = 0; i < neighbors.size(); ++i) {
      outputFile << neighbors[i];
      if (i < neighbors.size() - 1) {
        outputFile << ", ";
      }
    }
    outputFile << endl;
  }

  outputFile.close();
}
bool isOneGraph(graph &graph) { return true; }
bool isAdjoint(graph &graph) { return true; }
bool isLinear(graph &graph) { return true; }
void transform(graph &inputGraph, graph &outputGraph) {
  outputGraph = inputGraph;
};

int main() {
  graph inputGraph, outputGraph;
  readFile(inputGraph, "input.txt");

  if (!isOneGraph(inputGraph) || !isAdjoint(inputGraph)) {
    cerr << "Podany graf nie jest sprzezony!\n";
    return 1;
  }

  cout << "Podany graf jest sprzezony"
       << (isLinear(inputGraph) ? " oraz liniowy.\n" : ", ale nie liniowy.\n");

  transform(inputGraph, outputGraph);
  writeFile(outputGraph, "outputHubert.txt");

  return 0;
}
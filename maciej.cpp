#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
using namespace std;

typedef map<int, vector<int>> graph;
int adj_check(graph &adj) {
  for (auto it1 = adj.begin(); it1 != adj.end(); ++it1) {
    for (auto it2 = next(it1); it2 != adj.end(); ++it2) {

      vector<int> neighbors1 = it1->second;
      vector<int> neighbors2 = it2->second;

      // Sortowanie sąsiadów, aby ułatwić porównanie
      sort(neighbors1.begin(), neighbors1.end());
      sort(neighbors2.begin(), neighbors2.end());

      if (neighbors1 == neighbors2) {
        // Następniki są identyczne - kontynuujemy
        continue;
      } else {
        // Sprawdzamy, czy zbiory są całkowicie różne
        bool found_common = false;
        auto iter1 = neighbors1.begin();
        auto iter2 = neighbors2.begin();

        while (iter1 != neighbors1.end() && iter2 != neighbors2.end()) {
          if (*iter1 == *iter2) {
            found_common = true;
            break;
          } else if (*iter1 < *iter2) {
            ++iter1;
          } else {
            ++iter2;
          }
        }

        // Jeśli znaleziono wspólny element, graf nie jest sprzężony
        if (found_common) {
          cout << "Wierzcholek " << it1->first << " i wierzcholek "
               << it2->first << " mają wspólny element.\n";
          cout << "Graf nie jest sprzężony.\n";
          return 0;
        }
      }
    }
  }

  cout << "Graf jest sprzężony.\n";
  return 1;
}
void linear_check(graph &lin) {
  int endpoint = 0;

  for (auto &pair : lin) {
    int neighbor_size = pair.second.size();

    if (neighbor_size == 1) {
      endpoint++;
    } else if (neighbor_size != 2) {
      cout << "Graf nie jest liniowy" << "\n";
      return;
    }
  }

  if (endpoint == 2) {
    cout << "Graf jest liniowy" << "\n";
  } else {
    cout << "Graf nie jest liniowy" << "\n";
  }
}
void original(graph &original, map<int, pair<int, int>> &edges,
              graph &newGraph) {
  int edge_id = 1;

  // Tworzenie krawędzi w oryginalnym grafie jako nowy wierzchołek w nowym
  // grafie
  for (auto it = original.begin(); it != original.end(); ++it) {
    int start = it->first;
    for (int end : it->second) {
      if (start < end) { // Aby uniknąć dodawania krawędzi dwukrotnie
        edges[edge_id] = make_pair(start, end);
        edge_id++;
      }
    }
  }

  // Zestaw do śledzenia przetworzonych par krawędzi, aby uniknąć zapętlenia
  set<pair<int, int>> processed_edges;

  // Dodawanie połączeń w nowym grafie bazując na wspólnych wierzchołkach
  for (auto it = edges.begin(); it != edges.end(); ++it) {
    int edgeId = it->first;
    pair<int, int> vertexPair = it->second;
    int start = vertexPair.first;
    int end = vertexPair.second;

    for (auto otherIt = edges.begin(); otherIt != edges.end(); ++otherIt) {
      int otherEdgeId = otherIt->first;

      // Sprawdzenie, czy to ta sama krawędź, lub czy krawędź została już
      // przetworzona
      if (edgeId != otherEdgeId &&
          processed_edges.find({edgeId, otherEdgeId}) ==
              processed_edges.end()) {
        pair<int, int> otherVertexPair = otherIt->second;

        // Sprawdzanie wspólnego wierzchołka między krawędziami
        if (start == otherVertexPair.first || start == otherVertexPair.second ||
            end == otherVertexPair.first || end == otherVertexPair.second) {

          // Dodajemy połączenie i oznaczamy je jako przetworzone
          newGraph[edgeId].push_back(otherEdgeId);
          processed_edges.insert({edgeId, otherEdgeId});
          processed_edges.insert({otherEdgeId, edgeId});
        }
      }
    }
  }

  // Wyświetlenie nowego grafu, wykorzystując ID krawędzi jako połączenia
  ofstream file_output("new_graph.txt");

  cout << "Nowy graf:\n";
  for (auto it = newGraph.begin(); it != newGraph.end(); ++it) {
    cout << it->first << ": ";
    file_output << it->first << ": ";

    vector<int> &neighbors = it->second;
    for (size_t j = 0; j < neighbors.size(); ++j) {
      cout << neighbors[j];
      file_output << neighbors[j];
      if (j < neighbors.size() - 1) {
        cout << ", ";
        file_output << ", ";
      }
    }
    cout << "\n";
    file_output << "\n";
  }
  file_output.close();
}

int main() {
  ifstream file("graph.txt");
  if (!file.is_open()) {
    cout << "Nie udalo się otworzyc pliku";
    return 1;
  }

  graph list;

  string line;
  while (getline(file, line)) {
    stringstream split_line(line);
    int ver_num;
    char spacer;

    split_line >> ver_num >> spacer;

    int neighbor;
    vector<int> neighbors;

    while (split_line >> neighbor) {
      neighbors.push_back(neighbor);
    }

    list[ver_num] = neighbors;
  }

  map<int, pair<int, int>> edges;
  graph newGraph;

  if (adj_check(list)) {
    linear_check(list);
    original(list, edges, newGraph);
  }

  file.close();
  return 0;
}

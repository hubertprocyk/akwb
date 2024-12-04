#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

typedef unordered_map<int, vector<int>> graph;
void readFile(graph &graph, const string &filename) {
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Nie mozna otworzyc pliku: " << filename << endl;
		exit(1);
	}

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		int vertex;
		char colon;
		iss >> vertex >> colon;

		vector<int> successors;
		int successor;
		while (iss >> successor) {
			successors.push_back(successor);
		}

		graph[vertex] = successors;
	}

	file.close();
}
void writeFile(graph &graph, const string &filename) {
	ofstream file(filename);

	if (!file.is_open()) {
		cerr << "Nie mozna otworzyc pliku: " << filename << endl;
		return;
	}

	for (const auto &pair : graph) {
		file << pair.first << " :";
		for (int successor : pair.second) {
			file << " " << successor;
		}
		file << endl;
	}

	file.close();
}
bool isOneGraph(graph &graph) {
	for (auto &pair : graph) {
		vector<int> &successors = pair.second;
		sort(successors.begin(), successors.end());

		for (size_t i = 1; i < successors.size(); ++i) {
			if (successors[i] == successors[i - 1]) {
				return false;
			}
		}
	}
	return true;
}
bool isAdjoint(graph &graph) {
	for (auto it1 = graph.begin(); it1 != graph.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != graph.end(); ++it2) {
			vector<int> &succ1 = it1->second;
			vector<int> &succ2 = it2->second;

			sort(succ1.begin(), succ1.end());
			sort(succ2.begin(), succ2.end());

			vector<int> intersection;
			set_intersection(succ1.begin(), succ1.end(),
			                 succ2.begin(), succ2.end(),
			                 back_inserter(intersection));

			if (!intersection.empty() && intersection.size() != succ1.size() && intersection.size() != succ2.size()) {
				return false;
			}
		}
	}
	return true;
}
bool isLinear(graph &graph) {
	for (const auto &pair : graph) {
		int u = pair.first;
		const std::vector<int> &succ = pair.second;

		if (succ.size() >= 2) {
			for (size_t i = 0; i < succ.size(); ++i) {
				for (size_t j = i + 1; j < succ.size(); ++j) {
					int v1 = succ[i], v2 = succ[j];
					if (graph.count(v1) && graph.count(v2) && !graph[v1].empty() && !graph[v2].empty()) {
						if (graph[v1][0] == graph[v2][0]) {
							return false;
						}
					}
				}
			}

			for (int v : succ) {
				if (find(graph[v].begin(), graph[v].end(), u) != graph[v].end()) {
					return false;
				}
			}

			if (find(succ.begin(), succ.end(), u) != succ.end()) {
				for (int v : succ) {
					if (v != u && find(graph[v].begin(), graph[v].end(), u) != graph[v].end() &&
					        find(graph[v].begin(), graph[v].end(), v) != graph[v].end()) {
						return false;
					}
				}
			}
		}
	}

	return true;
}
void transform(graph &inputGraph, graph &outputGraph) {
    // Mapowanie wierzchołków grafu G na łuki w grafie H
    unordered_map<int, pair<int, int>> vertexToArc;
    int nextIndex = 1;

    // Tworzymy początkowy zbiór rozłącznych łuków w H
    for (const auto &[u, successors] : inputGraph) {
        vertexToArc[u] = {nextIndex, nextIndex + 1};
        outputGraph[nextIndex] = {nextIndex + 1};
        nextIndex += 2;
    }

    // Kompresujemy wierzchołki w H na podstawie połączeń w G
    for (const auto &[u, successors] : inputGraph) {
        int arcEnd = vertexToArc[u].second;
        for (int v : successors) {
            int arcStart = vertexToArc[v].first;

            // Kompresujemy arcEnd i arcStart w H
            for (auto &[_, adj] : outputGraph) {
                for (int &w : adj) {
                    if (w == arcStart) {
                        w = arcEnd;
                    }
                }
            }
        }
    }
}

int main() {
	string inputFilename = "input.txt";
	string outputFilename = "output.txt";

	graph inputGraph, outputGraph;
	readFile(inputGraph, inputFilename);

    if (!isOneGraph(inputGraph) || !isAdjoint(inputGraph)) {
        cerr << "Podany graf nie jest sprzezony!\n";
        return 1;
    }

    transform(inputGraph, outputGraph);
    cout << "Wynikowy graf jest sprzezony"
         << (isLinear(outputGraph) ? " oraz liniowy.\n" : ", ale nie liniowy.\n");

	writeFile(outputGraph, outputFilename);
	cout << "Graf zapisany do pliku: " << outputFilename << endl;

	return 0;
}

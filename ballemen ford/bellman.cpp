
// ============================================================================
// Bellman-Ford Algorithm
// ============================================================================
// Finds shortest paths from a source vertex to all other vertices
// Works with negative weights (unlike Dijkstra)
// Detects negative weight cycles
// Time Complexity: O(V * E) where V = vertices, E = edges
// Space Complexity: O(V)

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

// ============================================================================
// EDGE STRUCTURE
// ============================================================================

struct Edge {
    int source;      // Starting vertex of edge
    int destination; // Ending vertex of edge
    int weight;      // Weight of the edge
};

// ============================================================================
// BELLMAN-FORD ALGORITHM
// ============================================================================

class Graph {
private:
    int vertices;           // Number of vertices
    vector<Edge> edges;     // List of all edges

public:
    Graph(int v) : vertices(v) {}

    // Add an edge to the graph
    void addEdge(int src, int dest, int weight) {
        Edge edge = {src, dest, weight};
        edges.push_back(edge);
    }

    // Main Bellman-Ford algorithm
    void bellmanFord(int source) {
        // Step 1: Initialize distances array
        // Set all distances to infinity except source (0)
        vector<int> distance(vertices, INT_MAX);
        distance[source] = 0;

        // Step 2: Relax edges (V-1) times
        // Relaxation: if path through current edge is shorter, update distance
        for (int i = 0; i < vertices - 1; i++) {
            // Check each edge
            for (const Edge& edge : edges) {
                int u = edge.source;
                int v = edge.destination;
                int weight = edge.weight;

                // If source is reachable and path through this edge is shorter
                if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                }
            }
        }

        // Step 3: Check for negative weight cycles
        // If we can still relax an edge, there's a negative cycle
        bool hasNegativeCycle = false;
        for (const Edge& edge : edges) {
            int u = edge.source;
            int v = edge.destination;
            int weight = edge.weight;

            if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                hasNegativeCycle = true;
                break;
            }
        }

        // Step 4: Print results
        if (hasNegativeCycle) {
            cout << "Graph contains a negative weight cycle!\n";
            return;
        }

        cout << "Vertex distances from source " << source << ":\n";
        for (int i = 0; i < vertices; i++) {
            if (distance[i] == INT_MAX)
                cout << "Vertex " << i << ": INF (unreachable)\n";
            else
                cout << "Vertex " << i << ": " << distance[i] << "\n";
        }
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== Bellman-Ford Algorithm ===\n\n";

    // Test case 1: Simple graph with positive weights
    cout << "Test 1: Simple graph\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges: 0->1(4), 0->2(2), 1->2(1), 1->3(5), 2->3(8), 2->1(2)\n\n";

    Graph g1(4);
    g1.addEdge(0, 1, 4);
    g1.addEdge(0, 2, 2);
    g1.addEdge(1, 2, 1);
    g1.addEdge(1, 3, 5);
    g1.addEdge(2, 3, 8);
    g1.addEdge(2, 1, 2);

    g1.bellmanFord(0);
    cout << "\n";

    // Test case 2: Graph with negative weights (but no negative cycle)
    cout << "Test 2: Graph with negative weights\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges: 0->1(4), 0->2(2), 1->2(-3), 1->3(2), 3->2(5)\n\n";

    Graph g2(4);
    g2.addEdge(0, 1, 4);
    g2.addEdge(0, 2, 2);
    g2.addEdge(1, 2, -3);
    g2.addEdge(1, 3, 2);
    g2.addEdge(3, 2, 5);

    g2.bellmanFord(0);
    cout << "\n";

    // Test case 3: Graph with negative cycle
    cout << "Test 3: Graph with negative cycle\n";
    cout << "Vertices: 0, 1, 2\n";
    cout << "Edges: 0->1(1), 1->2(3), 2->1(-5)\n";
    cout << "(Cycle: 1->2->1 with weight 3+(-5) = -2, negative!)\n\n";

    Graph g3(3);
    g3.addEdge(0, 1, 1);
    g3.addEdge(1, 2, 3);
    g3.addEdge(2, 1, -5);

    g3.bellmanFord(0);

    return 0;
}

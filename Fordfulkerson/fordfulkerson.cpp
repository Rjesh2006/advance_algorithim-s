
// ============================================================================
// Ford-Fulkerson Algorithm - Maximum Flow Problem
// ============================================================================
// Finds maximum flow from source to sink in a weighted directed graph
// Uses DFS (Depth-First Search) to find augmenting paths
// Can also use BFS for Edmonds-Karp algorithm (better time complexity)
// Time Complexity: O(V * E²) with DFS, O(V * E³) best guarantee
// Space Complexity: O(V²) for adjacency matrix

#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

// ============================================================================
// FORD-FULKERSON WITH DFS
// ============================================================================

class FordFulkerson {
private:
    int vertices;                    // Number of vertices
    vector<vector<int>> capacity;    // Capacity matrix (original capacities)
    vector<vector<int>> residual;    // Residual matrix (remaining capacities)

    // DFS to find augmenting path from source to sink
    bool dfs(int source, int sink, int parent[], vector<vector<int>>& residualGraph) {
        vector<bool> visited(vertices, false);
        queue<int> q;

        q.push(source);
        visited[source] = true;

        // Find path from source to sink using DFS approach via adjacency
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // Check all adjacent vertices
            for (int v = 0; v < vertices; v++) {
                // If not visited and has capacity available
                if (!visited[v] && residualGraph[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);

                    // If sink is reached, return true
                    if (v == sink)
                        return true;
                }
            }
        }

        return false; // No path found
    }

public:
    FordFulkerson(int v) : vertices(v) {
        capacity.assign(v, vector<int>(v, 0));
        residual.assign(v, vector<int>(v, 0));
    }

    // Add edge from source to destination with given capacity
    void addEdge(int source, int dest, int cap) {
        capacity[source][dest] += cap;
    }

    // Main Ford-Fulkerson algorithm
    int maxFlow(int source, int sink) {
        // Initialize residual graph with capacity values
        residual = capacity;

        int maxFlowValue = 0;      // Total maximum flow
        int parent[vertices];       // Store path
        
        cout << "Finding augmenting paths:\n";
        cout << "=========================\n";

        // While there exists an augmenting path from source to sink
        int pathNum = 1;
        while (dfs(source, sink, parent, residual)) {
            cout << "\nPath " << pathNum << ": ";

            // Find minimum capacity along the path
            int pathFlow = INT_MAX;
            int v = sink;
            vector<int> path;

            // Traverse from sink to source using parent array
            while (v != source) {
                int u = parent[v];
                pathFlow = min(pathFlow, residual[u][v]);
                path.push_back(v);
                v = u;
            }
            path.push_back(source);

            // Print path
            cout << source;
            for (int i = path.size() - 2; i >= 0; i--) {
                cout << " -> " << path[i];
            }
            cout << " (Capacity: " << pathFlow << ")\n";

            // Update residual capacities of edges and reverse edges
            v = sink;
            while (v != source) {
                int u = parent[v];
                residual[u][v] -= pathFlow;      // Forward edge
                residual[v][u] += pathFlow;      // Reverse edge (for backflow)
                v = u;
            }

            // Add path flow to total flow
            maxFlowValue += pathFlow;
            pathNum++;
        }

        cout << "\n";
        return maxFlowValue;
    }

    // Print residual graph
    void printResidualGraph() {
        cout << "Residual Graph (Remaining Capacities):\n";
        cout << "======================================\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (residual[i][j] > 0)
                    cout << "Edge " << i << "->" << j << ": " << residual[i][j] << "\n";
            }
        }
        cout << "\n";
    }

    // Print capacity graph
    void printCapacityGraph() {
        cout << "Original Capacity Graph:\n";
        cout << "=======================\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (capacity[i][j] > 0)
                    cout << "Edge " << i << "->" << j << ": " << capacity[i][j] << "\n";
            }
        }
        cout << "\n";
    }

    // Get flow on a specific edge
    int getFlow(int source, int dest) {
        return capacity[source][dest] - residual[source][dest];
    }

    // Print final flow on all edges
    void printFinalFlow() {
        cout << "Final Flow on Each Edge:\n";
        cout << "=======================\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int flow = getFlow(i, j);
                if (flow > 0) {
                    cout << "Edge " << i << "->" << j 
                         << ": " << flow << "/" << capacity[i][j] << "\n";
                }
            }
        }
        cout << "\n";
    }
};

// ============================================================================
// EDMONDS-KARP (Ford-Fulkerson with BFS)
// ============================================================================
// Uses BFS instead of DFS for better time complexity

class EdmondsKarp {
private:
    int vertices;
    vector<vector<int>> capacity;
    vector<vector<int>> residual;

    // BFS to find shortest augmenting path
    bool bfs(int source, int sink, int parent[]) {
        vector<bool> visited(vertices, false);
        queue<int> q;

        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < vertices; v++) {
                // If not visited and has residual capacity
                if (!visited[v] && residual[u][v] > 0) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);

                    if (v == sink)
                        return true;
                }
            }
        }

        return false;
    }

public:
    EdmondsKarp(int v) : vertices(v) {
        capacity.assign(v, vector<int>(v, 0));
        residual.assign(v, vector<int>(v, 0));
    }

    void addEdge(int source, int dest, int cap) {
        capacity[source][dest] += cap;
    }

    // Main Edmonds-Karp algorithm
    int maxFlow(int source, int sink) {
        residual = capacity;

        int maxFlowValue = 0;
        int parent[vertices];
        
        cout << "Finding augmenting paths (using BFS):\n";
        cout << "====================================\n";

        int pathNum = 1;
        while (bfs(source, sink, parent)) {
            cout << "\nPath " << pathNum << ": ";

            // Find minimum capacity along path
            int pathFlow = INT_MAX;
            int v = sink;
            vector<int> path;

            while (v != source) {
                int u = parent[v];
                pathFlow = min(pathFlow, residual[u][v]);
                path.push_back(v);
                v = u;
            }
            path.push_back(source);

            // Print path
            cout << source;
            for (int i = path.size() - 2; i >= 0; i--) {
                cout << " -> " << path[i];
            }
            cout << " (Flow: " << pathFlow << ")\n";

            // Update residual capacities
            v = sink;
            while (v != source) {
                int u = parent[v];
                residual[u][v] -= pathFlow;
                residual[v][u] += pathFlow;
                v = u;
            }

            maxFlowValue += pathFlow;
            pathNum++;
        }

        cout << "\n";
        return maxFlowValue;
    }

    void printFinalFlow() {
        cout << "Final Flow on Each Edge:\n";
        cout << "=======================\n";
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                int flow = capacity[i][j] - residual[i][j];
                if (flow > 0) {
                    cout << "Edge " << i << "->" << j 
                         << ": " << flow << "/" << capacity[i][j] << "\n";
                }
            }
        }
        cout << "\n";
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== Ford-Fulkerson Algorithm - Maximum Flow ===\n\n";

    // Test case 1: Simple flow network
    cout << "Test 1: Simple Flow Network\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges:\n";
    cout << "  0 → 1: 16\n";
    cout << "  0 → 2: 12\n";
    cout << "  1 → 2: 10\n";
    cout << "  1 → 3: 12\n";
    cout << "  2 → 1: 9\n";
    cout << "  2 → 3: 20\n";
    cout << "Source: 0, Sink: 3\n\n";

    FordFulkerson ff1(4);
    ff1.addEdge(0, 1, 16);
    ff1.addEdge(0, 2, 12);
    ff1.addEdge(1, 2, 10);
    ff1.addEdge(1, 3, 12);
    ff1.addEdge(2, 1, 9);
    ff1.addEdge(2, 3, 20);

    ff1.printCapacityGraph();

    int maxFlow1 = ff1.maxFlow(0, 3);
    cout << "Maximum Flow: " << maxFlow1 << "\n\n";
    ff1.printFinalFlow();
    ff1.printResidualGraph();

    cout << "\n" << string(60, '=') << "\n\n";

    // Test case 2: More complex network
    cout << "Test 2: More Complex Flow Network\n";
    cout << "Vertices: 0, 1, 2, 3, 4, 5\n";
    cout << "Source: 0, Sink: 5\n\n";

    FordFulkerson ff2(6);
    ff2.addEdge(0, 1, 10);
    ff2.addEdge(0, 2, 10);
    ff2.addEdge(1, 2, 2);
    ff2.addEdge(1, 3, 4);
    ff2.addEdge(1, 4, 8);
    ff2.addEdge(2, 4, 9);
    ff2.addEdge(3, 5, 10);
    ff2.addEdge(4, 3, 6);
    ff2.addEdge(4, 5, 10);

    ff2.printCapacityGraph();

    int maxFlow2 = ff2.maxFlow(0, 5);
    cout << "Maximum Flow: " << maxFlow2 << "\n\n";
    ff2.printFinalFlow();
    ff2.printResidualGraph();

    cout << "\n" << string(60, '=') << "\n\n";

    // Test case 3: Edmonds-Karp (BFS version)
    cout << "Test 3: Edmonds-Karp Algorithm (BFS-based)\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges:\n";
    cout << "  0 → 1: 3\n";
    cout << "  0 → 2: 2\n";
    cout << "  1 → 2: 2\n";
    cout << "  1 → 3: 2\n";
    cout << "  2 → 3: 3\n";
    cout << "Source: 0, Sink: 3\n\n";

    EdmondsKarp ek(4);
    ek.addEdge(0, 1, 3);
    ek.addEdge(0, 2, 2);
    ek.addEdge(1, 2, 2);
    ek.addEdge(1, 3, 2);
    ek.addEdge(2, 3, 3);

    int maxFlow3 = ek.maxFlow(0, 3);
    cout << "Maximum Flow: " << maxFlow3 << "\n\n";
    ek.printFinalFlow();

    return 0;
}

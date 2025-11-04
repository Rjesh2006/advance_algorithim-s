
// ============================================================================
// Kruskal's Algorithm - Minimum Spanning Tree (MST)
// ============================================================================
// Finds the minimum spanning tree of a weighted undirected graph
// Uses greedy approach: select edges with smallest weights
// Uses Union-Find (Disjoint Set Union) data structure
// Time Complexity: O(E log E) for sorting edges
// Space Complexity: O(V)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ============================================================================
// EDGE STRUCTURE
// ============================================================================

struct Edge {
    int source;      // First vertex
    int destination; // Second vertex
    int weight;      // Weight of edge

    // Comparator for sorting edges by weight
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// ============================================================================
// UNION-FIND (DISJOINT SET UNION) DATA STRUCTURE
// ============================================================================

class UnionFind {
private:
    vector<int> parent;  // Parent of each vertex
    vector<int> rank;    // Rank for union by rank optimization

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);

        // Initially, each vertex is its own parent
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    // Find the root of the set containing x (with path compression)
    int find(int x) {
        // If x is not its own parent, recursively find the root
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // Path compression
        }
        return parent[x];
    }

    // Union two sets containing x and y
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        // If both are already in same set, no union needed
        if (rootX == rootY)
            return false;

        // Union by rank: attach smaller rank to larger rank
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }

        return true;
    }
};

// ============================================================================
// KRUSKAL'S ALGORITHM
// ============================================================================

class Graph {
private:
    int vertices;           // Number of vertices
    vector<Edge> edges;     // List of all edges

public:
    Graph(int v) : vertices(v) {}

    // Add an undirected edge
    void addEdge(int src, int dest, int weight) {
        Edge edge = {src, dest, weight};
        edges.push_back(edge);
    }

    // Main Kruskal's algorithm
    void kruskalMST() {
        // Step 1: Sort all edges by weight (ascending order)
        sort(edges.begin(), edges.end());

        // Step 2: Initialize Union-Find structure
        UnionFind uf(vertices);

        // Step 3: Process edges and build MST
        vector<Edge> mst;      // Edges in MST
        int totalWeight = 0;   // Sum of weights in MST

        cout << "Building Minimum Spanning Tree using Kruskal's Algorithm:\n";
        cout << "Edge (u-v) : Weight | Action\n";
        cout << "------------------------------\n";

        for (const Edge& edge : edges) {
            int u = edge.source;
            int v = edge.destination;
            int w = edge.weight;

            // Check if adding this edge creates a cycle
            if (uf.unite(u, v)) {
                // No cycle, add to MST
                mst.push_back(edge);
                totalWeight += w;
                cout << edge.source << "-" << edge.destination << " : " 
                     << w << " | Added (no cycle)\n";
            } else {
                // Would create cycle, skip
                cout << edge.source << "-" << edge.destination << " : " 
                     << w << " | Skipped (would create cycle)\n";
            }

            // Stop when we have V-1 edges
            if (mst.size() == vertices - 1)
                break;
        }

        // Step 4: Print MST
        cout << "\n=== Minimum Spanning Tree ===\n";
        cout << "Edges in MST:\n";
        for (const Edge& edge : mst) {
            cout << edge.source << " - " << edge.destination 
                 << " : " << edge.weight << "\n";
        }
        cout << "\nTotal weight of MST: " << totalWeight << "\n";
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== Kruskal's Algorithm (MST) ===\n\n";

    // Test case 1: Simple graph
    cout << "Test 1: Simple connected graph\n";
    cout << "Vertices: 0, 1, 2, 3, 4\n";
    cout << "Edges: 0-1(4), 0-2(2), 1-2(1), 1-3(5), 2-3(8), 2-4(10), 3-4(2)\n\n";

    Graph g1(5);
    g1.addEdge(0, 1, 4);
    g1.addEdge(0, 2, 2);
    g1.addEdge(1, 2, 1);
    g1.addEdge(1, 3, 5);
    g1.addEdge(2, 3, 8);
    g1.addEdge(2, 4, 10);
    g1.addEdge(3, 4, 2);

    g1.kruskalMST();
    cout << "\n";

    // Test case 2: Another graph
    cout << "Test 2: Another connected graph\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges: 0-1(1), 0-3(3), 1-2(5), 1-3(4), 2-3(2)\n\n";

    Graph g2(4);
    g2.addEdge(0, 1, 1);
    g2.addEdge(0, 3, 3);
    g2.addEdge(1, 2, 5);
    g2.addEdge(1, 3, 4);
    g2.addEdge(2, 3, 2);

    g2.kruskalMST();
    cout << "\n";

    // Test case 3: Star graph
    cout << "Test 3: Star graph (all edges from center)\n";
    cout << "Vertices: 0, 1, 2, 3\n";
    cout << "Edges: 0-1(2), 0-2(3), 0-3(5)\n\n";

    Graph g3(4);
    g3.addEdge(0, 1, 2);
    g3.addEdge(0, 2, 3);
    g3.addEdge(0, 3, 5);

    g3.kruskalMST();

    return 0;
}

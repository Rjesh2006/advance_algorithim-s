
# Kruskal's Algorithm

### Theory Overview

**What is Kruskal's Algorithm?**

Kruskal's algorithm finds the Minimum Spanning Tree (MST) of a weighted undirected graph using a greedy approach.

**Minimum Spanning Tree:** A subset of edges that connects all vertices with minimum total weight.

**Key Idea:**
- Sort all edges by weight (ascending)
- Greedily pick smallest edges that don't create a cycle
- Stop when we have (V-1) edges

**Cycle Detection:** Uses Union-Find (Disjoint Set Union) to detect cycles efficiently.

### Union-Find Data Structure

**What is it?**
Efficient data structure for tracking disjoint sets with two operations:
- **Find:** Find which set an element belongs to
- **Union:** Merge two sets

**Optimizations:**
- **Path Compression:** When finding, make elements point directly to root
- **Union by Rank:** Always attach smaller tree to larger tree

#### Example:


# Bellman-Ford Algorithm

### Theory Overview

**What is Bellman-Ford?**

Bellman-Ford finds the shortest paths from a source vertex to all other vertices in a weighted graph. Unlike Dijkstra, it handles **negative edge weights**.

**Key Features:**
- Works with negative weights
- Detects negative-weight cycles
- Slower than Dijkstra but more flexible
- Dynamic programming approach

**Why it works:**
- Relaxes all edges (n-1) times
- After (n-1) iterations, all shortest paths are found
- If an edge can still be relaxed on iteration n, there's a negative cycle

### How It Works

#### Relaxation Process

For edge (u, v) with weight w:
- If `distance[u] + w < distance[v]`
- Then update `distance[v] = distance[u] + w`

This means: "If we can reach v shorter by going through u, update it"

#### Main Algorithm Steps

1. **Initialize** - Set source distance to 0, all others to ∞
2. **Relax Edges (V-1) times** - For each edge, try to improve distances
3. **Check for Negative Cycles** - Try to relax again; if possible, cycle exists
4. **Output Results** - Print shortest distances or report negative cycle

### Pseudocode

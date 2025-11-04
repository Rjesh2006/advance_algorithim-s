# Ford-Fulkerson Algorithm - Maximum Flow

---

## Table of Contents
1. [Theory Overview](#theory-overview)
2. [Key Concepts](#key-concepts)
3. [How It Works](#how-it-works)
4. [Pseudocode](#pseudocode)
5. [Dry Run Example](#dry-run-example)
6. [Reverse Edges & Backflow](#reverse-edges--backflow)
7. [Ford-Fulkerson vs Edmonds-Karp](#ford-fulkerson-vs-edmonds-karp)
8. [Complexity Analysis](#complexity-analysis)
9. [Applications](#applications)
10. [Practice Problems](#practice-problems)

---

## Theory Overview

### What is Ford-Fulkerson?

**Ford-Fulkerson algorithm** solves the **maximum flow problem** - finding the maximum amount of flow (or resource) that can be sent from a source vertex to a sink vertex through a directed graph.

**Key Features:**
- Uses greedy approach with augmenting paths
- Finds maximum possible flow through a network
- Works with integer capacities
- Can use DFS or BFS for finding paths
- Applies in many real-world scenarios

### Problem Definition

**Maximum Flow Problem:**
```
Given:
- Directed graph G = (V, E)
- Source vertex s and sink vertex t
- Capacity c(u,v) for each edge (u,v)

Find:
- Maximum flow f from s to t such that:
  1. Flow on edge ≤ capacity of edge
  2. Flow in = flow out for every vertex except s and t
```

### Why Ford-Fulkerson?

- **Simple to understand** - Intuitive greedy approach
- **Flexible** - Works with various capacity types
- **Optimal** - Guarantees maximum flow (with integer capacities)
- **Practical** - Used in many real applications

---

## Key Concepts

### 1. Flow Network

A **flow network** is a directed graph where:
- Each edge has a **capacity** (maximum flow allowed)
- One **source** vertex (where flow originates)
- One **sink** vertex (where flow ends)
- All capacities are non-negative

**Example:**
```
    ┌─── 1 ───┐
   /16         \12
  0             3 (sink)
   \12         /20
    └─── 2 ───┘
    
- Vertices: 0, 1, 2, 3
- Source: 0, Sink: 3
- Edges with capacities shown
```

### 2. Capacity Constraint

For each edge (u, v):
```
0 ≤ flow(u, v) ≤ capacity(u, v)
```

Flow cannot exceed edge capacity.

### 3. Flow Conservation

For each vertex except source and sink:
```
∑(flow into v) = ∑(flow out of v)
```

What flows in must flow out (no accumulation).

### 4. Augmenting Path

An **augmenting path** is a path from source to sink where:
- Every edge has **residual capacity** > 0
- Residual capacity = capacity - current flow
- We can send additional flow along this path

**Bottleneck:** Minimum residual capacity on path (maximum flow we can send)

### 5. Residual Graph

The **residual graph** represents:
- Remaining capacity on each edge
- Reverse edges (initially 0) for backflow

**For each edge (u, v) with capacity c:**
- Forward edge: (u, v) with residual = c - flow
- Reverse edge: (v, u) with residual = flow

**Purpose:** Reverse edges allow "undoing" flow if better path found

### 6. Reverse Edges (Backflow)

**Why reverse edges?**

Without reverse edges, we can't reroute flow. With them:
- If we send 10 units via A → B → C
- But later find B → D → C is better
- We can use reverse edge B → A to send flow back
- Then reroute through the better path

---

## How It Works

### Algorithm Overview

```
1. Initialize residual graph = capacity graph
2. While augmenting path exists from source to sink:
   a. Find path using DFS or BFS
   b. Find minimum capacity (bottleneck) on path
   c. Send bottleneck flow along path
   d. Update residual capacities (forward and reverse edges)
   e. Add bottleneck to total flow
3. Return maximum flow
```

### Step-by-Step Process

#### Phase 1: Initialization

```
Residual Graph Setup:
- For each edge (u, v) with capacity c:
  ├─ residual[u][v] = c
  └─ residual[v][u] = 0

Maximum Flow = 0
```

#### Phase 2: Find Augmenting Path

Using DFS or BFS:
```
Start from source s
Visit all reachable vertices with residual capacity > 0
If reach sink t:
  ├─ Path found! (augmenting path)
  └─ Trace back path using parent array
If cannot reach sink:
  ├─ No more augmenting paths
  └─ Algorithm terminates
```

#### Phase 3: Calculate Bottleneck

```
Path: s → v1 → v2 → ... → t
Residual capacities: r1, r2, ..., rn

Bottleneck = MIN(r1, r2, ..., rn)
```

This is maximum flow we can send via this path.

#### Phase 4: Update Residual Graph

For each edge (u, v) in path:
```
residual[u][v] -= bottleneck     (forward edge)
residual[v][u] += bottleneck     (reverse edge)
```

#### Phase 5: Accumulate Flow

```
Total Flow += bottleneck
```

#### Phase 6: Repeat

Find next augmenting path until none exists.

### Visual Example: One Iteration

```
Initial State:
    4       6
  0 ─→ 1 ─→ 3
    8

Step 1: Find path 0 → 1 → 3
- Residual: 4, 6
- Bottleneck: MIN(4, 6) = 4

Step 2: Send 4 units
- Edge 0→1: 4 - 4 = 0
- Edge 1→0: 0 + 4 = 4 (reverse)
- Edge 1→3: 6 - 4 = 2
- Edge 3→1: 0 + 4 = 4 (reverse)

After Iteration:
        0       2
  0 ←─ 1 ──→ 3
      4       4
    4   ←─

Total Flow: 4
```

---

## Pseudocode

### Complete Ford-Fulkerson Algorithm

```
FORD_FULKERSON(capacity, source, sink, V)
    INPUT:
        - capacity[V][V]: capacity matrix
        - source: source vertex
        - sink: sink vertex
        - V: number of vertices
    
    // Step 1: Initialize residual graph
    FOR u = 0 to V-1
        FOR v = 0 to V-1
            residual[u][v] = capacity[u][v]
        END FOR
    END FOR
    
    maxFlow = 0
    
    // Step 2: Find augmenting paths
    parent[V]  // Track path
    
    WHILE DFS_OR_BFS(source, sink, residual, parent) == TRUE
        // Step 3: Find bottleneck capacity
        bottleneck = INFINITY
        v = sink
        
        WHILE v != source
            u = parent[v]
            bottleneck = MIN(bottleneck, residual[u][v])
            v = u
        END WHILE
        
        // Step 4: Update residual capacities along path
        v = sink
        WHILE v != source
            u = parent[v]
            residual[u][v] -= bottleneck
            residual[v][u] += bottleneck
            v = u
        END WHILE
        
        // Step 5: Add bottleneck to total flow
        maxFlow += bottleneck
    END WHILE
    
    RETURN maxFlow

// DFS-based path finding
DFS_OR_BFS(source, sink, residual, parent)
    visited[V] = FALSE
    stack/queue = [source]
    visited[source] = TRUE
    
    WHILE stack/queue not empty
        u = pop/dequeue()
        
        FOR v = 0 to V-1
            // If not visited and has residual capacity
            IF visited[v] == FALSE AND residual[u][v] > 0
                visited[v] = TRUE
                parent[v] = u
                
                IF v == sink
                    RETURN TRUE
                
                push/enqueue(v)
            END IF
        END FOR
    END WHILE
    
    RETURN FALSE  // No augmenting path
```

### Edmonds-Karp Variant (BFS)

```
EDMONDS_KARP(capacity, source, sink, V)
    // Same as Ford-Fulkerson but use BFS instead of DFS
    // BFS finds SHORTEST augmenting path
    
    WHILE BFS(source, sink, residual, parent) == TRUE
        // Bottleneck calculation same
        // Residual update same
        // Only difference: use queue (BFS) instead of stack (DFS)
    END WHILE
    
    RETURN maxFlow

// BFS guarantees O(VE³) time complexity
```

---

## Dry Run Example

### Complete Traced Execution

```
GRAPH: 4 vertices (0, 1, 2, 3)

Original Edges & Capacities:
  0 → 1: 16
  0 → 2: 12
  1 → 2: 10
  1 → 3: 12
  2 → 1: 9
  2 → 3: 20

Task: Find maximum flow from 0 to 3

═══════════════════════════════════════════

INITIALIZATION:
  Residual Graph = Capacity Graph
  maxFlow = 0

═══════════════════════════════════════════

ITERATION 1:
  
  Find Path: DFS from 0
  ├─ Visit 0 (source)
  ├─ Explore 0 → 1: residual[0][1] = 16 > 0 ✓
  ├─ Visit 1
  ├─ Explore 1 → 3: residual[1][3] = 12 > 0 ✓
  ├─ Visit 3 (SINK REACHED!)
  
  Path Found: 0 → 1 → 3
  
  Calculate Bottleneck:
  ├─ residual[0][1] = 16
  ├─ residual[1][3] = 12
  ├─ bottleneck = MIN(16, 12) = 12
  
  Send Flow: 12 units
  
  Update Residuals:
  ├─ residual[0][1] = 16 - 12 = 4
  ├─ residual[1][0] = 0 + 12 = 12
  ├─ residual[1][3] = 12 - 12 = 0
  ├─ residual[3][1] = 0 + 12 = 12
  
  Total Flow: 0 + 12 = 12

═══════════════════════════════════════════

ITERATION 2:
  
  Find Path: DFS from 0
  ├─ Visit 0
  ├─ Explore 0 → 1: residual[0][1] = 4 > 0 ✓
  ├─ Visit 1
  ├─ Explore 1 → 3: residual[1][3] = 0 (blocked!)
  ├─ Explore 1 → 2: residual[1][2] = 10 > 0 ✓
  ├─ Visit 2
  ├─ Explore 2 → 3: residual[2][3] = 20 > 0 ✓
  ├─ Visit 3 (SINK REACHED!)
  
  Path Found: 0 → 1 → 2 → 3
  
  Calculate Bottleneck:
  ├─ residual[0][1] = 4
  ├─ residual[1][2] = 10
  ├─ residual[2][3] = 20
  ├─ bottleneck = MIN(4, 10, 20) = 4
  
  Send Flow: 4 units
  
  Update Residuals:
  ├─ residual[0][1] = 4 - 4 = 0
  ├─ residual[1][0] = 12 + 4 = 16
  ├─ residual[1][2] = 10 - 4 = 6
  ├─ residual[2][1] = 9 + 4 = 13
  ├─ residual[2][3] = 20 - 4 = 16
  ├─ residual[3][2] = 0 + 4 = 4
  
  Total Flow: 12 + 4 = 16

═══════════════════════════════════════════

ITERATION 3:
  
  Find Path: DFS from 0
  ├─ Visit 0
  ├─ Explore 0 → 1: residual[0][1] = 0 (blocked!)
  ├─ Explore 0 → 2: residual[0][2] = 12 > 0 ✓
  ├─ Visit 2
  ├─ Explore 2 → 1: residual[2][1] = 13 > 0 ✓
  ├─ Visit 1
  ├─ Explore 1 → 3: residual[1][3] = 0 (blocked!)
  ├─ Explore 1 → 2: residual[1][2] = 6 > 0 (but 2 visited!)
  ├─ Explore 1 → 0: residual[1][0] = 16 > 0 (but 0 visited!)
  ├─ No unvisited neighbors with capacity!
  
  Path Not Found via 0 → 2 → 1...
  Try other paths... (all blocked)
  
  Find Path: DFS from 0
  ├─ Visit 0
  ├─ Explore 0 → 2: residual[0][2] = 12 > 0 ✓
  ├─ Visit 2
  ├─ Explore 2 → 3: residual[2][3] = 16 > 0 ✓
  ├─ Visit 3 (SINK REACHED!)
  
  Path Found: 0 → 2 → 3
  
  Calculate Bottleneck:
  ├─ residual[0][2] = 12
  ├─ residual[2][3] = 16
  ├─ bottleneck = MIN(12, 16) = 12
  
  Send Flow: 12 units
  
  Update Residuals:
  ├─ residual[0][2] = 12 - 12 = 0
  ├─ residual[2][0] = 0 + 12 = 12
  ├─ residual[2][3] = 16 - 12 = 4
  ├─ residual[3][2] = 4 + 12 = 16
  
  Total Flow: 16 + 12 = 28

═══════════════════════════════════════════

ITERATION 4:
  
  Find Path: DFS from 0
  ├─ Visit 0
  ├─ Explore 0 → 1: residual[0][1] = 0 (blocked!)
  ├─ Explore 0 → 2: residual[0][2] = 0 (blocked!)
  
  No more paths from source!
  
  NO AUGMENTING PATH FOUND
  
  Algorithm Terminates

═══════════════════════════════════════════

FINAL RESULT:

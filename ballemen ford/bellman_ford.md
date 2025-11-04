# Bellman-Ford Algorithm - Shortest Paths with Negative Weights

---

## Table of Contents
1. [Theory Overview](#theory-overview)
2. [Key Concepts](#key-concepts)
3. [How It Works](#how-it-works)
4. [Pseudocode](#pseudocode)
5. [Dry Run Example](#dry-run-example)
6. [Negative Cycles](#negative-cycles)
7. [Relaxation Process](#relaxation-process)
8. [Bellman-Ford vs Dijkstra](#bellman-ford-vs-dijkstra)
9. [Complexity Analysis](#complexity-analysis)
10. [Applications](#applications)
11. [Practice Problems](#practice-problems)

---

## Theory Overview

### What is Bellman-Ford Algorithm?

**Bellman-Ford algorithm** finds the **shortest paths** from a source vertex to all other vertices in a weighted directed graph.

**Key Features:**
- Works with negative edge weights
- Detects negative-weight cycles
- Uses dynamic programming approach
- Simpler than Dijkstra but slower
- Guaranteed correctness for integer weights

### Problem Definition

```
Given:
- Directed weighted graph G = (V, E)
- Source vertex s
- Edge weights w(u, v) can be negative

Find:
- Shortest path distance from s to every vertex v
- Detect if negative-weight cycles exist
```

### Why Bellman-Ford?

- **Handles negative weights** - Dijkstra cannot
- **Detects negative cycles** - Critical for applications
- **Guaranteed correctness** - With integer weights
- **Simpler logic** - No complex data structures needed
- **Dynamic programming** - Optimal substructure property

### Real-World Example

```
Problem: Currency Exchange Arbitrage

Exchange Rates (as weight in log scale):
  USD → EUR: -0.92 (multiply by 0.92)
  EUR → GBP: -0.87 (multiply by 0.87)
  GBP → JPY: -126.50 (multiply by 126.50)
  JPY → USD: -0.0079 (multiply by 0.0079)

Is there an arbitrage opportunity?
(Path where multiplying rates > 1, or sum of logs > 0)

Solution:
- Model as shortest path problem
- Use negative logs as weights
- If we find positive cycle → arbitrage exists!
- Bellman-Ford detects this negative cycle
```

---

## Key Concepts

### 1. Shortest Path

**Definition:** Path with minimum total weight from source to destination.

```
Graph:
    2       3       1
  A ─── B ─── C ─── D
  │           │     │
  │─────3─────┤     │
  │           │     2
  └──────10─────────┘

Shortest path A to D:
  Direct: A → D = 10
  Via B: A → B → C → D = 2 + 3 + 1 = 6
  Via B alternate: A → B → D (through C) = slower
  
  Shortest: 6 (path A → B → C → D)
```

### 2. Weighted Graph

**Definition:** Graph where each edge has numerical value (weight).

**Characteristics:**
- Weights can be positive or negative
- Weights can represent costs, distances, time, etc.
- Direction matters (directed graph)

```
Example:
    -2       5
  A ──→ B ──→ C
  │          ↑
  │3       -1│
  └─────────→D
```

### 3. Source Vertex

**Definition:** Starting point for shortest path computation.

**Properties:**
- Distance to self is 0
- All distances from source computed
- Unreachable vertices have distance ∞

### 4. Edge Relaxation

**Definition:** Process of updating shortest path estimate for an edge.

**Relaxation Condition:**
```
For edge (u, v) with weight w:
If dist[u] + w < dist[v]:
    dist[v] = dist[u] + w
    predecessor[v] = u
```

**Intuition:** If path through u is shorter than current path to v, update it.

### 5. Negative Weight

**Definition:** Edge with negative value.

**Issues:**
- Dijkstra fails with negative weights
- Can create cycles with negative total weight
- May cause shortest path not to exist

```
Example with negative weight:
  A ──5──→ B ──-2──→ C
  
Path A to C:
  Direct: 5 + (-2) = 3
  
With negative cycle:
  A ──5──→ B ──-7──→ B (cycle with weight -2)
  We can loop indefinitely decreasing total weight
  Path may not exist!
```

### 6. Negative Cycle

**Definition:** Cycle with negative total weight.

**Implications:**
- Shortest paths not well-defined
- Can always find shorter path by cycling
- Must be detected and reported

```
Example:
  A ──1──→ B ──-5──→ C ──2──→ B
  
Cycle B → C → B:
  Weight: -5 + 2 = -3 (negative!)
  
From A to B repeatedly going through cycle:
  A → B = 1
  A → B → C → B = 1 + (-5) + 2 = -2
  A → B → C → B → C → B = 1 + (-5) + 2 + (-5) + 2 = -5
  ... decreases forever!
  
No shortest path exists from A to any vertex past B!
```

### 7. Distance Array

**Definition:** Array storing shortest distance from source to each vertex.

**Initialization:**
```
dist[source] = 0
dist[all other vertices] = ∞
```

**Update:** Relaxation changes these values.

### 8. Predecessor Array

**Definition:** Tracks which vertex comes before in shortest path.

**Purpose:**
- Reconstruct actual paths (not just distances)
- Trace back from destination to source

```
If shortest path is A → B → C → D:
  predecessor[B] = A
  predecessor[C] = B
  predecessor[D] = C

Reconstruction from D:
  D ← C ← B ← A
  Path: A → B → C → D
```

---

## How It Works

### Algorithm Overview

```
BELLMAN_FORD(Graph, source):
    1. Initialize distances (source = 0, others = ∞)
    2. Relax all edges (V-1) times
    3. Check for negative cycles
    4. Return distances or report negative cycle
```

### Key Insight: Why V-1 Iterations?

**Theorem:** Shortest path with maximum V-1 edges.

**Proof:**
- Any simple path has at most V-1 edges
- After V-1 iterations, all shortest paths computed
- If edge still relaxable on iteration V, negative cycle exists

```
Graph with V=5 vertices:

Simple path (no cycles): at most 4 edges
Examples:
  A → B (1 edge)
  A → B → C (2 edges)
  A → B → C → D → E (4 edges)

Never need more than V-1 = 4 iterations!
```

### Phase 1: Initialization

```
dist[source] = 0
dist[all others] = INFINITY
predecessor[all] = NULL

Example:
  Source: A
  Vertices: A, B, C, D
  
  dist: [0, ∞, ∞, ∞]
  pred: [NULL, NULL, NULL, NULL]
```

### Phase 2: Relaxation (V-1 Iterations)

**Each iteration:**
- Process every edge exactly once
- For each edge (u, v) with weight w:
  - If dist[u] + w < dist[v]:
    - Update dist[v] = dist[u] + w
    - Update pred[v] = u

**Why V-1 times?**
- Shortest path has at most V-1 edges
- Each iteration fixes one more edge
- After V-1 iterations, all shortest paths found

### Phase 3: Negative Cycle Detection

**Check:** Try to relax all edges one more time
- If any edge can still be relaxed → negative cycle exists
- Report cycle and terminate

```
Detection:
  FOR each edge (u, v, w):
    IF dist[u] + w < dist[v]:
        RETURN "Negative cycle detected!"
```

### Phase 4: Output

```
If no negative cycle:
  Return dist[] and predecessor[]
  
If negative cycle:
  Report cycle and affected vertices
```

### Example Walkthrough

```
Graph:
  0 ──4──→ 1
  │        ↓
  │2       ├2──→ 3
  ↓        │
  2 ──3────┘

Edges: 0→1(4), 0→2(2), 1→3(2), 2→1(-3), 2→3(3)
Source: 0
Vertices: 4

Step 1: Initialize
  dist = [0, ∞, ∞, ∞]

Step 2: Relax edges (V-1 = 3 iterations)

Iteration 1:
  Edge 0→1(4): dist[0]+4=4 < ∞ → dist[1]=4
  Edge 0→2(2): dist[0]+2=2 < ∞ → dist[2]=2
  Edge 1→3(2): dist[1]+2=6 < ∞ → dist[3]=6
  Edge 2→1(-3): dist[2]-3=-1 < 4 → dist[1]=-1
  Edge 2→3(3): dist[2]+3=5 < 6 → dist[3]=5
  Result: dist = [0, -1, 2, 5]

Iteration 2:
  Edge 0→1(4): 0+4=4 > -1, no change
  Edge 0→2(2): 0+2=2 = 2, no change
  Edge 1→3(2): -1+2=1 < 5 → dist[3]=1
  Edge 2→1(-3): 2-3=-1 = -1, no change
  Edge 2→3(3): 2+3=5 > 1, no change
  Result: dist = [0, -1, 2, 1]

Iteration 3:
  All edges check out, no updates
  Result: dist = [0, -1, 2, 1]

Step 3: Check for negative cycle
  Try relaxing all edges again:
    All failed (no changes possible)
  No negative cycle!

Step 4: Output
  Shortest distances from 0:
    0 → 0: 0
    0 → 1: -1
    0 → 2: 2
    0 → 3: 1
```

---

## Pseudocode

### Complete Bellman-Ford Algorithm

```
BELLMAN_FORD(graph, source, V, E)
    INPUT:
        - graph: list of edges with weights
        - source: starting vertex
        - V: number of vertices
        - E: list of all edges
    
    OUTPUT:
        - distance[]: shortest distances from source
        - negative cycle if exists
    
    // Phase 1: Initialization
    FOR each vertex v in graph
        IF v == source
            dist[v] = 0
        ELSE
            dist[v] = INFINITY
        END IF
        predecessor[v] = NULL
    END FOR
    
    // Phase 2: Relax all edges V-1 times
    FOR i = 1 to V-1
        FOR each edge (u, v, w) in E
            // Relaxation step
            IF dist[u] != INFINITY AND dist[u] + w < dist[v]
                dist[v] = dist[u] + w
                predecessor[v] = u
            END IF
        END FOR
    END FOR
    
    // Phase 3: Check for negative-weight cycles
    FOR each edge (u, v, w) in E
        IF dist[u] != INFINITY AND dist[u] + w < dist[v]
            RETURN "Graph contains negative-weight cycle"
        END IF
    END FOR
    
    // Phase 4: Return results
    RETURN (dist[], predecessor[])

// Helper function to print distances
PRINT_DISTANCES(dist, source, V)
    FOR each vertex v in 0..V-1
        IF dist[v] == INFINITY
            PRINT "Vertex " v ": unreachable"
        ELSE
            PRINT "Vertex " v ": " dist[v]
        END IF
    END FOR

// Helper function to reconstruct path
RECONSTRUCT_PATH(source, dest, predecessor)
    IF predecessor[dest] == NULL AND dest != source
        RETURN "No path exists"
    END IF
    
    path = [dest]
    current = dest
    
    WHILE current != source
        current = predecessor[current]
        path.insert_at_beginning(current)
    END WHILE
    
    RETURN path
```

### Simplified Version

```
BELLMAN_FORD_SIMPLE(E, V, source)
    // All vertices indexed 0 to V-1
    
    dist = [INFINITY] × V
    dist[source] = 0
    
    // Relax V-1 times
    FOR i = 1 to V-1
        FOR each edge (u, v, w) in E
            IF dist[u] != INFINITY
                dist[v] = MIN(dist[v], dist[u] + w)
            END IF
        END FOR
    END FOR
    
    // Check for negative cycle
    FOR each edge (u, v, w) in E
        IF dist[u] != INFINITY AND dist[u] + w < dist[v]
            RETURN "Negative cycle found"
        END IF
    END FOR
    
    RETURN dist
```

---

## Dry Run Example

### Complete Traced Execution

```
GRAPH: 4 vertices (0, 1, 2, 3)

Edges with Weights:
  0 → 1: 4
  0 → 2: 2
  1 → 2: 1
  1 → 3: 5
  2 → 1: -3
  2 → 3: 8

Source: 0
Task: Find shortest distances from 0 to all vertices

═══════════════════════════════════════════════

PHASE 1: INITIALIZATION

dist[0] = 0 (source)
dist[1] = ∞
dist[2] = ∞
dist[3] = ∞

predecessor[0] = NULL
predecessor[1] = NULL
predecessor[2] = NULL
predecessor[3] = NULL

State: [0, ∞, ∞, ∞]

═══════════════════════════════════════════════

PHASE 2: RELAXATION (V-1 = 3 iterations)

─────────────────────────────────────────────

ITERATION 1:

Processing all edges:

Edge 0→1 (weight 4):
  dist[0] + 4 = 0 + 4 = 4
  4 < ∞ ✓
  Update: dist[1] = 4
  Update: pred[1] = 0

Edge 0→2 (weight 2):
  dist[0] + 2 = 0 + 2 = 2
  2 < ∞ ✓
  Update: dist[2] = 2
  Update: pred[2] = 0

Edge 1→2 (weight 1):
  dist[1] + 1 = 4 + 1 = 5
  5 > 2 ✗
  No update

Edge 1→3 (weight 5):
  dist[1] + 5 = 4 + 5 = 9
  9 < ∞ ✓
  Update: dist[3] = 9
  Update: pred[3] = 1

Edge 2→1 (weight -3):
  dist[2] + (-3) = 2 + (-3) = -1
  -1 < 4 ✓
  Update: dist[1] = -1
  Update: pred[1] = 2

Edge 2→3 (weight 8):
  dist[2] + 8 = 2 + 8 = 10
  10 > 9 ✗
  No update

After Iteration 1:
  dist = [0, -1, 2, 9]
  Changes made: YES

─────────────────────────────────────────────

ITERATION 2:

Processing all edges:

Edge 0→1 (weight 4):
  0 + 4 = 4
  4 > -1 ✗
  No update

Edge 0→2 (weight 2):
  0 + 2 = 2
  2 = 2 ✗
  No update

Edge 1→2 (weight 1):
  -1 + 1 = 0
  0 > 2 ✗
  No update

Edge 1→3 (weight 5):
  -1 + 5 = 4
  4 < 9 ✓
  Update: dist[3] = 4
  Update: pred[3] = 1

Edge 2→1 (weight -3):
  2 + (-3) = -1
  -1 = -1 ✗
  No update

Edge 2→3 (weight 8):
  2 + 8 = 10
  10 > 4 ✗
  No update

After Iteration 2:
  dist = [0, -1, 2, 4]
  Changes made: YES

─────────────────────────────────────────────

ITERATION 3:

Processing all edges:

Edge 0→1 (weight 4):
  0 + 4 = 4 > -1 ✗

Edge 0→2 (weight 2):
  0 + 2 = 2 = 2 ✗

Edge 1→2 (weight 1):
  -1 + 1 = 0 > 2 ✗

Edge 1→3 (weight 5):
  -1 + 5 = 4 = 4 ✗

Edge 2→1 (weight -3):
  2 + (-3) = -1 = -1 ✗

Edge 2→3 (weight 8):
  2 + 8 = 10 > 4 ✗

After Iteration 3:
  dist = [0, -1, 2, 4]
  Changes made: NO (converged)

═══════════════════════════════════════════════

PHASE 3: NEGATIVE CYCLE DETECTION

Checking if any edge can still be relaxed:

Edge 0→1: 0 + 4 = 4 > -1 ✗
Edge 0→2: 0 + 2 = 2 = 2 ✗
Edge 1→2: -1 + 1 = 0 > 2 ✗
Edge 1→3: -1 + 5 = 4 = 4 ✗
Edge 2→1: 2 + (-3) = -1 = -1 ✗
Edge 2→3: 2 + 8 = 10 > 4 ✗

Result: NO NEGATIVE CYCLE DETECTED ✓

═══════════════════════════════════════════════

PHASE 4: RESULTS

Shortest Distances from Source 0:
├─ Vertex 0: 0 (source)
├─ Vertex 1: -1 (path: 0 → 2 → 1)
├─ Vertex 2: 2 (path: 0 → 2)
└─ Vertex 3: 4 (path: 0 → 2 → 1 → 3)

Path Reconstruction:

To Vertex 1:
  pred[1] = 2
  pred[2] = 0
  pred[0] = NULL
  Path: 0 → 2 → 1
  Distance: 0 + 2 + (-3) = -1 ✓

To Vertex 3:
  pred[3] = 1
  pred[1] = 2
  pred[2] = 0
  pred[0] = NULL
  Path: 0 → 2 → 1 → 3
  Distance: 0 + 2 + (-3) + 5 = 4 ✓

═══════════════════════════════════════════════
```

---

## Negative Cycles

### Detection Mechanism

**How Bellman-Ford Detects Negative Cycles:**

```
If shortest path exists:
  - No edge can be relaxed after V-1 iterations
  
If negative cycle exists:
  - At least one edge can still be relaxed on iteration V
  - Path through cycle keeps decreasing distance
```

**Detection Code:**
```
FOR each edge (u, v, w) in E
    IF dist[u] != INFINITY AND dist[u] + w < dist[v]
        PRINT "Negative cycle detected!"
        RETURN FALSE
    END IF
END FOR
RETURN TRUE  // No negative cycle
```

### Example: Negative Cycle

```
GRAPH: 3 vertices (0, 1, 2)

Edges:
  0 → 1: 1
  1 → 2: -3
  2 → 1: 1

Cycle: 1 → 2 → 1
Weight: -3 + 1 = -2 (NEGATIVE!)

Dry Run:

INITIALIZATION:
  dist = [0, ∞, ∞]

ITERATION 1:
  0→1: dist[1] = 0 + 1 = 1
  1→2: dist[2] = 1 + (-3) = -2
  2→1: dist[1] = -2 + 1 = -1 (updated!)
  Result: dist = [0, -1, -2]

ITERATION 2:
  0→1: dist[1] = 0 + 1 = 1 > -1, no change
  1→2: dist[2] = -1 + (-3) = -4 < -2 (updated!)
  2→1: dist[1] = -4 + 1 = -3 < -1 (updated!)
  Result: dist = [0, -3, -4]

ITERATION 3:
  0→1: 0 + 1 = 1 > -3, no change
  1→2: -3 + (-3) = -6 < -4 (updated!)
  2→1: -4 + 1 = -3 = -3, no change
  Result: dist = [0, -3, -6]

CYCLE DETECTION:
  Try relaxing edges again:
  0→1: 0 + 1 = 1 > -3 ✗
  1→2: -3 + (-3) = -6 = -6 ✗
  2→1: -6 + 1 = -5 < -3 ✓ STILL RELAXABLE!
  
  NEGATIVE CYCLE FOUND!
  Cycle: 1 → 2 → 1 with weight -2
```

### Why Negative Cycles are Problematic

```
With negative cycle reachable from source:
- Can make path arbitrarily short
- No shortest path exists!
- Path can loop forever in cycle

Example:
  Path A to Z: 100 units
  Through negative cycle: 100 - 2 - 2 - 2 - ... = -∞

Applications affected:
- Currency arbitrage: profit increases forever
- Network routing: packets loop infinitely
- Supply chains: costs become undefined
```

---

## Relaxation Process

### What is Relaxation?

**Relaxation:** Process of updating shortest path estimate when better path found.

```
Relaxation Condition:
  IF dist[u] + weight(u,v) < dist[v]:
      dist[v] = dist[u] + weight(u,v)
```

**Intuition:** If path through u is shorter than current path to v, update it.

### Relaxation Example

```
Edge: A → B with weight 5

Initial:
  dist[A] = 3
  dist[B] = 15

Relaxation Check:
  dist[A] + 5 = 3 + 5 = 8
  8 < 15? YES
  
Action: RELAX
  dist[B] = 8 (updated from 15)

Interpretation:
  Old path to B: length 15
  New path: go through A, total 8
  Better path found!
```

### Multiple Relaxations

```
Sequence of relaxations on same edge:

Edge A → B: weight 2

Step 1:
  dist[A] = 10, dist[B] = ∞
  Relax: dist[B] = 10 + 2 = 12

Step 2 (after A updated):
  dist[A] = 7 (improved), dist[B] = 12
  Relax: dist[B] = 7 + 2 = 9

Step 3 (after A improved again):
  dist[A] = 5, dist[B] = 9
  Relax: dist[B] = 5 + 2 = 7

Each relaxation brings B closer to actual shortest distance
After enough iterations, B reaches optimal distance
```

### Why V-1 Relaxations Suffice

```
Shortest path properties:
- Simple path (no cycles): max V-1 edges
- Each relaxation improves one vertex by one edge
- After k iterations: vertices reachable in k edges optimized

Example: 4 vertices
  Shortest path from 0: 0 → 1 → 2 → 3 (3 edges)
  
  Iteration 1: Vertex 1 optimized (0→1)
  Iteration 2: Vertex 2 optimized (0→1→2)
  Iteration 3: Vertex 3 optimized (0→1→2→3)
  
  After 3 = V-1 iterations: all vertices optimized!
```

---

## Bellman-Ford vs Dijkstra

### Comparison Table

| Aspect | Bellman-Ford | Dijkstra |
|--------|---|---|
| **Negative Weights** | ✓ Handles | ✗ Cannot |
| **Negative Cycles** | ✓ Detects | ✗ Cannot |
| **Time Complexity** | O(VE) | O(E log V) |
| **Space Complexity** | O(V) | O(V) |
| **Data Structures** | Simple arrays | Heap/PQ |
| **Implementation** | Easy | Moderate |
| **Practical Speed** | Slower | Faster |
| **Use Case** | Negative weights | All positive |

### When to Use Which

**Use Bellman-Ford when:**
- Graph has negative edge weights
- Need to detect negative cycles
- Correctness more important than speed
- V, E small enough

**Use Dijkstra when:**
- All weights are non-negative
- Need faster execution
- Large graphs with many queries
- Practical speed important

### Example Comparison

```
Graph:
  0 ──4──→ 1
  │        ↓
  2        -3
  ↓        ↓
  3 ──1───→ 2

Negative edge: 1 → 2: -3

BELLMAN-FORD:
  Iteration 1: dist[1] = 4, dist[2] = 5
  Iteration 2: dist[2] = 4 + (-3) = 1
  Time: O(3 × 5) = O(15)
  ✓ Works with negative weight

DIJKSTRA:
  Would fail because:
  - Assumes all weights non-negative
  - Greedy choice becomes invalid
  - May produce wrong results
  ✗ Cannot be used
```

---

## Complexity Analysis

### Time Complexity

**Components:**

```
1. Initialization: O(V)
   - Initialize distance and predecessor arrays

2. Main Loop: V-1 iterations
   - For each iteration: O(E) to process all edges
   - Total: O((V-1) × E) = O(VE)

3. Negative Cycle Check: O(E)
   - Check each edge once

Total: O(V) + O(VE) + O(E) = O(VE)
```

**Detailed Breakdown:**

```
For each of V-1 iterations:
  - Check every edge: O(E)
  - Relaxation operation: O(1) per edge
  - Total per iteration: O(E)

Total: (V-1) × O(E) = O(VE)
```

### Space Complexity

```
Arrays:
  - dist[]: O(V)
  - predecessor[]: O(V)
  - Edge list: O(E)

Total: O(V + E)

(No additional space required beyond input)
```

### Worst Case Scenarios

**Time:**
```
Sparse graph: E = O(V)
  Complexity: O(V²)

Dense graph: E = O(V²)
  Complexity: O(V³)

Complete graph: E = V(V-1)/2 ≈ V²
  Complexity: O(V³)
```

### Comparison with Other Algorithms

| Algorithm | Time (worst) | Negative | Cycle Detection |
|-----------|---|---|---|
| Bellman-Ford | O(VE) | ✓ Yes | ✓ Yes |
| Dijkstra | O(E log V) | ✗ No | ✗ No |
| Floyd-Warshall | O(V³) | ✓ Yes | ✓ Yes |

---

## Applications

### 1. Cur

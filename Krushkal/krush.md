# Kruskal's Algorithm - Minimum Spanning Tree

---

## Table of Contents
1. [Theory Overview](#theory-overview)
2. [Key Concepts](#key-concepts)
3. [Union-Find Data Structure](#union-find-data-structure)
4. [How It Works](#how-it-works)
5. [Pseudocode](#pseudocode)
6. [Dry Run Example](#dry-run-example)
7. [Union-Find Operations Detailed](#union-find-operations-detailed)
8. [Kruskal's vs Prim's Algorithm](#kruskals-vs-prims-algorithm)
9. [Complexity Analysis](#complexity-analysis)
10. [Applications](#applications)
11. [Practice Problems](#practice-problems)

---

## Theory Overview

### What is Kruskal's Algorithm?

**Kruskal's algorithm** is a greedy algorithm that finds the **Minimum Spanning Tree (MST)** of a weighted undirected connected graph.

**Key Features:**
- Uses greedy approach
- Sorts edges by weight
- Builds MST by adding edges without creating cycles
- Uses Union-Find for efficient cycle detection
- Simple and intuitive to understand

### Minimum Spanning Tree (MST)

**Definition:** A spanning tree of a graph is a subgraph that:
- Includes all vertices
- Is a tree (connected, no cycles)
- Has exactly V-1 edges (where V = number of vertices)

**Minimum Spanning Tree:** Among all possible spanning trees, the one with minimum total edge weight.

**Properties:**
- Unique MST if all edge weights are different
- Multiple MSTs possible if weights are equal
- Always has exactly V-1 edges
- Total weight is minimized
- Never contains a cycle

### Problem Definition

```
Given:
- Undirected weighted graph G = (V, E)
- Weight w(e) for each edge e

Find:
- Spanning tree T ⊆ E such that:
  1. T connects all vertices
  2. T has no cycles
  3. Sum of weights in T is minimum
```

### Why Kruskal's?

- **Simple** - Easy to understand and implement
- **Efficient** - O(E log E) time complexity
- **Optimal** - Always produces optimal MST
- **Greedy** - No complex data structures beyond Union-Find
- **Versatile** - Works well with disconnected components

### Real-World Example

```
Problem: Connect 5 cities with minimum total road cost

Cities: A, B, C, D, E
Roads & Costs:
  A-B: $100
  A-C: $150
  B-C: $80  ← Smallest
  B-D: $120
  C-D: $140
  D-E: $90
  C-E: $110

Solution (Kruskal's):
  1. B-C: $80  (connect B and C)
  2. D-E: $90  (connect D and E)
  3. B-D: $120 (connect B-C group with D-E group)
  4. A-B: $100 (connect A to existing tree)
  
  Total Cost: $390 (Minimum!)
  Tree: A-B-C-D-E connected
```

---

## Key Concepts

### 1. Weighted Graph

A **weighted graph** is a graph where each edge has an associated numerical value (weight).

**Representation:**
```
   10      15
  A ─── B ─── C
  │     /\    │
  │    /  \   │
  20  5    8  12
  │  /      \ │
  │ /        \│
  D ─────────E
        25
```

### 2. Spanning Tree

**Definition:** A spanning tree is:
- A subgraph that includes ALL vertices
- Connected (one path between any two vertices)
- Acyclic (tree structure)
- Has exactly V-1 edges

**Example:**
```
Original Graph (6 vertices, 9 edges):
Multiple spanning trees possible

One Spanning Tree (6 vertices, 5 edges):
   10      15
  A ─── B ─── C
  │     /     │
  │    /      │
  5   /       12
  │  /        │
  │ /         │
  D          E
  └────25────┘

All vertices included, connected, no cycles, 5 edges ✓
```

### 3. Minimum Spanning Tree

Among all possible spanning trees, select the one with minimum total weight.

**Optimality:** For any cut of the graph, the minimum weight edge crossing the cut is in some MST (Cut Property).

```
Graph with weights:
  Edge A-B: 10
  Edge B-C: 15
  Edge A-D: 5
  Edge D-E: 25
  Edge C-E: 12
  (Many possible spanning trees)

MST (among all spanning trees):
  Edges: A-D(5), A-B(10), B-C(15), C-E(12)
  Total: 42 ← Minimum!
```

### 4. Disjoint Sets

**Definition:** A collection of sets with no elements in common (disjoint).

**Example:**
```
Set 1: {A, B, D}
Set 2: {C, E}
Set 3: {F}

No overlap → Disjoint
```

### 5. Union Operation

**Definition:** Merge two disjoint sets into one.

**Example:**
```
Before UNION(Set1, Set2):
  Set 1: {A, B, D}
  Set 2: {C, E}

After UNION(Set1, Set2):
  Set 1: {A, B, D, C, E}
  Set 2: {} (merged)
```

### 6. Cycle Detection

**Using Union-Find:**
```
If both vertices of an edge are already in same set
→ Adding this edge creates a cycle
→ Don't add to MST

If vertices are in different sets
→ Adding this edge doesn't create cycle
→ Safe to add to MST
```

### 7. Cut Property (Why Kruskal's Works)

**Cut Property:** For any cut (partition) of the graph:
- Minimum weight edge crossing the cut is in SOME MST

**Example:**
```
Graph:
    A ─10─ B
    │      │
    5      15
    │      │
    D ─25─ C ─20─ E

Cut 1: {A, B} | {D, C, E}
  Edges crossing: A-D(5), B-C(15)
  Minimum: A-D(5) ✓ (will be in MST)

Cut 2: {A, B, D} | {C, E}
  Edges crossing: B-C(15), D-C(25), B-E(20)
  Minimum: B-C(15) ✓ (will be in MST)

This is why greedy approach works!
```

---

## Union-Find Data Structure

### What is Union-Find?

**Union-Find** (also called Disjoint Set Union - DSU) is a data structure that:
- Maintains collection of disjoint sets
- Supports fast FIND and UNION operations
- Uses path compression and union by rank for optimization

### Basic Operations

#### MAKE_SET(x)
Creates a new set containing only element x.

```
MAKE_SET(x)
    parent[x] = x          // x is its own parent
    rank[x] = 0            // Initial rank is 0
```

#### FIND(x)
Returns the representative (root) of set containing x.

**Without Optimization:**
```
FIND(x)
    IF parent[x] != x
        RETURN FIND(parent[x])
    ELSE
        RETURN x
    END IF
```

**With Path Compression:**
```
FIND(x)
    IF parent[x] != x
        parent[x] = FIND(parent[x])    // Path compression!
    END IF
    RETURN parent[x]
```

#### UNION(x, y)
Merges the set containing x with set containing y.

**Without Optimization:**
```
UNION(x, y)
    rootX = FIND(x)
    rootY = FIND(y)
    
    IF rootX != rootY
        parent[rootX] = rootY
        RETURN TRUE
    ELSE
        RETURN FALSE  // Already in same set
    END IF
```

**With Union by Rank:**
```
UNION(x, y)
    rootX = FIND(x)
    rootY = FIND(y)
    
    IF rootX == rootY
        RETURN FALSE  // Already in same set
    END IF
    
    // Attach smaller rank tree to larger rank tree
    IF rank[rootX] < rank[rootY]
        parent[rootX] = rootY
    ELSE IF rank[rootX] > rank[rootY]
        parent[rootY] = rootX
    ELSE
        parent[rootY] = rootX
        rank[rootX]++
    END IF
    
    RETURN TRUE
END

// This keeps tree shallow → faster FIND operations
```

### Path Compression Example

```
Before Path Compression:
  Tree structure:  1 (root)
                   │
                   2
                   │
                   3
                   │
                   4

FIND(4):
  4 points to 3, recurse
  3 points to 2, recurse
  2 points to 1, recurse
  1 is root
  Return 1

After Path Compression:
  Tree structure:  1 (root)
                  /│\
                 2 3 4 (all direct children)

Next FIND(4):
  4 directly points to 1 ✓ Faster!
```

### Union by Rank Example

```
Set A: 1 (rank 2)     Set B: 5 (rank 0)
       ├─2
       └─3

UNION(1, 5) with rank optimization:
  rank[1] = 2 > rank[5] = 0
  → Attach 5 to 1 (smaller rank to larger)

Result:  1 (root, rank 2)
         ├─2
         ├─3
         └─5 (not nested deep)

Tree remains shallow → Fast operations!
```

---

## How It Works

### Algorithm Overview

```
1. Sort all edges by weight (ascending)
2. Initialize Union-Find with each vertex in its own set
3. For each edge in sorted order:
   - If endpoints are in different sets:
     └─ Add edge to MST
     └─ Union the two sets
   - Else (endpoints in same set):
     └─ Skip edge (would create cycle)
4. Stop when MST has V-1 edges
5. Return MST with minimum total weight
```

### Step-by-Step Process

#### Phase 1: Preparation

```
Input: Graph with V vertices, E edges
Output: MST with V-1 edges, minimum total weight

Create edge list with all edges and weights
Sort edges by weight in ascending order
```

#### Phase 2: Initialization

```
Create Union-Find structure
FOR each vertex v in graph:
    MAKE_SET(v)          // Each vertex in its own set
    
MST = empty list          // Will store selected edges
Total Weight = 0
```

#### Phase 3: Edge Processing

```
FOR each edge (u, v, weight) in sorted order:
    
    root_u = FIND(u)      // Find set representative of u
    root_v = FIND(v)      // Find set representative of v
    
    IF root_u != root_v   // Different sets?
        MST.add(edge)     // Add to MST
        UNION(u, v)       // Merge the sets
        Total Weight += weight
        
        IF MST.size() == V - 1
            BREAK         // MST complete
        END IF
    ELSE
        Skip edge         // Would create cycle
    END IF
END FOR
```

#### Phase 4: Output

```
Return MST edges and total weight
```

### Why This Works: Greedy Choice Property

**Lemma:** If edge e is minimum weight edge NOT in any cycle, then e is in SOME MST.

**Proof Sketch:**
1. Sort edges by weight
2. Smallest edge can't be in cycle with smaller edges (doesn't exist yet)
3. Therefore, smallest edge can be in some MST
4. Adding smallest edge doesn't create cycle (by construction)
5. Repeat for remaining graph

**Result:** Greedy choice is always safe!

---

## Pseudocode

### Complete Kruskal's Algorithm

```
KRUSKAL_MST(graph, V, E)
    INPUT:
        - graph: weighted undirected graph
        - V: number of vertices
        - E: list of edges with weights
    
    OUTPUT:
        - MST: list of edges in minimum spanning tree
        - totalWeight: sum of edge weights in MST
    
    // Step 1: Sort edges by weight
    SORT(E) by weight in ascending order
    
    // Step 2: Initialize Union-Find
    FOR each vertex v in 0..V-1
        MAKE_SET(v)
    END FOR
    
    MST = empty list
    totalWeight = 0
    edges_added = 0
    
    // Step 3: Process edges in order
    FOR each edge (u, v, weight) in E
        // Find representative of each vertex's set
        root_u = FIND(u)
        root_v = FIND(v)
        
        // If in different sets, can add without creating cycle
        IF root_u != root_v
            MST.add((u, v, weight))
            UNION(root_u, root_v)
            totalWeight += weight
            edges_added++
            
            // MST is complete when we have V-1 edges
            IF edges_added == V - 1
                BREAK
            END IF
        END IF
    END FOR
    
    RETURN (MST, totalWeight)

// Helper: Union-Find Operations

MAKE_SET(x)
    parent[x] = x
    rank[x] = 0

FIND(x)
    IF parent[x] != x
        parent[x] = FIND(parent[x])    // Path compression
    END IF
    RETURN parent[x]

UNION(x, y)
    // x and y are roots (from FIND)
    IF rank[x] < rank[y]
        parent[x] = y
    ELSE IF rank[x] > rank[y]
        parent[y] = x
    ELSE
        parent[y] = x
        rank[x]++
    END IF
```

### Simplified Version (Without Rank)

```
KRUSKAL_SIMPLE(graph, V, E)
    SORT(E) by weight
    parent = array where parent[i] = i initially
    
    MST = empty
    
    FOR each edge (u, v, w) in E
        root_u = FIND_SIMPLE(u)
        root_v = FIND_SIMPLE(v)
        
        IF root_u != root_v
            MST.add(edge)
            parent[root_u] = root_v
        END IF
    END FOR
    
    RETURN MST

FIND_SIMPLE(x)
    IF parent[x] != x
        parent[x] = FIND_SIMPLE(parent[x])
    END IF
    RETURN parent[x]
```

---

## Dry Run Example

### Complete Traced Execution

```
GRAPH: 5 vertices (0, 1, 2, 3, 4)

Original Edges with Weights:
  0-1: 4
  0-2: 2
  1-2: 1
  1-3: 5
  2-3: 8
  2-4: 10
  3-4: 2

Task: Find Minimum Spanning Tree

═══════════════════════════════════════════════

PHASE 1: SORTING EDGES BY WEIGHT

Sorted Order:
  1. 1-2: 1     ← Smallest
  2. 0-2: 2
  3. 3-4: 2     (tie with above)
  4. 0-1: 4
  5. 1-3: 5
  6. 2-3: 8
  7. 2-4: 10

═══════════════════════════════════════════════

PHASE 2: INITIALIZATION

Union-Find Setup:
  MAKE_SET(0): parent[0]=0, rank[0]=0
  MAKE_SET(1): parent[1]=1, rank[1]=0
  MAKE_SET(2): parent[2]=2, rank[2]=0
  MAKE_SET(3): parent[3]=3, rank[3]=0
  MAKE_SET(4): parent[4]=4, rank[4]=0

Initial State:
  Set 0: {0}
  Set 1: {1}
  Set 2: {2}
  Set 3: {3}
  Set 4: {4}

MST = []
Total Weight = 0
Edges Added = 0

═══════════════════════════════════════════════

PHASE 3: EDGE PROCESSING

─────────────────────────────────────────────

EDGE 1: 1-2 (weight 1)

  FIND(1): parent[1]=1 → root = 1
  FIND(2): parent[2]=2 → root = 2
  
  1 != 2 → Different sets ✓
  
  Action: ADD TO MST
  ├─ MST.add((1, 2, 1))
  ├─ UNION(1, 2):
  │  ├─ rank[1]=0, rank[2]=0 (equal)
  │  ├─ parent[2]=1
  │  ├─ rank[1]=1
  │  └─ Sets: {0}, {1,2}, {3}, {4}
  ├─ Total Weight = 0 + 1 = 1
  ├─ Edges Added = 1
  
  MST: [(1-2:1)]

─────────────────────────────────────────────

EDGE 2: 0-2 (weight 2)

  FIND(0): parent[0]=0 → root = 0
  FIND(2): parent[2]=1, parent[1]=1 → root = 1
  
  0 != 1 → Different sets ✓
  
  Action: ADD TO MST
  ├─ MST.add((0, 2, 2))
  ├─ UNION(0, 1):
  │  ├─ rank[0]=0, rank[1]=1
  │  ├─ rank[0] < rank[1]
  │  ├─ parent[0]=1
  │  └─ Sets: {0,1,2}, {3}, {4}
  ├─ Total Weight = 1 + 2 = 3
  ├─ Edges Added = 2
  
  MST: [(1-2:1), (0-2:2)]

─────────────────────────────────────────────

EDGE 3: 3-4 (weight 2)

  FIND(3): parent[3]=3 → root = 3
  FIND(4): parent[4]=4 → root = 4
  
  3 != 4 → Different sets ✓
  
  Action: ADD TO MST
  ├─ MST.add((3, 4, 2))
  ├─ UNION(3, 4):
  │  ├─ rank[3]=0, rank[4]=0 (equal)
  │  ├─ parent[4]=3
  │  ├─ rank[3]=1
  │  └─ Sets: {0,1,2}, {3,4}
  ├─ Total Weight = 3 + 2 = 5
  ├─ Edges Added = 3
  
  MST: [(1-2:1), (0-2:2), (3-4:2)]

─────────────────────────────────────────────

EDGE 4: 0-1 (weight 4)

  FIND(0): parent[0]=1, parent[1]=1 → root = 1
  FIND(1): parent[1]=1 → root = 1
  
  1 != 1? NO → Same set! ✗
  
  Action: SKIP (would create cycle)
  ├─ Reason: 0 and 1 already connected through 2
  ├─ Path: 0-2-1 (via edges already in MST)
  ├─ Adding 0-1 would create cycle: 0-1-2-0
  
  MST: [(1-2:1), (0-2:2), (3-4:2)]  (no change)

─────────────────────────────────────────────

EDGE 5: 1-3 (weight 5)

  FIND(1): parent[1]=1 → root = 1
  FIND(3): parent[3]=3 → root = 3
  
  1 != 3 → Different sets ✓
  
  Action: ADD TO MST
  ├─ MST.add((1, 3, 5))
  ├─ UNION(1, 3):
  │  ├─ rank[1]=1, rank[3]=1 (equal)
  │  ├─ parent[3]=1
  │  ├─ rank[1]=2
  │  └─ Sets: {0,1,2,3,4} (all merged!)
  ├─ Total Weight = 5 + 5 = 10
  ├─ Edges Added = 4
  
  MST COMPLETE! (V-1 = 5-1 = 4 edges added)
  
  MST: [(1-2:1), (0-2:2), (3-4:2), (1-3:5)]
  
  BREAK

═══════════════════════════════════════════════

PHASE 4: RESULTS

Minimum Spanning Tree:
  ├─ Edge 1-2: weight 1
  ├─ Edge 0-2: weight 2
  ├─ Edge 3-4: weight 2
  └─ Edge 1-3: weight 5

Total Weight: 1 + 2 + 2 + 5 = 10

Tree Structure:
       1
      / \
     2   3
     |   |
     0   4
  
All 5 vertices connected
4 edges (V-1 = 5-1 = 4) ✓
No cycles ✓
Minimum weight ✓

Verification of Spanning Tree:
  Vertices included: 0, 1, 2, 3, 4 ✓
  Number of edges: 4 (V-1) ✓
  Connected: Can reach any vertex from any other ✓
  Acyclic: No cycles possible with 4 edges connecting 5 vertices ✓

═══════════════════════════════════════════════
```

---

## Union-Find Operations Detailed

### Path Compression Example

```
Initial Tree (no compression):
              0 (rank 3)
             /│\
            1 2 3 (rank 2)
           /│ │
          4 5 6 (rank 1)
         /│ │
        7 8 9 (rank 0)

FIND(9) without path compression:
  parent[9] = 5, recurse
  parent[5] = 2, recurse
  parent[2] = 0, recurse
  parent[0] = 0, return 0
  
  Path: 9 → 5 → 2 → 0 (3 steps)

After Path Compression (FIND(9)):
  parent[9] = 0 (direct!)
  parent[5] = 0 (direct!)
  parent[2] = 0 (already direct)
  
New Tree:
              0 (root)
            / | \... \
           1  2  3 ... 9 (all direct children)

Next FIND(9):
  parent[9] = 0, return 0 (1 step!)
```

### Union by Rank Example

```
Scenario 1: Without union by rank
  
  Set A:  1       Set B:  5
          │               │
          2               6
          │
          3
          │
          4 (deep tree)

  Union without optimization:
  parent[1] = 5
  
  Result (bad!):
    5
    └─1
      └─2
        └─3
          └─4
        └─6
    (Tree becomes deeper!)

Scenario 2: With union by rank

  Set A (rank 3):  1       Set B (rank 1):  5
                   └─2                       └─6
                     └─3
                       └─4

  rank[1] = 3 > rank[5] = 1
  parent[5] = 1 (attach smaller to larger)
  
  Result (good!):
      1 (rank 3)
     /|\
    2 5 ?
    │ │
    3 6
    │
    4
  
  Trees stay balanced!
```

### Complete Union-Find Example

```
Sequence of operations:

MAKE_SET(A): parent[A]=A, rank[A]=0
MAKE_SET(B): parent[B]=B, rank[B]=0
MAKE_SET(C): parent[C]=C, rank[C]=0
MAKE_SET(D): parent[D]=D, rank[D]=0

Sets: {A}, {B}, {C}, {D}

─────────────────────────────

UNION(A, B):
  rank[A]=0, rank[B]=0 (equal)
  parent[B]=A, rank[A]=1
  
Sets: {A,B}, {C}, {D}

─────────────────────────────

FIND(B):
  parent[B]=A ≠ B
  parent[B]=FIND(A)=A (with compression)
  return A

Sets: {A,B}, {C}, {D}

─────────────────────────────

UNION(C, D):
  rank[C]=0, rank[D]=0 (equal)
  parent[D]=C, rank[C]=1
  
Sets: {A,B}, {C,D}

─────────────────────────────

UNION(A, C):
  rank[A]=1, rank[C]=1 (equal)
  parent[C]=A, rank[A]=2
  
Sets: {A,B,C,D}

─────────────────────────────

FIND(D):
  parent[D]=C
  parent[C]=A (path compression happens here)
  return A
  
After compression:
  parent[D]=A (direct link created!)

Final: All in one set with A as representative
```

---

## Kruskal's vs Prim's Algorithm

### Comparison Table

| Aspect | Kruskal's | Prim's |
|--------|-----------|--------|
| **Approach** | Greedy (edges) | Greedy (vertices) |
| **Process** | Sorts all edges, picks smallest that don't form cycle | Grows tree from starting vertex |
| **Data Structure** | Union-Find | Min-heap / Priority Queue |
| **Time Complexity** | O(E log E) | O(E log V) with heap |
| **Space Complexity** | O(V) | O(V) |
| **Best For** | Sparse graphs | Dense graphs |
| **Finds** | MST by edge selection | MST by vertex selection |
| **Implementation** | Simpler | More complex |
| **Starting Vertex** | No specific requirement | Need starting vertex |

### When to Use Which

**Use Kruskal's when:**
- Graph is sparse (E < V²)
- All edges fit in memory easily
- Prefer simpler implementation
- Need deterministic behavior

**Use Prim's when:**
- Graph is dense (E ≈ V²)
- Memory for storing edges is limited
- Good priority queue available
- Already iterating from specific vertex

### Example: Both Algorithms

```
Graph: 4 vertices, edges A-B(1), B-C(2), A-C(4), C-D(3)

Kruskal's Approach:
  1. Sort: A-B(1), B-C(2), C-D(3), A-C(4)
  2. Add A-B(1): {A,B}, {C}, {D}
  3. Add B-C(2): {A,B,C}, {D}
  4. Add C-D(3): {A,B,C,D}
  5. Skip A-C(4): would create cycle
  MST Weight: 1+2+3 = 6

Prim's Approach (starting from A):
  1. Start at A, closest edge A-B(1)
  2. From {A,B}, closest edge B-C(2)
  3. From {A,B,C}, closest edge C-D(3)
  4. All vertices included
  MST Weight: 1+2+3 = 6

Same result, different process!
```

---

## Complexity Analysis

### Time Complexity

**Components:**

```
1. Sorting edges: O(E log E)
   - Most expensive step

2. Union-Find operations: O(E α(V))
   - α(V) is inverse Ackermann function
   - Practically constant (< 5 for all real values)

3. Total: O(E log E + E α(V))
           = O(E log E) [E α(V) dominated]
```

**Detailed Breakdown:**

```
For E edges:
- Sorting: O(E log E)
- Processing each edge:
  - FIND twice: O(2 × α(V)) per edge
  - UNION once: O(α(V)) per edge
  - Total per edge: O(α(V))
- All edges: O(E × α(V))

Total: O(E log E + E α(V))
     = O(E log E)  [since α(V) ≈ 1]
```

### Space Complexity

```
Union-Find structures:
  - parent array: O(V)
  - rank array: O(V)

Edge list storage: O(E)

Total: O(V + E)
```

### Practical Complexity

```
Scenario 1: Sparse graph (E ≈ V)
  Time: O(V log V)
  Space: O(V)
  Very efficient!

Scenario 2: Dense graph (E ≈ V²)
  Time: O(V² log V)
  Space: O(V²)
  Still better than O(V³) algorithms

Scenario 3: Complete graph (E = V(V-1)/2)
  Time: O(V² log V)
  Space: O(V²)
  Acceptable for moderately large V
```

### Comparison with Prim's

| Algorithm | Time (worst) | Time (avg) | Space |
|-----------|---|---|---|
| Kruskal's | O(E log E) | O(E log E) | O(E) |
| Prim's (simple) | O(V²) | O(V²) | O(V) |
| Prim's (heap) | O(E log V) | O(E log V) | O(V) |

---

## Applications

### 1. Network Design

**Problem:** Connect cities with minimum road cost

```
Cities: A, B, C, D, E
Connection costs between cities given
Goal: Connect all cities with minimum total cost

Solution:
- Create graph with cities as vertices
- Edge weights = connection costs
- Find MST using Kruskal's
- Build connections along MST edges

Result: All cities connected at minimum cost!
```

### 2. Circuit Design

**Problem:** Minimize wire in circuit board

```
Components: Processors, memory, IO ports

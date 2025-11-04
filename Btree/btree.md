
# B-Tree - Complete Detailed Documentation

---

## Table of Contents
1. [Introduction & Motivation](#introduction--motivation)
2. [B-Tree Fundamentals](#b-tree-fundamentals)
3. [B-Tree Properties & Invariants](#b-tree-properties--invariants)
4. [B-Tree vs Binary Search Tree](#b-tree-vs-binary-search-tree)
5. [B+Tree Overview](#btree-overview)
6. [Node Structure](#node-structure)
7. [Search Operation](#search-operation)
8. [Insert Operation](#insert-operation)
9. [Delete Operation](#delete-operation)
10. [Complexity Analysis](#complexity-analysis)
11. [Real-World Applications](#real-world-applications)
12. [Practice Problems & Solutions](#practice-problems--solutions)

---

## Introduction & Motivation

### Why B-Trees?

#### The Problem with Binary Search Trees

```
Binary Search Tree (BST):
                50
               /  \
              30   70
             / \   / \
            20 40 60 80

Insert: 10, 20, 30, 40, 50, 60, 70, 80
(Sorted input)

Result (DEGENERATED TO LINKED LIST):
                10
                  \
                   20
                     \
                      30
                        \
                         40
                           \
                            50
                              \
                               ...

Performance:
├─ Height: 7 (for 8 nodes)
├─ Search: O(n) instead of O(log n)
├─ Insertion becomes linear time!

Problem: Unbalanced tree for sorted data
```

#### Why Disk I/O Matters

```
Main Memory vs Disk Access:

┌──────────────────────────────────────────┐
│ CPU Cache Hit: 1 nanosecond              │
│ Main Memory Access: 100 nanoseconds      │
│ SSD Disk Access: 100,000 nanoseconds     │
│ HDD Disk Access: 10,000,000 nanoseconds  │
└──────────────────────────────────────────┘

If Tree Height = 10 in BST:
├─ 10 disk accesses needed!
├─ Time: 10 × 10,000,000 ns = 100 ms

If B-Tree with branching factor 100:
├─ Height = log₁₀₀(1,000,000) ≈ 3
├─ 3 disk accesses needed!
├─ Time: 3 × 10,000,000 ns = 30 ms

B-Tree is 3× faster!
```

### Solution: B-Tree

**Key Insight:** Increase branching factor to reduce tree height

```
B-Tree Structure (t=3, max keys=5):

Level 0 (Root):          [30, 70]
                        /   |   \
Level 1:        [10,20] [40,50,60] [80,90,95]

Height: 2 (vs 8 for degenerated BST)
Disk accesses: 2 (vs 8)

All disk pages are fully utilized:
├─ Root has 3 children
├─ Internal nodes have 2-3 children
└─ Better cache locality
```

---

## B-Tree Fundamentals

### What is a B-Tree?

**Definition:** A self-balancing tree data structure that maintains sorted data and allows searches, insertions, and deletions in logarithmic time.

**Key Characteristics:**
- Every node stores multiple keys (not just one)
- Every internal node has multiple children
- The tree remains balanced after every operation
- All leaves are at the same level
- Tree height is logarithmic in tree size

### Core Concepts

#### 1. Minimum Degree (t)

```
Minimum Degree t: Defines tree structure
├─ Each node has at most 2t-1 keys
├─ Each node has at most 2t children
├─ Each node (except root) has at least t-1 keys
├─ Each internal node (except root) has at least t children

Example: t = 3

Node Limits:
├─ Maximum keys: 2(3)-1 = 5
├─ Maximum children: 2(3) = 6
├─ Minimum keys (non-root): 3-1 = 2
├─ Minimum children (non-root internal): 3

Visual Representation:
Node Layout: [K₁, K₂, K₃, K₄, K₅] with 6 children
             └─ C₀ ─┘ └─ C₁ ─┘ └─ C₂ ─┘ └─ C₃ ─┘ └─ C₄ ─┘ └─ C₅ ─┘
             < K₁   K₁<*<K₂  K₂<*<K₃  K₃<*<K₄  K₄<*<K₅  > K₅
```

#### 2. Keys and Children Relationship

```
For node with k keys:
├─ Node has k+1 children (internal node)
├─ Node has 0 children (leaf node)

Key Ordering Rule:
├─ All keys in child Cᵢ < keys[i]
├─ All keys in child Cᵢ₊₁ > keys[i]

Example: Node with keys [30, 70]
         C₀    | C₁    | C₂
       <30    30-70   >70
       
Children:
├─ C₀ contains all keys < 30
├─ C₁ contains all keys between 30 and 70
└─ C₂ contains all keys > 70
```

---

## B-Tree Properties & Invariants

### Critical Properties

#### Property 1: All Leaves at Same Level

```
Valid B-Tree:
         [30, 70]
        /    |    \
    [10,20] [40,60] [80,90]  ← All at level 2

Invalid B-Tree:
         [30, 70]
        /    |    \
    [10,20] [40,60] [80] ← Different levels!
                      /
                    [85]

Why this matters:
├─ Guarantees O(log n) search time
├─ Ensures balanced structure
└─ All operations remain efficient
```

#### Property 2: Keys Within Node are Sorted

```
Valid: [10, 25, 40, 55, 70]
       10 < 25 < 40 < 55 < 70 ✓

Invalid: [10, 55, 25, 70, 40]
         Not sorted ✗
```

#### Property 3: Minimum Fill Constraint

```
Every node (except root) must have at least t-1 keys

Example: t = 3

├─ Minimum keys (non-root): 3-1 = 2
├─ Maximum keys (all nodes): 2(3)-1 = 5

Valid node states for non-root:
├─ [30, 40] (2 keys) ✓
├─ [30, 40, 50] (3 keys) ✓
├─ [30, 40, 50, 60] (4 keys) ✓
├─ [30, 40, 50, 60, 70] (5 keys) ✓

Invalid:
├─ [30] (1 key - too few) ✗
├─ [30, 40, 50, 60, 70, 80] (6 keys - too many) ✗
```

### Invariants Maintained by Operations

```
BEFORE any operation:
├─ All leaves at same level
├─ All keys sorted within nodes
├─ Minimum fill satisfied
├─ No duplicate keys

AFTER any operation:
├─ All leaves still at same level
├─ All keys still sorted within nodes
├─ Minimum fill still satisfied
├─ No duplicate keys

Operations must maintain these invariants!
```

---

## B-Tree vs Binary Search Tree

### Comparison Table

```
┌─────────────────────────────────────────────────────────┐
│ Property          │ BST        │ B-Tree (t=3)           │
├─────────────────────────────────────────────────────────┤
│ Keys per node     │ 1          │ 2-5                    │
│ Children per node │ 0-2        │ 0-6                    │
│ Height for n=1M   │ 20 worst   │ 3-4                    │
│ Sorted iteration  │ O(n)       │ O(n)                   │
│ Disk access       │ Many       │ Few                    │
│ Balanced by design│ No         │ Yes                    │
│ Insertion simple? │ Yes        │ No (may split)         │
│ Deletion simple?  │ No         │ No (may merge)         │
│ Best for          │ RAM        │ Disk/External Storage  │
└─────────────────────────────────────────────────────────┘

Visual Comparison (1,000,000 keys):

BST (Balanced):
├─ Height: 20
├─ Path length: 20 node accesses
├─ If each access = 10ms: 200ms total

B-Tree (t=100):
├─ Height: 3
├─ Path length: 3 node accesses
├─ If each access = 10ms: 30ms total

B-Tree is 6-7× faster for large datasets!
```

---

## B+Tree Overview

### What is B+Tree?

**Definition:** Variant of B-Tree where:
- Internal nodes store keys (used for navigation only)
- Leaf nodes store actual data and keys
- Leaf nodes are linked (form linked list)

### B-Tree vs B+Tree

```
B-Tree:
        [30, 70]
       /    |    \
    [10] [40,60] [80]
    
Data at BOTH internal and leaf nodes

B+Tree:
        [30, 70]          (Keys only, no data)
       /    |    \
    [10] [40,60] [80]    (Keys and data)
    └────────────┬─────────── (Linked list of leaves)


Advantages of B+Tree:
├─ All data at leaves (sequential access friendly)
├─ Linked leaves enable range queries
├─ Internal nodes hold only keys (more keys per node)
└─ Better cache performance

Disadvantages:
├─ Duplicate keys in internal nodes
├─ More complex to implement
└─ Extra pointer overhead for leaf links
```

### B+Tree Example

```
B+Tree with linked leaf nodes:

         [50]
        /    \
    [20]      [80]
    / \       / \
  [10,20] [40,50] [60,70] [80,90]
    |  ↔  |    ↔   |    ↔    |
    (Linked list of leaf nodes)

Range Query: Find all keys between 35 and 85
├─ Search for 35: Navigate to [40, 50]
├─ Follow linked list: [40,50] → [60,70] → [80,90]
├─ Collect: 40, 50, 60, 70, 80
├─ Time: O(log n + k) where k = results
└─ Very efficient for range queries!
```

---

## Node Structure

### B-Tree Node Components

```
CLASS BTreeNode
    FIELDS:
    ├─ keys: array of size 2t-1
    │  └─ Stores keys in sorted order
    │
    ├─ children: array of size 2t
    │  └─ Pointers to child nodes
    │
    ├─ isLeaf: boolean
    │  └─ TRUE if leaf node, FALSE if internal node
    │
    ├─ currentKeyCount: integer
    │  └─ Number of keys actually stored (≤ 2t-1)
    │
    └─ t: integer
       └─ Minimum degree (shared parameter)
```

### Memory Layout

```
B-Tree Node (t=3):
┌──────────────────────────────────────────┐
│ BTreeNode                                │
├──────────────────────────────────────────┤
│ t = 3                                    │
│ keys = [30, 70, ?, ?, ?]   (5 slots)    │
│ children = [↓, ↓, ↓, ↓, ↓, ↓] (6 slots)│
│ isLeaf = false                           │
│ currentKeyCount = 2                      │
└──────────────────────────────────────────┘

Actually used:
├─ keys[0] = 30, keys[1] = 70
├─ children[0], children[1], children[2]
└─ Remaining slots unused until needed
```

### Node Initialization

```
CREATE_NODE(t, isLeaf)
    ├─ Create new BTreeNode
    ├─ Allocate keys array of size 2t-1
    ├─ Allocate children array of size 2t
    ├─ Set isLeaf flag
    ├─ Initialize currentKeyCount = 0
    └─ Return new node

Example:
├─ BTreeNode root = CREATE_NODE(3, true)
│  └─ Empty leaf node with space for 5 keys
│
├─ BTreeNode internal = CREATE_NODE(3, false)
│  └─ Empty internal node with space for 5 keys and 6 children
```

---

## Search Operation

### Search Algorithm

```
BTREE_SEARCH(node, key)
    ├─ Step 1: Find position to check
    │  ├─ i = 0
    │  └─ WHILE i < node.currentKeyCount AND key > node.keys[i]
    │     └─ i++
    │
    ├─ Step 2: Check if key found at position i
    │  ├─ IF i < node.currentKeyCount AND key == node.keys[i]
    │  │  └─ RETURN node (found!)
    │  │
    │  ├─ Step 3: If leaf and not found, key doesn't exist
    │  ├─ IF node.isLeaf
    │  │  └─ RETURN NULL
    │  │
    │  └─ Step 4: Recurse to appropriate child
    │     └─ RETURN BTREE_SEARCH(node.children[i], key)
```

### Search Dry Run

```
B-Tree (t=3):
          [40, 80]
         /    |    \
    [10,20] [50,60,70] [90,100]

Search 1: SEARCH(root, 60)
├─ At node [40, 80]
├─ Compare 60 with keys:
│  ├─ 60 > 40? Yes, i++
│  ├─ 60 > 80? No, stop
│  ├─ i = 1
├─ keys[1] = 80, 60 ≠ 80
├─ Not a leaf, recurse to children[1]
├─ At node [50, 60, 70]
├─ Compare 60 with keys:
│  ├─ 60 > 50? Yes, i++
│  ├─ 60 > 60? No, stop
│  ├─ i = 1
├─ keys[1] = 60, 60 == 60? YES!
├─ RETURN node ✓

Search 2: SEARCH(root, 55)
├─ At node [40, 80]
├─ Compare 55 with keys:
│  ├─ 55 > 40? Yes, i++
│  ├─ 55 > 80? No, stop
│  ├─ i = 1
├─ keys[1] = 80, 55 ≠ 80
├─ Not a leaf, recurse to children[1]
├─ At node [50, 60, 70]
├─ Compare 55 with keys:
│  ├─ 55 > 50? Yes, i++
│  ├─ 55 > 60? No, stop
│  ├─ i = 1
├─ keys[1] = 60, 55 ≠ 60
├─ IS A LEAF, but 55 not found
├─ RETURN NULL (not found) ✗

Search 3: SEARCH(root, 10)
├─ At node [40, 80]
├─ Compare 10 with keys:
│  ├─ 10 > 40? No, stop
│  ├─ i = 0
├─ keys[0] = 40, 10 ≠ 40
├─ Not a leaf, recurse to children[0]
├─ At node [10, 20]
├─ Compare 10 with keys:
│  ├─ 10 > 10? No, stop
│  ├─ i = 0
├─ keys[0] = 10, 10 == 10? YES!
├─ RETURN node ✓
```

### Search Complexity

```
Time Complexity: O(t × log_t(n))

Where:
├─ n = number of keys in tree
├─ t = minimum degree
├─ log_t(n) = height of tree
├─ t = time to search within node

Example: n = 1,000,000, t = 100
├─ Height = log₁₀₀(1,000,000) ≈ 3
├─ Comparisons per node = 100
├─ Total comparisons = 3 × 100 = 300
└─ Much better than 20 for balanced BST!

But with binary search within node:
├─ Time per node = O(log t)
├─ Total time = O(log_t(n) × log t)
└─ Even more efficient!
```

---

## Insert Operation

### Insert Strategy

**Key Idea:** Keep tree balanced by:
1. Preventing nodes from becoming overfull (> 2t-1 keys)
2. Splitting full nodes **before** descending into them
3. Promoting middle key to parent

### Full Node Splitting

#### Splitting Process (t=3)

```
Full Node (5 keys):
Before Split: [10, 20, 30, 40, 50] (2t-1 = 5 keys)

Split at middle (position t-1 = 2):
├─ Left part: [10, 20]        (keys 0 to t-2)
├─ Middle key: 30             (key t-1)
├─ Right part: [40, 50]       (keys t to 2t-2)

Result:
├─ Left node: [10, 20]
├─ Promoted to parent: 30
├─ Right node: [40, 50]

Parent before split: [70]
Parent after split:  [30, 70]  (30 inserted)

If parent becomes full:
└─ Parent is split next!
```

### Insert Algorithm (Proactive Splitting)

```
BTREE_INSERT(tree, key)
    ├─ IF root is full
    │  ├─ Create new root
    │  ├─ Old root becomes child of new root
    │  ├─ Split old root
    │  └─ Insert key into new root
    ├─ ELSE
    │  └─ INSERT_NON_FULL(root, key)

INSERT_NON_FULL(node, key)
    ├─ Step 1: Find position for key
    │  ├─ i = node.currentKeyCount - 1
    │  └─ WHILE i >= 0 AND key < node.keys[i]
    │     └─ i--
    │  └─ i++ (now at correct position)
    │
    ├─ Step 2: If leaf, insert directly
    │  ├─ IF node.isLeaf
    │  │  ├─ Shift keys right to make space
    │  │  ├─ Insert key at position i
    │  │  ├─ Increment currentKeyCount
    │  │  └─ RETURN
    │  │
    │  └─ Step 3: If internal node and child full, split it
    │     ├─ IF node.children[i] is full
    │     │  ├─ SPLIT_CHILD(node, i)
    │     │  ├─ IF key > node.keys[i]
    │     │  │  └─ i++
    │     │  │
    │     │  └─ Step 4: Recurse to appropriate child
    │     │     └─ INSERT_NON_FULL(node.children[i], key)
```

### Insert Dry Run

```
B-Tree (t=3): max keys = 5, trigger split at 5

Initial empty tree: root = NULL

Insert 1: INSERT(10)
├─ Create root: [10]
├─ Tree: [10]

Insert 2: INSERT(20)
├─ Root not full
├─ Add to root: [10, 20]
├─ Tree: [10, 20]

Insert 3: INSERT(30)
├─ Root not full
├─ Add to root: [10, 20, 30]
├─ Tree: [10, 20, 30]

Insert 4: INSERT(5)
├─ Root not full
├─ Add to root: [5, 10, 20, 30]
├─ Tree: [5, 10, 20, 30]

Insert 5: INSERT(40)
├─ Root not full
├─ Add to root: [5, 10, 20, 30, 40]
├─ Tree: [5, 10, 20, 30, 40] (now full with 5 keys)

Insert 6: INSERT(50) [ROOT IS FULL, MUST SPLIT]
├─ Root [5, 10, 20, 30, 40] is full (5 keys)
├─ Before inserting 50, must split root
├─ Split at middle: key 20
│
├─ New structure:
│        [20]
│       /    \
│   [5,10]  [30,40]
│
├─ Now insert 50 into right child
├─ Right child not full
├─ Add to right: [30, 40, 50]
│
├─ Final Tree:
│        [20]
│       /    \
│   [5,10]  [30,40,50]

Insert 7: INSERT(60)
├─ Root [20] not full
├─ 60 > 20, go to right child
├─ Right child [30, 40, 50] not full (3 keys < 5)
├─ Add: [30, 40, 50, 60]
│
├─ Final Tree:
│        [20]
│       /    \
│   [5,10]  [30,40,50,60]

Insert 8: INSERT(25)
├─ Root [20] not full
├─ 25 > 20, go to right child
├─ Right child [30, 40, 50, 60] not full (4 keys < 5)
├─ Add: [25, 30, 40, 50, 60] (after sorting)
│
├─ Final Tree:
│        [20]
│       /    \
│   [5,10]  [25,30,40,50,60]

Insert 9: INSERT(15) [RIGHT CHILD BECOMES FULL]
├─ Root [20] not full
├─ 15 < 20, go to left child
├─ Left child [5, 10] not full
├─ Add: [5, 10, 15]
│
├─ Final Tree:
│          [20]
│         /    \
│   [5,10,15] [25,30,40,50,60]

Insert 10: INSERT(70) [RIGHT CHILD FULL, MUST SPLIT]
├─ Root [20] not full
├─ 70 > 20, go to right child
├─ Right child [25, 30, 40, 50, 60] is FULL (5 keys)
├─ Before descending, split right child
├─ Split at middle: key 40
│
├─ New structure:
│              [20, 40]
│          /      |      \
│    [5,10,15]  [25,30]  [50,60]
│
├─ Now insert 70 into correct child
├─ 70 > 40, go to rightmost child
├─ Add to [50, 60]: [50, 60, 70]
│
├─ Final Tree:
│              [20, 40]
│          /      |      \
│    [5,10,15]  [25,30]  [50,60,70]

Tree Summary:
├─ Height: 2
├─ Root has 2 keys, 3 children
├─ All leaves at same level
└─ All nodes satisfy B-Tree properties ✓
```

### Visual Growth Over Time

```
Step-by-step insertion sequence: 10,20,5,6,12,30,7,17,3,35

After 5 insertions: [5,6,10,12,20]
└─ Single node (full)

After 6th insertion (30): Split occurs
       [10]
       /  \
    [5,6] [12,20,30]

After 9 insertions: [10, 17]
       /      |      \
    [5,6,7]  [12]  [20,30,35]

After 10 insertions: Complex structure forms
└─ Height: 2
└─ All internal nodes have 2-3 keys
└─ All leaves have 2-3 keys
```

---

## Delete Operation

### Delete Strategy

**Challenge:** Maintain B-Tree properties after deletion:
- Keys must stay sorted within nodes
- Minimum fill must be maintained (except root)
- Tree must remain balanced

**Three Cases:**

```
1. Key in leaf node: Delete directly
2. Key in internal node: Replace with predecessor/successor
3. Underflow: Borrow from sibling or merge with sibling
```

### Delete Algorithm

```
BTREE_DELETE(node, key)
    ├─ Step 1: Find position of key
    │  ├─ i = 0
    │  └─ WHILE i < node.currentKeyCount AND key > node.keys[i]
    │     └─ i++
    │
    ├─ Step 2: Case 1 - Key found in current node
    │  ├─ IF i < node.currentKeyCount AND key == node.keys[i]
    │  │  ├─ IF node.isLeaf
    │  │  │  └─ DELETE from leaf
    │  │  │
    │  │  └─ ELSE
    │  │     ├─ IF node.children[i] has ≥ t keys
    │  │     │  └─ Replace with predecessor
    │  │     ├─ ELSE IF node.children[i+1] has ≥ t keys
    │  │     │  └─ Replace with successor
    │  │     └─ ELSE
    │  │        ├─ Merge children[i] and children[i+1]
    │  │        └─ Recurse into merged child
    │  │
    │  └─ Step 3: Case 2 - Key not in current node
    │     ├─ IF node.isLeaf
    │     │  └─ Key doesn't exist, error
    │     │
    │     └─ ELSE
    │        ├─ Check if key is in subtree rooted at children[i]
    │        ├─ IF children[i] has only t-1 keys
    │        │  ├─ TRY BORROW from left sibling
    │        │  ├─ IF left sibling has ≥ t keys
    │        │  │  └─ Move key from parent to child
    │        │  │  └─ Move key from left sibling to parent
    │        │  │
    │        │  ├─ ELSE TRY BORROW from right sibling
    │        │  ├─ IF right sibling has ≥ t keys
    │        │  │  └─ Move key from parent to child
    │        │  │  └─ Move key from right sibling to parent
    │        │  │
    │        │  └─ ELSE MERGE with sibling
    │        │     ├─ Merge child with sibling
    │        │     └─ Move separating key down to child
    │        │
    │        └─ RECURSE into appropriate child
```

### Delete Dry Run

```
Starting B-Tree (t=2):
        [30, 70]
       /   |    \
    [10] [40,50] [80,90]

Delete 1: DELETE(40)
├─ Node [30, 70] root
├─ 40 > 30, go to children[1] = [40, 50]
├─ [40, 50] is a leaf
├─ 40 is first key in this leaf
├─ Remove 40: [50]
│
├─ Result:
│        [30, 70]
│       /   |    \
│    [10] [50] [80,90]

Delete 2: DELETE(10)
├─ Node [30, 70] root
├─ 10 < 30, go to children[0] = [10]
├─ [10] is a leaf
├─ 10 is in leaf
├─ Remove 10: []
├─ NOW LEAF IS EMPTY! (Underflow)
├─ Must BORROW from sibling
├─ Right sibling [50] has 1 key (minimum for t=2)
├─ LEFT SIBLING doesn't exist
├─ MERGE [empty], key 30, [50] into one node
├─ New middle node: [30, 50]
├─ Remove 30 from parent
│
├─ Result:
│        [70]
│       /    \
│   [30,50] [80,90]

Delete 3: DELETE(70)
├─ Root [70]
├─ 70 is first key
├─ Root has only 1 key
├─ Left child [30, 50] has 2 keys (can give one)
├─ Right child [80, 90] has 2 keys (can give one)
├─ BORROW from right child [80, 90]
├─ Move 70 down: [30, 50, 70]
├─ Move 80 up to root: [80]
├─ Right child becomes [90]
│
├─ Result:
│        [80]
│       /    \
│   [30,50,70] [90]

Final Result after 3 deletions:
        [80]
       /    \
   [30,50,70] [90]
```

### Deletion Complexity

```
Time Complexity: O(t × log_t(n))

Why complex?
├─ Must handle underflow conditions
├─ May need to borrow from siblings
├─ May need to merge nodes
├─ Each operation requires careful key movement

Space: O(1) excluding tree
```

---

## Complexity Analysis

### Time Complexity Summary

```
┌─────────────────────────────────────────────────────────

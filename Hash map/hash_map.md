# Hash Map - Complete Detailed Documentation

---

## Table of Contents
1. [Introduction & Theory](#introduction--theory)
2. [Key Concepts](#key-concepts)
3. [Hash Function Design](#hash-function-design)
4. [Collision Resolution Techniques](#collision-resolution-techniques)
5. [Implementation Details](#implementation-details)
6. [Operations - Deep Dive](#operations---deep-dive)
7. [Complexity Analysis](#complexity-analysis)
8. [Real-World Examples](#real-world-examples)
9. [Advanced Topics](#advanced-topics)
10. [Practice Problems & Solutions](#practice-problems--solutions)

---

## Introduction & Theory

### What is a Hash Map?

A **Hash Map** (also called Hash Table or Dictionary) is a data structure that implements an associative array - a structure that maps keys to values using a **hash function**.

```
Real-World Analogy:
┌─────────────────────────────────────────────────────────┐
│ Telephone Directory                                      │
│ ─────────────────────────────────────────────────────────│
│ Key: Person's Name → Value: Phone Number               │
│                                                          │
│ Traditional Array (Sequential Search):                   │
│ - Must go through every entry                           │
│ - Time: O(n)                                            │
│                                                          │
│ Hash Map (Direct Lookup):                               │
│ - Hash name to get phone directory shelf                │
│ - Go directly to shelf and find number                  │
│ - Time: O(1) on average                                 │
└─────────────────────────────────────────────────────────┘
```

### Why Hash Maps Are Important

**Problem Without Hash Maps:**
```
Data: ["Alice", "Bob", "Charlie", "Diana"]
Search for "Charlie": 
├─ Check index 0: "Alice" ≠ "Charlie"
├─ Check index 1: "Bob" ≠ "Charlie"
├─ Check index 2: "Charlie" = "Charlie" ✓
Time: O(n) - need to scan through

With 1 million entries, average 500,000 comparisons!
```

**Solution With Hash Maps:**
```
Hash("Charlie") = 42
Go directly to index 42, find "Charlie"
Time: O(1) - direct access
```

### Core Idea

1. **Hash Function**: Converts key to array index
2. **Hash Table**: Array to store key-value pairs
3. **Collision Resolution**: Handle multiple keys hashing to same index
4. **Load Factor**: Ratio of entries to table size

---

## Key Concepts

### 1. Hash Function

**Definition:** Mathematical function that maps input (key) to fixed output (index)

**Requirements:**
- **Deterministic**: Same input always produces same output
- **Distributed**: Keys spread evenly across table
- **Fast**: Computable in O(1) time
- **Collision-Minimizing**: Different keys should hash to different indices

#### Simple Hash Function Examples

```
For Integer Keys:
├─ h(key) = key % capacity
├─ Example: h(25) = 25 % 10 = 5
└─ Places key 25 at index 5

For String Keys:
├─ Method 1: Sum of ASCII values
│  ├─ "cat" = (99 + 97 + 116) % 10 = 312 % 10 = 2
│  └─ Places at index 2
│
├─ Method 2: Polynomial rolling hash
│  ├─ h = (c₀×p⁰ + c₁×p¹ + c₂×p²) % m
│  └─ More sophisticated, better distribution
│
└─ Method 3: DJB2 algorithm
   ├─ h = 5381
   ├─ For each char c: h = ((h << 5) + h) + c
   └─ Industry standard for strings
```

#### Hash Function Visualization

```
Hash Table with Capacity = 10

Keys: 12, 35, 27, 42, 18, 55

Hash Function: h(key) = key % 10

┌────────────────────────────────────────────┐
│ Index │ Key │ Distribution                 │
├────────────────────────────────────────────┤
│   0   │  -  │ ░░░░░░░░░░░░░░░░░░░░       │
│   1   │  -  │ ░░░░░░░░░░░░░░░░░░░░       │
│   2   │  12 │ ██████ (12 % 10 = 2)       │
│   3   │  -  │ ░░░░░░░░░░░░░░░░░░░░       │
│   4   │  -  │ ░░░░░░░░░░░░░░░░░░░░       │
│   5   │ 35  │ ████████ (35 % 10 = 5)     │
│   6   │ -   │ ░░░░░░░░░░░░░░░░░░░░       │
│   7   │ 27  │ ███████ (27 % 10 = 7)      │
│   8   │ 18  │ █████ (18 % 10 = 8)        │
│   9   │ -   │ ░░░░░░░░░░░░░░░░░░░░       │
│  10   │ 42  │ ████ (42 % 10 = 2) WAIT... │
└────────────────────────────────────────────┘

Wait! 42 % 10 = 2, but 12 is already at index 2!
This is a COLLISION!
```

### 2. Collision

**Definition:** When two different keys hash to the same index

```
Hash Table Example:

Capacity: 5
Keys: 12, 17, 22, 27, 32

Hash Function: h(key) = key % 5

Results:
├─ 12 % 5 = 2 → Index 2
├─ 17 % 5 = 2 → Index 2 (COLLISION!)
├─ 22 % 5 = 2 → Index 2 (COLLISION!)
├─ 27 % 5 = 2 → Index 2 (COLLISION!)
└─ 32 % 5 = 2 → Index 2 (COLLISION!)

All 5 keys hash to same index! This is bad!

┌─────────────────────────────────┐
│ Index │ Content                 │
├─────────────────────────────────┤
│   0   │ NULL                    │
│   1   │ NULL                    │
│   2   │ 12→17→22→27→32 (chain) │
│   3   │ NULL                    │
│   4   │ NULL                    │
└─────────────────────────────────┘

Now searching is O(5) instead of O(1)!
```

### 3. Load Factor

**Definition:** α = n / m (number of entries / table capacity)

```
Impact of Load Factor:

Load Factor < 0.25:
├─ Lots of empty space
├─ Few collisions
├─ Fast operations O(1)
└─ Memory wasteful

Load Factor = 0.5 (GOOD):
├─ Balanced space usage
├─ Manageable collisions
├─ Average case O(1 + α) ≈ O(1)
└─ Recommended threshold

Load Factor = 0.75 (TIME TO RESIZE):
├─ Many collisions expected
├─ Longer chains
├─ Average case O(1 + α) slowing down
└─ Should trigger rehashing

Load Factor > 1.0:
├─ More entries than table size
├─ With chaining: possible, chains get long
├─ Performance degrades significantly
└─ Must resize immediately
```

---

## Hash Function Design

### Good vs Bad Hash Functions

#### Bad Hash Function Example

```
Capacity: 10

Function: h(key) = key % 10

Keys: 10, 20, 30, 40, 50, 60, 70, 80, 90, 100

All keys are multiples of 10:
├─ 10 % 10 = 0
├─ 20 % 10 = 0
├─ 30 % 10 = 0
├─ 40 % 10 = 0
└─ ... all hash to index 0!

Result: Single chain of length 10
┌───────────────────────────┐
│ Idx │ Content             │
├───────────────────────────┤
│  0  │ 10→20→30→...→100   │
│  1  │ NULL                │
│  2  │ NULL                │
│  ... (all NULL)           │
└───────────────────────────┘

Performance: O(n) for search!
Problem: Poor distribution for multiples
```

#### Good Hash Function Example

```
Capacity: 10

Function: h(key) = (key * 2654435761) % 2^32 % capacity
(Knuth's multiplicative hash)

Keys: 10, 20, 30, 40, 50, 60, 70, 80, 90, 100

Distribution:
├─ h(10) = 9 (spread!)
├─ h(20) = 8 (spread!)
├─ h(30) = 7 (spread!)
├─ h(40) = 6 (spread!)
├─ h(50) = 5 (spread!)
├─ h(60) = 4 (spread!)
├─ h(70) = 3 (spread!)
├─ h(80) = 2 (spread!)
├─ h(90) = 1 (spread!)
└─ h(100) = 0 (spread!)

Result: All indices used!
┌──────────────────┐
│ Idx │ Content    │
├──────────────────┤
│  0  │ 100        │
│  1  │ 90         │
│  2  │ 80         │
│  3  │ 70         │
│  ... (each has one entry)
│  9  │ 10         │
└──────────────────┘

Performance: O(1) for search!
Benefit: Even distribution
```

### Common Hash Functions in Practice

#### 1. Division Method
```cpp
h(key) = key % m

Pros: Simple, fast
Cons: Poor for certain key patterns
Use: Quick implementations
```

#### 2. Multiplication Method
```cpp
h(key) = floor(m * (A * key mod 1))
where A ≈ 0.618 (golden ratio)

Pros: Good distribution
Cons: More complex
Use: Production systems
```

#### 3. Folding Method (for large numbers)
```cpp
// Example: h(123456789) with m = 10000
// Split into chunks: 123, 456, 789
// Sum: 123 + 456 + 789 = 1368
// h = 1368 % 10000 = 1368

Pros: Works for large numbers
Cons: Can lose information
Use: Phone numbers, IDs
```

#### 4. Mid-Square Method
```cpp
// Square the key and take middle digits
// h(123) with m = 1000
// 123² = 15129
// Middle 3 digits: 512
// h = 512

Pros: Good randomness
Cons: Slow (requires squaring)
Use: Scientific applications
```

---

## Collision Resolution Techniques

### Method 1: Chaining (Used in Our Implementation)

**How It Works:**
Each table index stores a linked list (chain) of entries that hash to that index.

```
Hash Table (Capacity = 5):

Insert: 12, 7, 22, 17, 29

h(key) = key % 5

├─ h(12) = 2 → Index 2
├─ h(7)  = 2 → Index 2 (COLLISION)
├─ h(22) = 2 → Index 2 (COLLISION)
├─ h(17) = 2 → Index 2 (COLLISION)
└─ h(29) = 4 → Index 4

Table Structure:
┌────────────────────────────────────────┐
│ Index │ Chain                          │
├────────────────────────────────────────┤
│   0   │ NULL                           │
│   1   │ NULL                           │
│   2   │ 29→22→17→7→12 → NULL          │
│   3   │ NULL                           │
│   4   │ 29 → NULL                     │
└────────────────────────────────────────┘

Actually: (correcting above)
Index 2: 29→22→17→7→12→NULL

Wait, let me recalculate:
├─ 12 % 5 = 2
├─ 7 % 5 = 2
├─ 22 % 5 = 2
├─ 17 % 5 = 2
└─ 29 % 5 = 4

So:
┌────────────────────────────────────────┐
│ Index │ Chain                          │
├────────────────────────────────────────┤
│   0   │ NULL                           │
│   1   │ NULL                           │
│   2   │ 17→22→7→12→NULL (insertion order reversed)
│   3   │ NULL                           │
│   4   │ 29→NULL                       │
└────────────────────────────────────────┘
```

#### Insertion with Chaining

```
INSERT(key=17, value="Alice")

Step 1: Compute index
├─ h(17) = 17 % 5 = 2

Step 2: Check if key exists (update vs insert)
├─ Go to table[2]
├─ Traverse chain: 
│  ├─ Node 1: key=12? No
│  ├─ Node 2: key=7? No
│  ├─ Node 3: key=22? No
│  └─ Reach NULL: Key not found

Step 3: Insert new node at beginning of chain
├─ Create newNode: (17, "Alice")
├─ newNode.next = table[2]
├─ table[2] = newNode
├─ Result: table[2] = (17,"Alice")→(22,...)→(7,...)→(12,...)→NULL

Time: O(1 + average chain length)
     = O(1 + α) where α = load factor
```

#### Search with Chaining

```
SEARCH(key=22)

Step 1: Compute index
├─ h(22) = 22 % 5 = 2

Step 2: Traverse chain at index 2
├─ Node 1: key=17? No, move to next
├─ Node 2: key=22? YES! Found it ✓
├─ Return associated value

Time: O(length of chain)
     = O(1 + α) average case
     = O(n) worst case (all in one chain)
```

#### Delete with Chaining

```
DELETE(key=7)

Step 1: Compute index
├─ h(7) = 7 % 5 = 2

Step 2: Traverse chain, keeping track of previous
├─ Node 1: key=17? No, prev=nil, curr=node1
├─ Node 2: key=22? No, prev=node1, curr=node2
├─ Node 3: key=7? YES! Found it
│  └─ prev.next = curr.next
│  └─ Free memory of curr
│  └─ Result: table[2] = (17,...)→(22,...)→(12,...)→NULL

Step 3: Update size
├─ size--

Time: O(length of chain)
     = O(1 + α) average case
```

### Method 2: Open Addressing (Alternative Approach)

**How It Works:**
If position is occupied, probe for another empty position.

#### Linear Probing
```
h(key, i) = (h(key) + i) % m

Where i = 0, 1, 2, ... (probe sequence)

Example: Insert 12, 7, 22, 17 (capacity = 5, h(key) = key % 5)

Insert 12:
├─ h(12, 0) = 2 % 5 = 2 → Empty, insert at 2

Insert 7:
├─ h(7, 0) = 2 % 5 = 2 → Occupied!
├─ h(7, 1) = 3 % 5 = 3 → Empty, insert at 3

Insert 22:
├─ h(22, 0) = 2 % 5 = 2 → Occupied!
├─ h(22, 1) = 3 % 5 = 3 → Occupied!
├─ h(22, 2) = 4 % 5 = 4 → Empty, insert at 4

Insert 17:
├─ h(17, 0) = 2 % 5 = 2 → Occupied!
├─ h(17, 1) = 3 % 5 = 3 → Occupied!
├─ h(17, 2) = 4 % 5 = 4 → Occupied!
├─ h(17, 3) = 0 % 5 = 0 → Empty, insert at 0

Table:
┌─────────────────────┐
│ Idx │ Value | Probes│
├─────────────────────┤
│  0  │  17   │   4   │
│  1  │ NULL  │   -   │
│  2  │  12   │   1   │
│  3  │   7   │   2   │
│  4  │  22   │   3   │
└─────────────────────┘

Problem: Clustering
├─ Keys pile up in same area
└─ Increases probes needed
```

#### Quadratic Probing
```
h(key, i) = (h(key) + i²) % m

Reduces clustering but more complex
└─ Uses: (0, 1, 4, 9, 16, ...)
```

#### Double Hashing
```
h(key, i) = (h₁(key) + i × h₂(key)) % m

Uses second hash function to determine probe sequence
└─ Best clustering behavior among open addressing methods
```

### Chaining vs Open Addressing

```
┌──────────────────────────────────────────────────────────┐
│ Aspect         │ Chaining      │ Open Addressing       │
├──────────────────────────────────────────────────────────┤
│ Space          │ O(n + m)      │ O(m) where m ≥ n     │
│ Cache-friendly │ No (pointers) │ Yes (sequential)      │
│ Deletion       │ Easy          │ Complex (tombstones)  │
│ Load Factor    │ Can exceed 1  │ Must be < 1           │
│ Implementation │ Simple        │ More complex          │
│ Best use       │ General       │ Cache-sensitive       │
└──────────────────────────────────────────────────────────┘
```

---

## Implementation Details

### Class Structure

```cpp
CLASS HashMap
    PRIVATE:
        ├─ Node[] table          // Array of linked lists
        ├─ capacity             // Size of table
        ├─ size                 // Number of entries
        └─ hashFunction()       // Compute index
    
    PUBLIC:
        ├─ INSERT(key, value)   // Add/update entry
        ├─ SEARCH(key)          // Find value by key
        ├─ DELETE(key)          // Remove entry
        ├─ DISPLAY()            // Print all entries
        └─ CLEAR()              // Remove all entries
```

### Node Structure for Chaining

```cpp
STRUCT Node<K, V>
    ├─ key: K
    ├─ value: V
    └─ next: Node<K, V>*  // Pointer to next node in chain
```

### Why Generic Templates?

```cpp
// Without templates:
class StringHashMap
    INSERT(string, string)
    SEARCH(string)
    DELETE(string)

class IntHashMap
    INSERT(int, int)
    SEARCH(int)
    DELETE(int)

// Repetitive, maintenance nightmare!

// With templates:
template <typename K, typename V>
class HashMap
    INSERT(K, V)
    SEARCH(K)
    DELETE(K)

// Works for ANY key and value types!
```

---

## Operations - Deep Dive

### Operation 1: INSERT

#### Algorithm

```
INSERT(key, value)
    ├─ Step 1: Compute hash index
    │  └─ index = HASH_FUNCTION(key)
    │
    ├─ Step 2: Traverse chain to check if key exists
    │  ├─ node = table[index]
    │  └─ WHILE node != NULL
    │     ├─ IF node.key == key
    │     │  └─ UPDATE: node.value = value, RETURN
    │     ├─ ELSE
    │     │  └─ node = node.next
    │     └─ END WHILE
    │
    └─ Step 3: Key doesn't exist, add new node
       ├─ CREATE newNode(key, value)
       ├─ newNode.next = table[index]
       ├─ table[index] = newNode
       └─ size++
```

#### Dry Run: Insert Operations

```
HashMap<String, String> with capacity = 5

Operation 1: INSERT("John", "Engineer")
├─ hash("John") = (74+111+104+110) % 5 = 399 % 5 = 4
├─ table[4] is empty
├─ Create node: ("John", "Engineer")
├─ table[4]: [John→Engineer] → NULL
├─ size = 1

Operation 2: INSERT("Sarah", "Manager")
├─ hash("Sarah") = (83+97+114+97+104) % 5 = 495 % 5 = 0
├─ table[0] is empty
├─ Create node: ("Sarah", "Manager")
├─ table[0]: [Sarah→Manager] → NULL
├─ size = 2

Operation 3: INSERT("Mike", "Developer")
├─ hash("Mike") = (77+105+107+101) % 5 = 390 % 5 = 0 (COLLISION!)
├─ table[0] is not empty
├─ Create node: ("Mike", "Developer")
├─ table[0]: [Mike→Developer] → [Sarah→Manager] → NULL
├─ size = 3

Operation 4: INSERT("Tom", "Analyst")
├─ hash("Tom") = (84+111+109) % 5 = 304 % 5 = 4 (COLLISION!)
├─ table[4] is not empty
├─ Create node: ("Tom", "Analyst")
├─ table[4]: [Tom→Analyst] → [John→Engineer] → NULL
├─ size = 4

Operation 5: INSERT("John", "Senior Engineer") [UPDATE]
├─ hash("John") = 4
├─ Traverse chain at table[4]:
│  ├─ First node: key="Tom"? No
│  ├─ Second node: key="John"? YES!
│  └─ UPDATE: value = "Senior Engineer"
├─ size = 4 (no change)

Final State:
┌──────────────────────────────────────────┐
│ Index │ Chain                            │
├──────────────────────────────────────────┤
│   0   │ Mike→Dev → Sarah→Mgr → NULL     │
│   1   │ NULL                             │
│   2   │ NULL                             │
│   3   │ NULL                             │
│   4   │ Tom→Analyst → John→Sr.Eng → NULL│
└──────────────────────────────────────────┘

Load Factor: 4/5 = 0.8
```

### Operation 2: SEARCH

#### Algorithm

```
SEARCH(key)
    ├─ Step 1: Compute hash index
    │  └─ index = HASH_FUNCTION(key)
    │
    ├─ Step 2: Traverse chain at this index
    │  ├─ node = table[index]
    │  └─ WHILE node != NULL
    │     ├─ IF node.key == key
    │     │  └─ RETURN node.value
    │     ├─ ELSE
    │     │  └─ node = node.next
    │     └─ END WHILE
    │
    └─ Step 3: Key not found
       └─ RETURN NULL or exception
```

#### Dry Run: Search Operations

```
Using HashMap from previous example

Search 1: SEARCH("John")
├─ hash("John") = 4
├─ Go to table[4]
├─ Traverse chain:
│  ├─ Node 1: key="Tom"? No
│  ├─ Node 2: key="John"? YES! ✓
│  └─ Return "Senior Engineer"
├─ Steps taken: 2
├─ Status: FOUND

Search 2: SEARCH("Mike")
├─ hash("Mike") = 0
├─ Go to table[0]
├─ Traverse chain:
│  ├─ Node 1: key="Mike"? YES! ✓
│  └─ Return "Developer"
├─ Steps taken: 1
├─ Status: FOUND

Search 3: SEARCH("Alice")
├─ hash("Alice") = (65+108+105+99+101) % 5 = 478 % 5 = 3
├─ Go to table[3]
├─ table[3] is empty (NULL)
├─ Steps taken: 1
├─ Status: NOT FOUND

Search 4: SEARCH("Sarah")
├─ hash("Sarah") = 0
├─ Go to table[0]
├─ Traverse chain:
│  ├─ Node 1: key="Mike"? No
│  ├─ Node 2: key="Sarah"? YES! ✓
│  └─ Return "Manager"
├─ Steps taken: 2
├─ Status: FOUND

Average steps: (2 + 1 + 1 + 2) / 4 = 1.5
```

### Operation 3: DELETE

#### Algorithm

```
DELETE(key)
    ├─ Step 1: Compute hash index
    │  └─ index = HASH_FUNCTION(key)
    │
    ├─ Step 2: Traverse chain, keeping previous pointer
    │  ├─ node = table[index]
    │  ├─ prev = NULL
    │  └─ WHILE node != NULL
    │     ├─ IF node.key == key
    │     │  ├─ IF prev == NULL  // Deleting first node
    │     │  │  └─ table[index] = node.next
    │     │  ├─ ELSE
    │     │  │  └─ prev.next = node.next
    │     │  ├─ FREE(node)
    │     │  ├─ size--
    │     │  └─ RETURN true
    │     ├─ ELSE
    │     │  ├─ prev = node
    │     │  └─ node = node.next
    │     └─ END WHILE
    │
    └─ Step 3: Key not found
       └─ RETURN false
```

#### Dry Run: Delete Operations

```
Starting state from previous examples:
┌──────────────────────────────────────────┐
│ Index │ Chain                            │
├──────────────────────────────────────────┤
│   0   │ Mike→Dev → Sarah→Mgr → NULL     │
│   1   │ NULL                             │
│   2   │ NULL                             │
│   3   │ NULL                             │
│   4   │ Tom→Analyst → John→Sr.Eng → NULL│
└──────────────────────────────────────────┘

Delete 1: DELETE("Tom")
├─ hash("Tom") = 4
├─ Go to table[4]
├─ Traverse:
│  ├─ prev = NULL, node = first node
│  ├─ node.key = "Tom"? YES!
│  ├─ prev == NULL (first node), so:
│  │  └─ table[4] = node.next
│  │  └─ table[4]: [John→Sr.Eng] → NULL
│  ├─ FREE(node)
│  ├─ size = 3
│  └─ RETURN true ✓

Delete 2: DELETE("Sarah")
├─ hash("Sarah") = 0
├─ Go to table[0]
├─ Traverse:
│  ├─ prev = NULL, node = first node ("Mike")
│  ├─ node.key = "Mike"? No
│  ├─ prev = node ("Mike"), node = second node ("Sarah")
│  ├─ node.key = "Sarah"? YES!
│  ├─ prev != NULL, so:
│  │  └─ prev.next = node.next (null)
│  │  └─ table[0]: [Mike→Dev] → NULL
│  ├─ FREE(node)
│  ├─ size = 2
│  └─ RETURN true ✓

Delete 3: DELETE("Bob")
├─ hash("Bob") = (66+111+98) % 5 = 275 % 5 = 0
├─ Go to table[0]
├─ Traverse:
│  ├─ Only "Mike" at table[0]
│  ├─ "Mike" != "Bob"
│  ├─ node = NULL
│  ├─ Loop ends
│  └─ RETURN false (NOT FOUND)

Final state after deletions:
┌──────────────────────────────────────────┐
│ Index │ Chain                            │
├──────────────────────────────────────────┤
│   0   │ Mike→Developer → NULL            │
│   1   │ NULL                             │
│   2   │ NULL                             │
│   3   │ NULL                             │
│   4   │ John→Sr.Engineer → NULL          │
└──────────────────────────────────────────┘

Load Factor: 2/5 = 0.4
```

### Operation 4: DISPLAY

#### Algorithm

```
DISPLAY()
    ├─ FOR each index i from 0 to capacity-1
    │  ├─ IF table[i] is not NULL
    │  │  ├─ PRINT "Index {i}: "
    │  │  ├─ node = table[i]
    │  │  ├─ WHILE node != NULL
    │  │  │  ├─ PRINT "[{key} → {value}]"
    │  │  │  ├─ IF node.next != NULL
    │  │  │  │  └─ PRINT " → "
    │  │  │  └─

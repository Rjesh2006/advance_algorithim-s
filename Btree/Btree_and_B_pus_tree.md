
# B-Tree and B+Tree

### Theory Overview

**What is a B-Tree?**

A B-Tree is a self-balancing search tree designed to work well with large datasets stored on disk. It maintains sorted data and allows searches, sequential access, insertions, and deletions in logarithmic time.

**Key Properties:**
- Every node has at most `2t-1` keys and `2t` children (where `t` is the minimum degree)
- Every non-leaf node (except root) has at least `t-1` keys
- The root has at least 1 key
- All leaves are at the same level
- A non-leaf node with k keys has k+1 children

**B+Tree:** Similar to B-Tree but:
- Stores data only in leaf nodes
- Internal nodes contain only keys for navigation
- Leaves are linked together (supports range queries efficiently)

### How It Works

#### Insertion Process

1. **Start at root** - Check if node is full (has 2t-1 keys)
2. **If full, split** - Divide node into two, push middle key up to parent
3. **Traverse down** - Move to appropriate child based on key comparison
4. **Insert at leaf** - Place key in sorted order in the leaf node

#### Search Process

1. Start at root
2. Compare search key with node keys
3. If match found, return node
4. If key < node's first key, go to first child
5. Find appropriate child and recurse
6. If leaf reached and no match, key doesn't exist

#### Split Operation

When a node has `2t-1` keys (full) and needs insertion:
- Find middle key at position `t-1`
- Create new right node
- Move keys from position `t` to `2t-1` to new node
- Push middle key to parent
- Recursively split parent if needed

### Pseudocode

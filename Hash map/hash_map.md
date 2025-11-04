
# Hash Map

### Theory Overview

**What is a Hash Map?**

A hash map is a data structure that implements an associative array - a structure that maps keys to values. Uses a hash function to compute index into array of buckets or slots.

**Key Operations:**
- **Insert:** Add key-value pair
- **Search:** Find value by key
- **Delete:** Remove key-value pair
- **Update:** Modify value for existing key

**Why Hash Maps?**
- Fast average lookup: O(1) instead of O(n) for arrays
- Flexible key types
- Widely used in real applications

### Collision Handling: Chaining

**What is Collision?**
When two different keys hash to the same index.

**Chaining Solution:**
Store linked list at each hash index. Multiple key-value pairs can coexist at same index.

#### Example:

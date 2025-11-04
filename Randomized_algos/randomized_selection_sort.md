# Randomized Selection Sort

### Theory Overview

**What is Selection Sort?**

Selection sort divides the array into two parts: sorted and unsorted. Repeatedly finds the minimum element from unsorted part and places it in sorted part.

**Why Randomized?**

Normal selection sort searches sequentially. Randomized version uses randomized partitioning (like QuickSort) to find the k-th smallest element efficiently.

**Key Idea:**
- Use randomized partition to find k-th smallest element
- Place it at position k-1
- Repeat for remaining elements
- Average Time: O(n²), but often faster than standard selection sort

### How It Works

#### Step-by-Step Process

1. **For each position i (0 to n-1):**
   - Find the minimum element in range [i, n-1]
   - Use randomized selection to find i-th smallest element
   - Swap it to position i

2. **Randomized Selection:**
   - Pick random pivot index
   - Partition around pivot
   - If pivot position = target k, return that element
   - If k < pivot position, search left partition
   - If k > pivot position, search right partition

#### Partitioning Process
rea

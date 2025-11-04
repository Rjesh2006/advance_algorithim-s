# Randomized Quick Sort

### Theory Overview

**What is Quick Sort?**

Quick sort is a divide-and-conquer sorting algorithm that:
- Selects a pivot element
- Partitions array so elements < pivot are on left, > pivot on right
- Recursively sorts both partitions

**Why Randomized?**

Normal QuickSort can degrade to O(n²) on sorted arrays (always picking worst pivot). Randomized version picks a random pivot, avoiding this worst case.

**Key Advantages:**
- Average case: O(n log n)
- In-place sorting (O(log n) space for recursion)
- Better performance than MergeSort in practice
- Cache-friendly


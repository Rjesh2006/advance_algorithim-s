
// ============================================================================
// Randomized Selection Sort
// ============================================================================
// Selection Sort: Find the minimum element and place it at the beginning
// Randomized: Use random pivot for optimization (similar to randomized QuickSort)
// Time Complexity: O(n²) worst case, O(n) best case
// Space Complexity: O(1)

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// ============================================================================
// RANDOMIZED PARTITION (Used in randomized selection)
// ============================================================================

// Partition array around a random pivot and return its position
int randomizedPartition(vector<int>& arr, int low, int high) {
    // Pick a random index between low and high
    int randomIndex = low + rand() % (high - low + 1);

    // Swap random element with last element
    swap(arr[randomIndex], arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    // Partition: move all smaller elements to left
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    // Place pivot in correct position
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// ============================================================================
// RANDOMIZED SELECTION (Find k-th smallest element)
// ============================================================================

// Find the k-th smallest element (0-indexed)
int randomizedSelect(vector<int>& arr, int low, int high, int k) {
    // Base case: if only one element
    if (low == high)
        return arr[low];

    // Partition around random pivot
    int pivotIndex = randomizedPartition(arr, low, high);

    // If k is at pivot position, we found it
    if (k == pivotIndex)
        return arr[k];
    // If k is on left side
    else if (k < pivotIndex)
        return randomizedSelect(arr, low, pivotIndex - 1, k);
    // If k is on right side
    else
        return randomizedSelect(arr, pivotIndex + 1, high, k);
}

// ============================================================================
// RANDOMIZED SELECTION SORT
// ============================================================================

void randomizedSelectionSort(vector<int>& arr) {
    int n = arr.size();

    // For each position from start to end
    for (int i = 0; i < n - 1; i++) {
        // Find the (i)-th smallest element in arr[i...n-1]
        vector<int> temp(arr.begin() + i, arr.end());
        int kthSmallest = randomizedSelect(temp, 0, temp.size() - 1, 0);

        // Find position of kthSmallest in original array from position i
        int pos = i;
        while (pos < n && arr[pos] != kthSmallest)
            pos++;

        // Swap kthSmallest to position i
        swap(arr[i], arr[pos]);
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Print array elements
void printArray(const vector<int>& arr) {
    for (int x : arr)
        cout << x << " ";
    cout << "\n";
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    srand(time(0)); // Seed for random number generation

    cout << "=== Randomized Selection Sort ===\n\n";

    // Test case 1
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    cout << "Original array: ";
    printArray(arr1);

    randomizedSelectionSort(arr1);

    cout << "Sorted array:   ";
    printArray(arr1);
    cout << "\n";

    // Test case 2
    vector<int> arr2 = {5, 2, 8, 1, 9};
    cout << "Original array: ";
    printArray(arr2);

    randomizedSelectionSort(arr2);

    cout << "Sorted array:   ";
    printArray(arr2);
    cout << "\n";

    // Test case 3
    vector<int> arr3 = {1};
    cout << "Original array: ";
    printArray(arr3);

    randomizedSelectionSort(arr3);

    cout << "Sorted array:   ";
    printArray(arr3);

    return 0;
}

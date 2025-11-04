

// ============================================================================
// Randomized Quick Sort
// ============================================================================
// Quick Sort: Divide and conquer sorting algorithm using pivot
// Randomized: Pick random pivot to avoid worst-case on sorted arrays
// Time Complexity: O(n log n) average, O(n²) worst case
// Space Complexity: O(log n) for recursion stack

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// ============================================================================
// RANDOMIZED PARTITION
// ============================================================================

// Partition array and return pivot position
// All elements < pivot go to left, >= pivot go to right
int randomizedPartition(vector<int>& arr, int low, int high) {
    // Pick a random index between low and high
    int randomIndex = low + rand() % (high - low + 1);

    // Swap random element with last element (to use as pivot)
    swap(arr[randomIndex], arr[high]);

    int pivot = arr[high]; // Last element is now pivot
    int i = low - 1;       // Index for smaller elements

    // Move all elements smaller than pivot to the left
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    // Place pivot in its correct sorted position
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// ============================================================================
// RANDOMIZED QUICK SORT RECURSIVE FUNCTION
// ============================================================================

void randomizedQuickSortUtil(vector<int>& arr, int low, int high) {
    // Base case: if array has more than one element
    if (low < high) {
        // Partition and get pivot position
        int pi = randomizedPartition(arr, low, high);

        // Recursively sort left partition (elements < pivot)
        randomizedQuickSortUtil(arr, low, pi - 1);

        // Recursively sort right partition (elements > pivot)
        randomizedQuickSortUtil(arr, pi + 1, high);
    }
}

// ============================================================================
// RANDOMIZED QUICK SORT WRAPPER FUNCTION
// ============================================================================

void randomizedQuickSort(vector<int>& arr) {
    if (arr.empty())
        return;

    randomizedQuickSortUtil(arr, 0, arr.size() - 1);
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

// Verify if array is sorted
bool isSorted(const vector<int>& arr) {
    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i + 1])
            return false;
    }
    return true;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    srand(time(0)); // Seed for random number generation

    cout << "=== Randomized Quick Sort ===\n\n";

    // Test case 1: Random array
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90};
    cout << "Test 1 - Original array:    ";
    printArray(arr1);

    randomizedQuickSort(arr1);

    cout << "After quick sort:           ";
    printArray(arr1);
    cout << "Is sorted? " << (isSorted(arr1) ? "Yes" : "No") << "\n\n";

    // Test case 2: Already sorted array (worst case for normal quicksort)
    vector<int> arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "Test 2 - Original array:    ";
    printArray(arr2);

    randomizedQuickSort(arr2);

    cout << "After quick sort:           ";
    printArray(arr2);
    cout << "Is sorted? " << (isSorted(arr2) ? "Yes" : "No") << "\n\n";

    // Test case 3: Reverse sorted array
    vector<int> arr3 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    cout << "Test 3 - Original array:    ";
    printArray(arr3);

    randomizedQuickSort(arr3);

    cout << "After quick sort:           ";
    printArray(arr3);
    cout << "Is sorted? " << (isSorted(arr3) ? "Yes" : "No") << "\n\n";

    // Test case 4: Array with duplicates
    vector<int> arr4 = {5, 2, 8, 2, 9, 1, 5, 5};
    cout << "Test 4 - Original array:    ";
    printArray(arr4);

    randomizedQuickSort(arr4);

    cout << "After quick sort:           ";
    printArray(arr4);
    cout << "Is sorted? " << (isSorted(arr4) ? "Yes" : "No") << "\n\n";

    // Test case 5: Single element
    vector<int> arr5 = {42};
    cout << "Test 5 - Original array:    ";
    printArray(arr5);

    randomizedQuickSort(arr5);

    cout << "After quick sort:           ";
    printArray(arr5);
    cout << "Is sorted? " << (isSorted(arr5) ? "Yes" : "No") << "\n";

    return 0;
}

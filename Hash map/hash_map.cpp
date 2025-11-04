
// ============================================================================
// Hash Map Implementation with Collision Handling
// ============================================================================
// Custom hash map with chaining for collision resolution
// Uses linked lists to handle collisions
// Supports insert, search, delete, and display operations
// Time Complexity: O(1) average, O(n) worst case
// Space Complexity: O(n)

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

// ============================================================================
// NODE STRUCTURE FOR LINKED LIST (Chaining)
// ============================================================================

template <typename K, typename V>
struct Node {
    K key;           // Key of the pair
    V value;         // Value of the pair
    Node* next;      // Pointer to next node

    Node(K k, V v) : key(k), value(v), next(nullptr) {}
};

// ============================================================================
// HASH MAP CLASS
// ============================================================================

template <typename K, typename V>
class HashMap {
private:
    static const int DEFAULT_CAPACITY = 10;

    vector<Node<K, V>*> table;     // Array of linked lists
    int capacity;                  // Size of the hash table
    int size;                      // Number of key-value pairs

    // Hash function: converts key to index
    int hashFunction(K key) {
        // Simple hash function using modulo
        // For string, use sum of ASCII values
        int hashCode = 0;

        if (is_same<K, string>::value) {
            string s = reinterpret_cast<string&>(key);
            for (char c : s) {
                hashCode += (int)c;
            }
        } else {
            // For numeric types
            hashCode = (int)key;
        }

        return abs(hashCode) % capacity;
    }

public:
    // Constructor: Initialize hash map
    HashMap() : capacity(DEFAULT_CAPACITY), size(0) {
        table.resize(capacity, nullptr);
    }

    // Insert or update a key-value pair
    void insert(K key, V value) {
        int index = hashFunction(key);
        Node<K, V>* node = table[index];

        // Check if key already exists (update case)
        while (node != nullptr) {
            if (node->key == key) {
                node->value = value;
                cout << "Updated key: " << key << " with value: " << value << "\n";
                return;
            }
            node = node->next;
        }

        // Key doesn't exist, add new node at the beginning
        Node<K, V>* newNode = new Node<K, V>(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;

        cout << "Inserted: " << key << " -> " << value << "\n";
    }

    // Search for a key and return its value
    V* search(K key) {
        int index = hashFunction(key);
        Node<K, V>* node = table[index];

        // Traverse the linked list at this index
        while (node != nullptr) {
            if (node->key == key) {
                return &(node->value);
            }
            node = node->next;
        }

        return nullptr; // Key not found
    }

    // Delete a key-value pair
    bool deleteKey(K key) {
        int index = hashFunction(key);
        Node<K, V>* node = table[index];
        Node<K, V>* prev = nullptr;

        // Find and delete the node
        while (node != nullptr) {
            if (node->key == key) {
                if (prev == nullptr) {
                    table[index] = node->next;
                } else {
                    prev->next = node->next;
                }
                delete node;
                size--;
                cout << "Deleted key: " << key << "\n";
                return true;
            }
            prev = node;
            node = node->next;
        }

        cout << "Key not found: " << key << "\n";
        return false;
    }

    // Display all key-value pairs
    void display() {
        cout << "\n=== Hash Map Contents ===\n";
        int count = 0;

        for (int i = 0; i < capacity; i++) {
            Node<K, V>* node = table[i];

            if (node != nullptr) {
                cout << "Index " << i << ": ";

                // Traverse linked list at this index
                while (node != nullptr) {
                    cout << "[" << node->key << " -> " << node->value << "]";

                    if (node->next != nullptr)
                        cout << " -> ";

                    node = node->next;
                    count++;
                }
                cout << "\n";
            }
        }

        cout << "Total elements: " << size << "\n";
        cout << "Load factor: " << (double)size / capacity << "\n\n";
    }

    // Get the size
    int getSize() {
        return size;
    }

    // Clear the hash map
    void clear() {
        for (int i = 0; i < capacity; i++) {
            Node<K, V>* node = table[i];

            while (node != nullptr) {
                Node<K, V>* temp = node;
                node = node->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        size = 0;
        cout << "Hash map cleared!\n\n";
    }

    // Destructor
    ~HashMap() {
        clear();
    }
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    cout << "=== Hash Map Implementation ===\n\n";

    // Test case 1: Integer keys with Integer values
    cout << "Test 1: Integer -> Integer Hash Map\n";
    cout << "================================\n";

    HashMap<int, int> map1;

    map1.insert(1, 100);
    map1.insert(2, 200);
    map1.insert(11, 1100);  // Will collide with 1 (same hash index)
    map1.insert(3, 300);
    map1.insert(13, 1300);  // Will collide with 3
    map1.insert(5, 500);

    map1.display();

    cout << "Searching for key 11: ";
    int* result = map1.search(11);
    if (result != nullptr)
        cout << "Found! Value = " << *result << "\n\n";
    else
        cout << "Not found!\n\n";

    map1.deleteKey(2);
    map1.display();

    cout << "\n";

    // Test case 2: String keys with String values
    cout << "Test 2: String -> String Hash Map\n";
    cout << "=================================\n";

    HashMap<string, string> map2;

    map2.insert("Alice", "Engineer");
    map2.insert("Bob", "Designer");
    map2.insert("Charlie", "Manager");
    map2.insert("Diana", "Developer");
    map2.insert("Eve", "Analyst");

    map2.display();

    cout << "Searching for key 'Bob': ";
    string* result2 = map2.search("Bob");
    if (result2 != nullptr)
        cout << "Found! Value = " << *result2 << "\n\n";
    else
        cout << "Not found!\n\n";

    map2.deleteKey("Charlie");
    map2.display();

    cout << "\n";

    // Test case 3: String keys with Integer values
    cout << "Test 3: String -> Integer Hash Map\n";
    cout << "==================================\n";

    HashMap<string, int> map3;

    map3.insert("Apple", 50);
    map3.insert("Banana", 30);
    map3.insert("Orange", 40);
    map3.insert("Grape", 60);
    map3.insert("Mango", 45);

    map3.display();

    cout << "Searching for key 'Grape': ";
    int* result3 = map3.search("Grape");
    if (result3 != nullptr)
        cout << "Found! Value = " << *result3 << "\n\n";
    else
        cout << "Not found!\n\n";

    map3.deleteKey("Banana");
    map3.insert("Grape", 65);  // Update existing key
    map3.display();

    return 0;
}

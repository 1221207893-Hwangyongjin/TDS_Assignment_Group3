#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

// ==================== Separate Chaining Hash Table Implementation ====================

// Structure for each node in the hash table's linked list (chain)
struct HashNode {
    int key;            
    string name, category, size, color;
    float price;       
    
    HashNode* next;     // Pointer to the next node in the chain (for collision handling)
    
    // Constructor to initialize a new node
    HashNode(int Key, string Name, string Category, string Size, string Color, float Price){
    	key = Key;
		name = Name; 
		category = Category;
		size = Size;
		color = Color;
		price = Price;
		next = nullptr;
	} 
};

// Class for the Separate Chaining Hash Table
class SeparateHash {
private:
    int capacity;       // Size of the hash table (number of buckets)
    HashNode** table;   // Array of pointers to HashNode (the hash table itself)

public:
    // Constructor: Initializes the hash table with given size
    SeparateHash(int size) {
        capacity = size;
        table = new HashNode*[capacity](); // Allocate and initialize to nullptr
    }
    
    // Hash function: Computes the index for a given key using modulo operation
    int getHashIndex(int key) {
        return key % capacity;
    }
    
    // Insert a new key-value pair into the hash table
    void insert(int key, string name, string category, string size, string color,  float price) {
        int index = getHashIndex(key);      // Compute the bucket index
        HashNode* newNode = new HashNode(key, name, category, size, color,  price); // Create new node
        
        // Insert at the beginning of the chain (handles collisions by chaining)
        newNode->next = table[index];
        table[index] = newNode;
    }
    
    // Search for a key in the hash table
    HashNode* search(int key) {
        int index = getHashIndex(key);      // Compute the bucket index
        HashNode* current = table[index];   // Start at the head of the chain
        
        // Traverse the chain to find the key
        while (current != nullptr) {
            if (current->key == key) {
                return current; // Key found
            }
            current = current->next;
        }
        return nullptr; // Key not found
    }
    
    // Display the entire hash table (for debugging/visualization)
    void display() {
        for (int i = 0; i < capacity; i++) {
            cout << "Bucket " << i << ":";
            HashNode* current = table[i];
            while (current != nullptr) {
                cout << " -> [" << current->key << "] " << current->name;
                current = current->next;
            }
            cout << endl;
        }
    }
    
	void displayHashTable() {
    cout << "\nHash Table Contents:\n";
    cout << "------------------------------------------------------------------------------------------------------------\n";
    cout << setw(9) << "ID" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
    cout << "------------------------------------------------------------------------------------------------------------\n";
    
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            cout << setw(9) << current->key 
                 << setw(25) << current->name 
                 << setw(18) << current->category 
                 << setw(16) << current->size 
                 << setw(19) << current->color 
                 << setw(16) << "RM " << fixed << setprecision(2) << current->price 
                 << endl;
            current = current->next;
        }
    }
    cout << "------------------------------------------------------------------------------------------------------------\n";
}
};

// ==================== Product Management System ====================

// Structure for each product in the queue
struct Product {
    int key;            
	string name, category, size, color; 
	float price;
    Product* next;      // Pointer to the next product in the queue
};


class LinkedQueue {
private:
    Product *front, *rear;      // Pointers to the front and rear of the queue
    bool dataLoaded = false;    // Flag to check if data is loaded from file
    SeparateHash hashTable;    
    
public:
    // Constructor: Initializes an empty queue and a hash table with 10 buckets
    LinkedQueue() : front(nullptr), rear(nullptr), hashTable(100) {}
    
    // Check if the queue is empty
    bool empty() {
        return front == nullptr;
    }
    
    // Add a new product to the end of the queue and insert into the hash table
    void append(int id, string productName, string productCategory, string productSize, string productColor,float productPrice) { 
        Product* newProduct = new Product{id, productName, productCategory, productSize, productColor, productPrice, nullptr};
        
        if (rear != nullptr) {
            rear->next = newProduct; // Link to the end of the queue
            rear = newProduct;       // Update rear pointer
        } else { 
            front = rear = newProduct; // First element in the queue
        }
        
        // Insert into the hash table for fast lookup
        hashTable.insert(id, productName, productCategory, productSize, productColor, productPrice);
    }
    
    // Remove and return the product at the front of the queue (FIFO)
    Product serve() { 
        if (empty()) { 
            cout << "\t\tQueue is Empty";
            return Product(); // Return a default product if queue is empty
        }
        
        Product servedProduct = *front; // Copy the front product
        Product* temp = front;          // Temporary pointer to free memory
        front = front->next;           // Move front to the next product
        
        delete temp; // Free memory of the served product
        
        if (front == nullptr) 
            rear = nullptr; // Queue is now empty
        
        return servedProduct;
    }
    
    // Display all products in the queue (in insertion order)
    void printQueue() {
        if (empty()) {
            cout << "\nQueue is Empty\n";
            return;
        }

        cout << "\nQueue (Product Info in Order of Insertion):\n";
        cout << "------------------------------------------------------------------------------------------------------------\n";
        cout << setw(9) << "ID" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
        cout << "------------------------------------------------------------------------------------------------------------\n";
        
        Product* current = front;
        while (current != nullptr) {
            cout << setw(9) << current->key << setw(25) << current->name << setw(18) << current->category << setw(16) << current->size << setw(19) << current->color << setw(16) << "RM " << fixed << setprecision(2) << current->price << endl;
            current = current->next;
        }
        cout << "------------------------------------------------------------------------------------------------------------\n";
    }
    
    // Load product data from "products.txt" into the queue and hash table
    void displayFromFile() {
        if (!dataLoaded) {
            ifstream file("products.txt");
            if (!file) {
                cout << "No file found!" << endl;
                return;
            }

            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                int id;
                string name, category, size, color;
                float price;
                
                char comma;

                ss >> id >> comma;
                getline(ss >> ws, name, ',');
                getline(ss >> ws, category, ',');
                getline(ss >> ws, size, ',');
                getline(ss >> ws, color, ',');
                ss >> price;

                append(id, name, category, size, color, price); // Add to queue and hash table
            }
            dataLoaded = true; // Mark data as loaded
        }
    }
    
    // Save all products from the queue to "teamA.txt"
    void saveToFile() {
        ofstream outFile("TeamA_sorted_information.txt");
        if (!outFile) {
            cout << "Error opening file for writing!\n";
            return;
        }
    
        while (!empty()) {
            Product p = serve();
            outFile << p.key << "," << p.name << "," << p.category << "," << p.size << "," << p.color << "," <<   p.price << "\n";
        }
        outFile.close();
        cout << "Products saved to file successfully!\n";
    }

    // Search for a product by ID using the hash table
    Product* searchById(int key) {
        HashNode* result = hashTable.search(key); // Use hash table for fast lookup
        if (result != nullptr) {
            // Convert HashNode to Product for compatibility
            Product* product = new Product{result->key, result->name, result->category, result->size, result->color, result->price, nullptr};
            return product;
        }
        return nullptr; // Not found
    }
    
    // Display the contents of the hash table (for debugging/visualization)
    void displayHashTable() {
        cout << "\nHash Table Contents:\n";
        hashTable.display();
    }

		// Purpose: Allows 'teamA' to access the SeparateHash table stored inside a LinkedQueue object.  
		// Usage: Call via LinkedQueue object (e.g., `lq.classSeperateHashing().search(key)`).  
		// Returns: Reference to the LinkedQueue's internal hashTable for direct SeparateHash operations. 
        SeparateHash& classSeperateHashing() { return hashTable; }

};

// ==================== Team A: Product Management System ====================

class TeamA {
private:
    LinkedQueue lq; // Manages products using a queue + hash table
    
    // Generate a pseudo-random key based on current time
    long long int newTime() {
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);
        long long int x = (tm->tm_hour) * 10000000 + (tm->tm_min) * 100000
                        + (tm->tm_sec) * 1000 + (tm->tm_mday) * 10 + (tm->tm_year);
        return x;
    }

    // Generate a unique key for new products
    long int getKey() {
        static long long int key = newTime();
        key = key * key;

            key = key / 10000;
            key = key % 100000000;
        
        return key;
    }
    
    long int getUniqueKey() {
    long int key;
    bool keyExists;
    
    do {
        key = getKey(); // Generate a new key
        keyExists = (lq.classSeperateHashing().search(key) != nullptr); // Check if key exists
    } while (keyExists); // Repeat until we find a unique key
    
    return key;
}

public:
    // Main menu for Team A's product management system
    void mainMenu() {
        char choice;
        do {
            system("cls");
            lq.displayFromFile(); // Load data if not already loaded
            cout << "Swimming Equipment And Swimwear Management System\n";
            cout << "1. View all products from queue\n";
            cout << "2. Add product\n";
            cout << "3. Search by ID\n";
            cout << "4. Save products to file\n";
            cout << "5. Display sorted data\n";
            cout << "6. Display hash table\n";
            cout << "0. Exit\n";
            cout << "\nEnter your Choice: ";
            cin >> choice;

            switch (choice) {
                case '1': system("cls"); lq.printQueue(); break;
                case '2': system("cls"); addProduct(); break;
                case '3': system("cls"); searchById(); break;
                case '4': lq.saveToFile(); break;
                case '5': system("cls"); displaySortedFile(); break;
                case '6': system("cls"); lq.displayHashTable(); break;
                case '0': return;
                default: cout << "Invalid choice! Please try again.\n";
            }
            system("pause");
        } while (true);
    }

    // Add a new product to the system
    void addProduct() {
    		
        int key =  getUniqueKey();
        
        string name, category, color, size;
        float price;
        cout<<"------------------------------------------------"<<endl;
        cout << "\t\tAdd new product "<<endl;
        cout<<"------------------------------------------------"<<endl;
        cout << "\nEnter the product name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter the product category: ";
        getline(cin, category);

        cout << "Enter the product size: ";
        getline(cin, size);
		        
        cout << "Enter the product color: ";
        getline(cin, color);
        

        
        cout << "Enter the product price: RM ";
        cin >> price;
        
        lq.append(key, name, category, size, color, price); // Add to queue and hash table
        cout << "Product added successfully with ID: " << key << endl;
    }

    // Search for a product by ID
    void searchById() {
        int k;  
        lq.classSeperateHashing().displayHashTable(); // Show existing products
        cout << "Enter the product ID to search: ";
        cin >> k;

        Product* result = lq.searchById(k); // Use hash table for search

        cout << "\nSearch Results for \"" << k << "\":\n";
    
        if (result == nullptr) {
            cout << "No product found with this ID!\n";
        } else {
            cout << "Product found:\n";
            cout << "ID: " << result->key << endl;              
            cout << "Product Name: " << result->name << endl;   
            cout << "Product Category: " << result->category << endl;
			cout << "Product Color: " << result->color << endl;    
            cout << "Product Size: " << result->size << endl;   
            cout << "Price: RM " << result->price << endl;
            delete result; // Free memory allocated for the result
        }
    }

    // Display products from "teamA.txt" in a formatted table
    void displaySortedFile() {
        ifstream file("TeamA_sorted_information.txt");
        if (!file) {
            cout << "No file found!" << endl;
            return;
        }
        cout << "Sorted Information \n";
        cout << "------------------------------------------------------------------------------------------------------------\n";
        cout << setw(9) << "ID" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
        cout << "------------------------------------------------------------------------------------------------------------\n";
        
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id;
            string name, category, size, color;
            double price;
                
            char comma;

            ss >> id >> comma;
            getline(ss >> ws, name, ',');
            getline(ss >> ws, category, ',');
            getline(ss >> ws, size, ',');
            getline(ss >> ws, color, ',');
            ss >> price;
            cout << setw(9) << id << setw(25) << name << setw(18) << category << setw(16) << size << setw(19) << color << setw(16) << "RM " << fixed << setprecision(2) << price << endl;
        }
        cout << "------------------------------------------------------------------------------------------------------------\n";
    }
};

// ==================== Team B: Placeholder for Another Team ====================

class TeamB {
public:
    void mainPage() {
        system("cls");
        cout << "Welcome to Team B" << endl;
        system("pause");
    }
};

// ==================== Main Program ====================

int main() {
    TeamA teamA; // Product Management System
    TeamB teamB; // Placeholder for another team
    char choice;

    do {
        system("cls");
        cout << "=== MAIN MENU ===\n";
        cout << "1. Team A\n";
        cout << "2. Team B\n";
        cout << "0. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1': teamA.mainMenu(); break;
            case '2': teamB.mainPage(); break;
            case '0': cout << "Exiting program...\n"; return 0;
            default: cout << "Invalid choice!\n";
        }
        system("pause");
    } while (true);

    return 0;
}
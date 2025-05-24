#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <string>

#include <cmath> // for log2 , Meta Binary Search
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
    cout << setw(9) << "Key" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
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
        cout << setw(9) << "Key" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
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
    	// call the dis
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
        
        cout << "Enter the product color: ";
        getline(cin, color);
        
        cout << "Enter the product size: ";
        getline(cin, size);
        
        cout << "Enter the product price: RM ";
        cin >> price;
        
        lq.append(key, name, category, color, size, price); // Add to queue and hash table
        cout << "Product added successfully with key: " << key << endl;
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
        cout << setw(9) << "Key" << setw(25) << "Product Name" << setw(18) << "Category" << setw(17) << "Size" << setw(18) << "Color" << setw(19) << "Price" << endl;
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
    
    long int generateProductKey() {
        return getUniqueKey();
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

const string HEADER_COLOR = "\033[38;2;85;170;255m"; // blue color for the header, decoration 
const string RESET   = "\033[0m"; // reset color

// decoration the header
void getPrintHeader() {
		cout << HEADER_COLOR << "+"
	         << setfill('-') << setw(12)  << "" << "+" // Fill the setw() space with "-"
	         << setw(22) << "" << "+"
	         << setw(17) << "" << "+"
	         << setw(8)  << "" << "+"
	         << setw(14) << "" << "+"
	         << setw(12) << "" << "+\n";
	    cout << setfill(' '); // Restore defaults, fill with spaces
	 
		cout << "| " << left
			     << setw(10)  << "ID"       << " | "
			     << setw(20) << "Name"     << " | "
			     << setw(15) << "Category" << " | "
			     << setw(6)  << "Size"     << " | "
			     << setw(12) << "Color"    << " | "
			     << setw(10) << "Price" << " |\n";
	
	    cout << "+"
	         << setfill('-') << setw(12)  << "" << "+"
	         << setw(22) << "" << "+"
	         << setw(17) << "" << "+"
	         << setw(8)  << "" << "+"
	         << setw(14) << "" << "+"
	         << setw(12) << "" << "+\n";
	    cout << setfill(' ') << RESET;
	
}


// Stack class with sorting/searching
class ADTstack {
private:
    Product stack[100]; //  fixed-capacity stack of size 100:
    int topstack;

public:
    ADTstack() {
        topstack = -1;
    }

    bool empty() {
        return topstack == -1;
    }

    bool full() {
        return topstack == 99;
    }

    void push(Product &p) {
        if (!full()) {
            topstack++;
            stack[topstack] = p;
        } else {
            cout << "Stack is Full" << endl;
        }
    }

    Product pop() {
        if (!empty()) {
            Product p = stack[topstack];
            topstack--;
            return p;
        } else {
            cout << "Stack is Empty" << endl;
            return Product{}; // return empty obeject avoid undefined
        }
    }
 
	// get product by using index
    Product getProductAt(int index) const {
        if (index >= 0 && index <= topstack) {
            return stack[index];
        } else {
            cout << "Invalid index!" << endl;
            return Product{}; // return empty obeject avoid undefined
        }
    }    

	// function to check if ID exist
    bool isIdExist(int id) const {
        for (int i = 0; i <= topstack; ++i) {
            if (stack[i].key == id) {
                return true;
            }
        }
        return false;
    }
	
	// function to diplay all products/items
    void display() {
        if (empty()) {
            cout << "Stack is Empty" << endl;
            return;
        }
        
        cout << "Products in Stack (Top to Bottom):\n";
		getPrintHeader();
			        
        for (int i = topstack; i >= 0; --i) {
            Product &p = stack[i];
            
            cout << "| " << left
              << setw(10) << p.key << " | "
              << setw(20) << p.name << " | "
              << setw(15) << p.category << " | "
              << setw(6) << p.size << " | "
              << setw(12) << p.color << " | "
              << setw(10) << p.price << " |\n";
        }
       
	    cout << "+"
	         << setfill('-') << setw(12)  << "" << "+"
	         << setw(22) << "" << "+"
	         << setw(17) << "" << "+"
	         << setw(8)  << "" << "+"
	         << setw(14) << "" << "+"
	         << setw(12) << "" << "+\n";
	    cout << setfill(' ');	    
    }
	
	// save file
    void saveToFile(const string &filename) {
        ofstream file(filename);
        
        for (int i = 0; i <= topstack; ++i) {
            Product &p = stack[i];
            file 
              << p.key       << ","
              << p.name     << ","
              << p.category << ","
              << p.size     << ","
              << p.color    << ","
              << p.price    << "\n";
        }
        
        cout << "Stack saved to file TeamB_sorted_information.\n";
    }
	
	// load 'products.txt' file 
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        
	    if (!file.is_open()) {
	        cout << "Error: cannot open file '" << filename << "' for reading.\n";
	        return;
	    }        
        
        string line;
        
        while (getline(file, line)) {
            if (full()) {
                cout << "Stack is full. Some records were not loaded.\n";
                break;
            }
            stringstream ss(line);
            Product p;
            string tmp;
			
			// convert to int
            getline(ss, tmp, ',');
            p.key = stoi(tmp);
            
            getline(ss, p.name, ',');
            getline(ss, p.category, ',');
            getline(ss, p.size, ',');
            getline(ss, p.color, ',');
			
            getline(ss, tmp, ',');  
			p.price = stod(tmp);

            push(p);
        }
        
    }
    
    // Gnome sort by chosen field
    void sortByField(int field) {
        int n = topstack + 1;
        int index = 0;
        while (index < n) {
            if (index == 0) 
				index++;
				
            bool inOrder;
            
            // Determine the comparison logic based on the user selected field
            switch (field) {
			    // id
			    case  1: 
					inOrder = stack[index].key >= stack[index-1].key;
					break;  // Ascending by id			
			    case  2: 
					inOrder = stack[index].key   <= stack[index-1].key;
					break;  // Descending by id
			
			    // name
			    case  3: 
					inOrder = stack[index].name >= stack[index-1].name; 
					break;  // Ascending by name
			    case  4: inOrder = stack[index].name <= stack[index-1].name;
					break;  // Descending by name
			
			    // category
			    case  5: 
					inOrder = stack[index].category >= stack[index-1].category; 
					break;  // Asc by category
			    case  6: 
					inOrder = stack[index].category <= stack[index-1].category; 
					break;  // Desc by category
			
			    // size
			    case  7: 
					inOrder = stack[index].size >= stack[index-1].size; 
					break;  // Asc by size
			    case  8: 
					inOrder = stack[index].size <= stack[index-1].size; 
					break;  // Desc by size
			
			    // color
			    case  9:  
					inOrder = stack[index].color >= stack[index-1].color; 
					break;  // Asc by color
			    case 10:  
					inOrder = stack[index].color <= stack[index-1].color; 
					break;  // Desc by color
			
			    // price
			    case 11: 
					inOrder = stack[index].price >= stack[index-1].price; 
					break;  // Asc by price
			    case 12: 
					inOrder = stack[index].price <= stack[index-1].price; 
					break;  // Desc by price
					
                default: 
					inOrder = true; 
					break;
            }
            
            if (inOrder) {
                index++;
            } else {
                swap(stack[index], stack[index-1]);
                index--;
            }
        }
    }
    
    
	// Function to show the working of Meta binary search
	int metaBinarySearch(int key_to_search) {
	    int n = topstack + 1;
	    
	    if (n == 0) 
			return -1; // represent with stack empty
	    
	    // Set number of bits to represent largest array index
	    int lg = log2(n-1)+1; 
	
	    //while ((1 << lg) < n - 1)
	        //lg += 1;
	
	    int pos = 0;
	    for (int i = lg ; i >= 0; i--) {
	        if (stack[pos].key == key_to_search)
	            return pos;
	
	        // Incrementally construct the
	        // index of the target value
	        int new_pos = pos | (1 << i);
	
	        // find the element in one
	        // direction and update position
	        if ((new_pos < n) && (stack[new_pos].key <= key_to_search))
	            pos = new_pos;
	    }
	
	    // if element found return pos otherwise -1
	    return ((stack[pos].key == key_to_search) ? pos : -1);
	}
    
};


void mainmenu() {
    ADTstack st;
    const string filename = "products.txt";
    st.loadFromFile(filename);

    int choice;
    do {
        cout << "Team B Menu:\n"
             << "1. Display Products (unsorted)\n"
             << "2. Add a Product\n"
             << "3. Remove a Product\n"
             << "4. Sorting Option\n"
             << "5. Search by Id \n"
             << "6. Save to File\n"
             << "0. Exit\n"
             << "\nEnter choice: ";
        cin >> choice;
		system("cls");
		
        switch (choice) {
        case 1:
            st.display();
            system("pause");
            system("cls");
            break;
        case 2: {
        	string again;
        	do {
        		
			
	            Product p;
	            
				TeamA team;
				long int key = team.generateProductKey();
				p.key = key;
				cout << "Product ID is : " << p.key;        
		
	            cout << "\nEnter name    : ";
	            cin.ignore();
				getline(cin, p.name);
	            
				cout << "Enter category: ";
				getline(cin, p.category);
	            
				cout << "Enter size    : ";
				getline(cin, p.size);
	            
				cout << "Enter color   : ";
				getline(cin, p.color);
	            
				cout << "Enter price   : ";
				cin >> p.price;
				
	            st.push(p);
	            
	            cout << "Product added successful!\n\n";
	            cout << "Do you want to add another product? (Y/N): ";
	            cin.ignore();
	            getline(cin, again);
	            
	            system("cls");
	    	} while (again == "Y" || again == "y");
	    	
	        break;
        }
        case 3: {
        	if (!st.empty()) {
	            Product p = st.pop();
				cout << "Removed Product\n"
				     << "Product ID  : " << p.key << "\n"
				     << "Name        : " << p.name << "\n"
				     << "Category    : " << p.category << "\n"
				     << "Size        : " << p.size << "\n"
				     << "Color       : " << p.color << "\n"
				     << "Price       : " << fixed << setprecision(2) << p.price << "\n\n";
	    	}
	    	
	    	system("pause");
	    	system("cls");
            break;
        }

        case 4: {
			cout << "Choose field to sort by:\n"
                      << "1. " << left << setw(25) << "Ascending by id"   
                      << "2. " << left << setw(25) << "Descending by id" << endl
                      << "3. " << left << setw(25) << "Ascending by name"
                      << "4. " << left << setw(25) << "Descending by name" << endl
                      << "5. " << left << setw(25) << "Ascending by category" 
                      << "6. " << left << setw(25) << "Descending by category" << endl
                      << "7. " << left << setw(25) << "Ascending by size"              
					  << "8. " << left << setw(25) << "Descending by size" << endl
                      << "9. " << left << setw(25) << "Ascending by color"
                      << "10." << left << setw(25) << "Descending by color" << endl
                      << "11." << left << setw(25) << "Ascending by price"
                      << "12." << left << setw(25) << "Descending by price" << endl				  
					  << "\nEnter field number: ";
            int field;
            cin >> field;
            //cin.ignore();
            
            st.sortByField(field);
            string fieldNames[13] = {
			    "", 
			    "ascending by id", "descending by id",
			    "ascending by name", "descending by name",
			    "ascending by category", "descending by category",
			    "ascending by size", "descending by size",
			    "ascending by color", "descending by color",
			    "ascending by price", "descending by price"
			};
            
            if (field >= 1 && field <= 12)
                cout << "Products sorted by " << fieldNames[field] << ". Displayed in Stack (Top = last added)\n";
            else
                cout << "Invalid field choice. No sorting applied.\n";
                
            st.display();
            
            break; 
		}
		
		case 5: {	
			st.sortByField(1);
		    int productId;
		    cout << "Enter product ID to search: ";
		    cin >> productId;
			
			int index = st.metaBinarySearch(productId);
			
		    if (index != -1) {
		        Product p = st.getProductAt(index); // Get product details by using index  
		        cout << "Product Found:\n";
		        
		        getPrintHeader();
		        
	            cout << "| " << left
	        	<< setw(10) << p.key << " | "
	            << setw(20) << p.name << " | "
	            << setw(15) << p.category << " | "
	            << setw(6) << p.size << " | "
	            << setw(12) << p.color << " | "
	            << setw(10) << p.price << " |\n";
	        
	       
			    cout << "+"
			         << setfill('-') << setw(12)  << "" << "+"
			         << setw(22) << "" << "+"
			         << setw(17) << "" << "+"
			         << setw(8)  << "" << "+"
			         << setw(14) << "" << "+"
			         << setw(12) << "" << "+\n";
			    cout << setfill(' ');	
		    } else {
		        cout << "Product not found.\n";
		    }
			
			break;
		}
       
        case 6:
            st.saveToFile("TeamB_sorted_information.txt");
            system("pause");
	    	system("cls");
            break;    
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);


} 

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
            case '2': mainmenu(); break;
            case '0': cout << "Exiting program...\n"; return 0;
            default: cout << "Invalid choice!\n";
        }
        system("pause");
    } while (true);

    return 0;
}
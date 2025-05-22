#include <iostream>
#include <string>
#include <fstream>
#include <sstream> // for stringstream

using namespace std;

// product
struct Product {
    string id;
    string name;
    string category;
    int quantity;
    double price;
};

class ADTstack {
private:
    Product stack[100];
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

    void push(const Product &p) {
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
            return Product{}; // avoid undefined
        }
    }

    void display() {
        if (empty()) {
            cout << "Stack is Empty" << endl;
            return;
        }
        
        cout << "Products in Stack (Top to Bottom):\n";
        for (int i = topstack; i >= 0; --i) {
            Product &p = stack[i];
            cout 
              << p.id       << ", "
              << p.name     << ", "
              << p.category << ", "
              << p.quantity << ", "
              << p.price    << "\n";
        }
    }

    void saveToFile(const string &filename) {
        ofstream file(filename);
        
        for (int i = 0; i <= topstack; ++i) {
            Product &p = stack[i];
            file 
              << p.id       << ","
              << p.name     << ","
              << p.category << ","
              << p.quantity << ","
              << p.price    << "\n";
        }
        
        cout << "Stack saved to file.\n";
    }

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

            getline(ss, p.id,       ',');
            getline(ss, p.name,     ',');
            getline(ss, p.category, ',');

            // convert to int
            getline(ss, tmp, ',');  
			p.quantity = stoi(tmp);
			
            getline(ss, tmp, ',');  
			p.price = stod(tmp);

            push(p);
        }
        
    }
    
    // Edit arbitrary product name by matching oldName -> newName
	bool editName(const string& oldName, const string& newName) {
	    if (empty()) 
			return false;
	
	    ADTstack tmp;
	    bool found = false;
	
	    while (!empty()) {
	        Product p = pop();
	        if (!found && p.name == oldName) {
	            p.name = newName;
	            found = true;
	        }
	        tmp.push(p);
	    }
	
	    while (!tmp.empty()) {
	        push(tmp.pop());
	    }
	
	    if (!found)
	        cout << "Name not found.\n";
	
	    return found;
	} 

    bool removeByName(const string& targetName) {
        if (empty()) 
			return false;
			
        ADTstack tmp;
        bool removed = false;
        
        while (!empty()) {
            Product p = pop();
            if (!removed && p.name == targetName) {
                removed = true;
                continue;
            }
            tmp.push(p);
        }
        
        while (!tmp.empty()) {
            push(tmp.pop());
        }
        
        if (!removed) 
			cout << "Product name \"" << targetName << "\" not found.\n";
			
        return removed;
    }    
    
};

// Menu-driven program
int main() {
    ADTstack st;
    const string filename = "products.txt";
    st.loadFromFile(filename);

    int choice;
    do {
        cout << "Menu:\n"
             << "1. Display Products\n"
             << "2. Add a Product\n"
             << "3. Remove a Product (Top)\n"
             << "4. Save to File\n"
             << "5. Edit a Product\n"
             << "6. Remove Product by Name\n"
             << "0. Exit\n"
             << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            st.display();
            break;
        case 2: {
            Product p;
            cout << "Enter id: ";        getline(cin, p.id);
            cout << "Enter name: ";      getline(cin, p.name);
            cout << "Enter category: ";  getline(cin, p.category);
            cout << "Enter quantity: ";  cin >> p.quantity; cin.ignore();
            cout << "Enter price: ";     cin >> p.price;    cin.ignore();
            st.push(p);
            break;
        }
        case 3: {
            Product p = st.pop();
            if (!p.id.empty())
                cout << "Removed: "
                     << p.id << ", "
                     << p.name << ", "
                     << p.category << ", "
                     << p.quantity << ", "
                     << p.price << "\n";
            break;
        }
        case 4:
            st.saveToFile(filename);
            break;

        case 5: {
            string oldN, newN;
            cout << "Enter existing product name to edit: ";
            getline(cin, oldN);
            cout << "Enter new product name: ";
            getline(cin, newN);
            st.editName(oldN, newN);
            break;
        }
       
	    case 6: {
	        string target;
	        cout << "Enter product name to remove: ";
	        getline(cin, target);
	        st.removeByName(target);
	        break;
	    }       
       
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
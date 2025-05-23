#include <iostream>
#include <string>
#include <fstream>
#include <sstream> // for stringstream
#include <iomanip>    // for setw()

using namespace std;

// product
struct Product {
    string id;
    string name;
    string category;
    string size;
    string color;
    string material;
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
              << p.size     << ", "
              << p.color    << ", "
              << p.material << ", "
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
              << p.size     << ","
              << p.color    << ","
              << p.material << ","
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
            getline(ss, p.size,     ',');
            getline(ss, p.color,    ',');
            getline(ss, p.material, ',');

            // convert to int
            getline(ss, tmp, ',');  
			p.quantity = stoi(tmp);
			
            getline(ss, tmp, ',');  
			p.price    = stod(tmp);

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
            
            switch (field) {
			    // id
			    case  1: inOrder = stack[index].id >= stack[index-1].id;
					break;  // Ascending by id
			    case  2: inOrder = stack[index].id   <= stack[index-1].id;
					break;  // Descending by id
			
			    // name
			    case  3: inOrder = stack[index].name >= stack[index-1].name; 
					break;  // Ascending by name
			    case  4: inOrder = stack[index].name <= stack[index-1].name;
					break;  // Descending by name
			
			    // category
			    case  5: inOrder = stack[index].category >= stack[index-1].category; 
					break;  // Asc by category
			    case  6: inOrder = stack[index].category <= stack[index-1].category; 
					break;  // Desc by category
			
			    // size
			    case  7: inOrder = stack[index].size >= stack[index-1].size; 
					break;  // Asc by size
			    case  8: inOrder = stack[index].size <= stack[index-1].size; 
					break;  // Desc by size
			
			    // color
			    case  9:  inOrder = stack[index].color >= stack[index-1].color; 
					break;  // Asc by color
			    case 10:  inOrder = stack[index].color <= stack[index-1].color; 
					break;  // Desc by color
			
			    // material
			    case 11: inOrder = stack[index].material >= stack[index-1].material; 
					break;  // Asc by material
			    case 12: inOrder = stack[index].material <= stack[index-1].material; 
					break;  // Desc by material
			
			    // quantity
			    case 13: inOrder = stack[index].quantity >= stack[index-1].quantity; 
					break;  // Asc by quantity
			    case 14: inOrder = stack[index].quantity <= stack[index-1].quantity; 
					break;  // Desc by quantity
			
			    // price
			    case 15: inOrder = stack[index].price >= stack[index-1].price; 
					break;  // Asc by price
			    case 16: inOrder = stack[index].price <= stack[index-1].price; 
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
             << "3. Remove a Product\n"
             << "4. Save to File\n"
             << "5. Sorting by categories \n"
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
            cout << "Enter size: ";      getline(cin, p.size);
            cout << "Enter color: ";     getline(cin, p.color);
            cout << "Enter material: ";  getline(cin, p.material);
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
                     << p.size << ", "
                     << p.color << ", "
                     << p.material << ", "
                     << p.quantity << ", "
                     << p.price << "\n";
            break;
        }
        case 4:
            st.saveToFile(filename);
            break;
        case 5: {
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
                      << "11." << left << setw(25) << "Ascending by material"
                      << "12." << left << setw(25) << "Descending by material" << endl
                      << "13." << left << setw(25) << "Ascending by quantity"
                      << "14." << left << setw(25) << "Descending by quantity" << endl
                      << "15." << left << setw(25) << "Ascending by price"
                      << "16." << left << setw(25) << "Descending by price" << endl				  
					  << "\nEnter field number: ";
            int field;
            cin >> field;
            //cin.ignore();
            
            st.sortByField(field);
            string fieldNames[17] = {
			    "", 
			    "ascending by id", "descending by id",
			    "ascending by name", "descending by name",
			    "ascending by category", "descending by category",
			    "ascending by size", "descending by size",
			    "ascending by color", "descending by color",
			    "ascending by material", "descending by material",
			    "ascending by quantity", "descending by quantity",
			    "ascending by price", "descending by price"
			};
            
            if (field >= 1 && field <= 16)
                cout << "Products sorted by " << fieldNames[field] << ".\n";
            else
                cout << "Invalid field choice. No sorting applied.\n";
                
            st.display();
            
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
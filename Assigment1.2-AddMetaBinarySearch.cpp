#include <iostream>
#include <string>
#include <fstream>
#include <sstream> // for stringstream
#include <iomanip> // for setw()

#include <cmath> // for log2 , Meta Binary Search

using namespace std;

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

// product structure
struct Product {
    int key;
    string name;
    string category;
    string size;
    string color;
    double price;
};

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

// Menu-driven program
int main() {
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
            Product p;
            
	            cout << "Enter id: ";        
				cin >> p.key;        
            /*
            bool idExist;
            do {
	            cout << "Enter id: ";        
				cin >> p.id;
				
				idExist = st.isIdExist(p.id);
				if (idExist) {
				    cout << "This ID already exists. Please enter a different ID!" << endl;
				}            	
			} while (idExist);
			*/
			
            cout << "Enter name: ";
            cin.ignore();
			getline(cin, p.name);
            
			cout << "Enter category: ";
			getline(cin, p.category);
            
			cout << "Enter size: ";
			getline(cin, p.size);
            
			cout << "Enter color: ";
			getline(cin, p.color);
            
			cout << "Enter price: ";
			cin >> p.price;
			
            st.push(p);
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

    return 0;
} // end of main

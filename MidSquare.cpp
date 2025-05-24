#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
using namespace std;

class TeamA {
private:
    struct Product {
        long int key;
        string name;
        string category;
        string size;
        string color;
        double price;
    };
    
    // Simple storage for products (you might want to replace this with your hash table later)
    Product products[100];
    int productCount = 0;
    
public:	
    void mainMenu() {
        char choice;
        do {
            system("cls");
            cout << "Swimming Equipment And Swimwear Management System\n";
            cout << "1. Add product\n";
            cout << "2. View all products\n";
            cout << "\nEnter your Choice: ";
            cin >> choice;

            switch (choice) {
                case '1': system("cls"); addProduct(); break;
                case '2': system("cls"); viewProducts(); break;
                case '0': return;
                default: cout << "Invalid choice! Please try again.\n";
            }
            system("pause");
        } while (true);
    }
    
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
    
    void addProduct() {
        cout << "=== ADD NEW PRODUCT ===\n\n";
        
        // Generate unique key
        long int productKey = getKey();
        cout << "Generated Product ID: " << productKey << "\n\n";
        
        // Get product details
        Product newProduct;
        newProduct.key = productKey;
        
        cout << "Enter Product Name: ";
        cin.ignore();
        getline(cin, newProduct.name);
        
        cout << "Enter Category: ";
        getline(cin, newProduct.category);
        
        cout << "Enter Size: ";
        getline(cin, newProduct.size);
        
        cout << "Enter Color: ";
        getline(cin, newProduct.color);
        
        cout << "Enter Price: RM ";
        cin >> newProduct.price;
        
        // Store the product
        products[productCount++] = newProduct;
        
        cout << "\nProduct added successfully!\n";
    }
    
    void viewProducts() {
        if (productCount == 0) {
            cout << "No products available.\n";
            return;
        }
        
        cout << "=== ALL PRODUCTS ===\n\n";
        cout << setw(10) << "ID" << setw(25) << "Product Name" << setw(15) << "Category" 
             << setw(10) << "Size" << setw(10) << "Color" << setw(10) << "Price" << endl;
        cout << string(80, '-') << endl;
        
        for (int i = 0; i < productCount; i++) {
            cout << setw(10) << products[i].key 
                 << setw(25) << products[i].name 
                 << setw(15) << products[i].category
                 << setw(10) << products[i].size 
                 << setw(10) << products[i].color 
                 << setw(9) << "RM " << fixed << setprecision(2) << products[i].price << endl;
        }
    }
};

class TeamB {
public:
    void mainPage() {
        system("cls");
        cout << "Welcome to Team B" << endl;
        system("pause");
    }
};

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
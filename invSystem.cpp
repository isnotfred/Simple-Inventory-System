#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <string>
using namespace std;

// Structure to represent an inventory item
struct Item {
    int id;
    string name;
    string brand;
    double price;
    int quantity;
    vector<double> priceHistory;
    vector<int> stockHistory;

    // Display item details, including price and stock history
    void displayDetails() {
        cout << fixed << setprecision(2);
        cout << "ID: " << setw(3) << setfill('0') << id << endl;
        cout << "Name: " << name << endl;
        cout << "Brand: " << brand << endl;
        cout << "Price: " << 'P' << price << endl;
        cout << "Quantity: " << quantity << " items" << endl;

        cout << "Price History: ";
        for (size_t i = 0; i < priceHistory.size(); ++i) {
            cout << "P" << priceHistory[i];
            if (i < priceHistory.size() - 1)
                cout << " | ";
            else
                cout << " (current)";
        }
        cout << endl;

        cout << "Stock History: ";
        for (size_t i = 0; i < stockHistory.size(); ++i) {
            cout << stockHistory[i] << " items";
            if (i < stockHistory.size() - 1)
                cout << " | ";
            else
                cout << " (current)";
        }
        cout << endl;
    }

    // Update the item price and store it in history
    void updatePrice(double newPrice) {
        price = newPrice;
        priceHistory.push_back(price);
        cout << "Price updated." << endl;
    }

    // Increase item stock and store new quantity in history
    void increaseStock(int stockIncrease) {
        quantity += stockIncrease;
        stockHistory.push_back(quantity);
        cout << "Stock increased." << endl;
    }

    // Decrease item stock and store new quantity in history
    void decreaseStock(int stockDecrease) {
        if (stockDecrease > quantity) {
            cout << "Input is higher than current stock! (" << quantity << " items)" << endl;
            return;
        }
        quantity -= stockDecrease;
        stockHistory.push_back(quantity);
        cout << "Stock decreased." << endl;
    }
};

// Validate if input string is a valid positive number
template <typename T>
bool isValidPositiveNum(string& str, T& result) {
    stringstream ss(str);
    ss >> result;
    return ss.eof() && !ss.fail() && result >= 0;
}

// Repeatedly prompt user until valid positive number is given
template <typename T>
T getValidPositiveNum(const string& prompt) {
    string input;
    T value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (isValidPositiveNum(input, value)) {
            return value;
        } else {
            cout << "Invalid input! Please enter a valid positive number." << endl;
        }
    }
}

// Get next available ID by finding the max and adding 1
int getNextID(const vector<Item>& inventory) {
    int maxID = 0;
    for (const auto& item : inventory) {
        if (item.id > maxID)
            maxID = item.id;
    }
    return maxID + 1;
}

// Validate that input is a numeric 3-digit string (e.g., 001 to 999)
string getValidID() {
    string validId;

    while (true) {
        cout << "Enter ID of Product: ";
        getline(cin, validId);
        bool valid = false;
        for (char c : validId) {
            if (!isdigit(c)) {
                cout << "Invalid Input! ID should only include digits.\n";
                valid = false;
                break;
            }
            valid = true;
        }

        if (!valid) continue;
        
        if (validId.length() != 3) {
            cout << "Invalid ID! Enter from 001-999.\n";
            continue;
        }
        break;
    } 

    return validId;
}

// Add a new item to the inventory
void addItem(vector<Item>& inventory) {
    string itemName, itemBrand;
    double initialPrice;
    int initialQuantity;

    cout << "========== New Item ==========" << endl;
    cout << "Name: ";
    getline(cin, itemName);
    cout << "Brand: ";
    getline(cin, itemBrand);

    initialPrice = getValidPositiveNum<double>("Initial Price: ");
    initialQuantity = getValidPositiveNum<int>("Initial Quantity: ");   
    
    Item newItem;
    newItem.id = getNextID(inventory);
    newItem.name = itemName;
    newItem.brand = itemBrand;
    newItem.price = initialPrice;
    newItem.quantity = initialQuantity;
    newItem.priceHistory.push_back(initialPrice);
    newItem.stockHistory.push_back(initialQuantity);

    inventory.push_back(newItem);

    cout << "Item added successfully! Assigned ID: " << setw(3) << setfill('0') << newItem.id << endl;
}

// Remove an item based on ID
void deleteItem(vector<Item>& inventory) {
    cout << "======== Remove Item =========" << endl;
    int idToRemove = stoi(getValidID());

    auto it = find_if(inventory.begin(), inventory.end(), [idToRemove](const Item& item) { return item.id == idToRemove; });

    if (it != inventory.end()) {
        inventory.erase(it);
        cout << "Item with ID " << setw(3) << setfill('0') << idToRemove << " removed!" << endl;
    } else {
        cout << "Item with ID " << setw(3) << setfill('0') << idToRemove << " not found!" << endl;
    }
}

// Display all items in the inventory
void viewItems(vector<Item>& inventory) {
    if (inventory.empty()) {
        cout << "No items in inventory." << endl;
        return;
    }

    cout << "=========== Items ============";
    for (auto item : inventory) {
        cout << endl;
        item.displayDetails();
    }
}

// Update the price or stock of an item
void updateItem(vector<Item>& inventory) {
    cout << "======== Update Item =========" << endl;
    int idToUpdate = stoi(getValidID());

    auto it = find_if(inventory.begin(), inventory.end(), [idToUpdate](const Item& item) { return item.id == idToUpdate; });

    if (it != inventory.end()) {
        cout << "ID: " << setw(3) << setfill('0') << it->id << ' ';
        cout << "Item name: " << it->name << endl;
    } else {
        cout << "Item with ID " << idToUpdate << " not found!" << endl;
        return;
    }

    int choice;
    bool running = true;

    while (running) {
        cout << "======== Update Item =========" << endl;
        cout << "1 - Update Price" << endl;
        cout << "2 - Increase Stock" << endl;
        cout << "3 - Decrease Stock" << endl;
        cout << "4 - Back" << endl;
        cout << "==============================" << endl;
        
        do {
            choice = getValidPositiveNum<int>("Enter choice: ");
            if (choice < 1 || choice > 4)
                cout << "Invalid choice! Enter from 1-4" << endl;
        } while (choice < 1 || choice > 4);

        switch (choice) {
            case 1:
                cout << "Current price: " << it->price << endl;
                it->updatePrice(getValidPositiveNum<double>("Enter new price: "));
                break;
            case 2:
                cout << "Current stock: " << it->quantity << endl;
                it->increaseStock(getValidPositiveNum<int>("Enter increase in stock: "));
                break;
            case 3:
                cout << "Current stock: " << it->quantity << endl;
                it->decreaseStock(getValidPositiveNum<int>("Enter decrease in stock: "));
                break;
            case 4:
                running = false;
                break;
        }        
    }
}

// Save inventory data to a file
void saveToFIle(vector<Item>& inventory, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "File is not open/found!";
        return;        
    }

    for (const auto& item : inventory) {
        outFile << fixed << setprecision(2);
        outFile << "ID: " << setw(3) << setfill('0') << item.id << endl;
        outFile << "Name: " << item.name << endl;
        outFile << "Brand: " << item.brand << endl;
        outFile << "Price: " << item.price << endl;
        outFile << "Quantity: " << item.quantity << endl;

        outFile << "Price History: ";
        for (size_t i = 0; i < item.priceHistory.size(); ++i) {
            outFile << "P" << item.priceHistory[i] << ' ';
        }
        outFile << endl;

        outFile << "Stock History: ";
        for (size_t i = 0; i < item.stockHistory.size(); ++i) {
            outFile << item.stockHistory[i] << ' ';
        }
        outFile << endl;
        outFile << "---" << endl;
    }

    outFile.close();
}

// Load inventory data from a file
void loadFromFIle(vector<Item>& inventory, const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "File is not open/found!";
        return;
    }

    inventory.clear();
    Item currentItem;
    string line;

    while (getline(inFile, line)) {
        if (line.rfind("ID:", 0) == 0) 
            currentItem.id = stoi(line.substr(4));
        else if (line.rfind("Name:", 0) == 0)
            currentItem.name = line.substr(6);
        else if (line.rfind("Brand:", 0) == 0)
            currentItem.brand = line.substr(7);
        else if (line.rfind("Price:", 0) == 0)
            currentItem.price = stod(line.substr(7));
        else if (line.rfind("Quantity:", 0) == 0)
            currentItem.quantity = stoi(line.substr(10 ));
        else if (line.rfind("Price History:", 0) == 0) {
            currentItem.priceHistory.clear();
            istringstream iss(line.substr(15));
            double p;
            while (iss >> p) {
                currentItem.priceHistory.push_back(p);
            }
        } else if (line.rfind("Stock History:", 0) == 0) {
            currentItem.stockHistory.clear();
            istringstream iss(line.substr(15));
            int q;
            while (iss >> q) {
                currentItem.stockHistory.push_back(q);
            }
        } else if (line == "---") {
            inventory.push_back(currentItem);
            currentItem = Item(); // reset for next item
        }
    }

    inFile.close();
}

// Main program loop
int main() {
    vector<Item> inventory;
    string filename = "inventory.txt";
    int choice;
    bool running = true;

    // Load existing inventory
    loadFromFIle(inventory, filename);

    while (running) {
        // Main menu
        cout << "========= INVENTORY ==========" << endl;
        cout << "1 - View Items" << endl;
        cout << "2 - Add Item" << endl;
        cout << "3 - Remove Item" << endl;
        cout << "4 - Update Item" << endl;
        cout << "5 - Exit" << endl;
        cout << "==============================" << endl;

        // Get valid menu option
        do {
            choice = getValidPositiveNum<int>("Enter choice: ");
            if (choice < 1 || choice > 5)
                cout << "Invalid choice! Enter from 1-5" << endl;
        } while (choice < 1 || choice > 5);

        // Handle menu choice
        switch (choice) {
            case 1:
                viewItems(inventory);
                break;
            case 2:
                addItem(inventory);
                break;
            case 3:
                deleteItem(inventory);
                break;
            case 4:
                updateItem(inventory);
                break;
            case 5:
                cout << "Changes saved! Thank you!" << endl;
                running = false;
                break;
        }
    }

    // Save changes before exiting
    saveToFIle(inventory, filename);
    return 0;
}
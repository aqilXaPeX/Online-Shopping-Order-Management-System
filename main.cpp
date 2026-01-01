#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;
using namespace chrono;

// Order structure with phone number
struct Order {
    int orderID;              // 1. Order ID
    string customerName;      // 2. Customer Name
    string phoneNumber;       // 3. Phone Number
    string productName;       // 4. Product Name
    string productCategory;   // 5. Product Category (for reports only)
    int quantity;             // 6. Quantity
    double unitPrice;         // 7. Unit Price
    double totalAmount;       // 8. Total Amount (from file)
    Order* next;              // For linked list
    
    Order() : next(nullptr) {}
};

// Global variables
Order* orderList = nullptr;
int orderCount = 0;
const int MAX_ORDERS = 10000;
const int DISPLAY_LIMIT = 100;

// Sorting statistics
struct SortStats {
    long long swapCount;
    double executionTime;
};

// Function prototypes
void mainMenu();
void displayOrdersMenu();
void sortingMenu();
void searchingMenu();
void reportsMenu();
void loadFromFile();
void saveToFile();
void displayFirstNOrders(int n, bool sorted = false);
void displayOrder(Order* order, int index);
SortStats insertionSort();
SortStats quickSort();
void quickSortRecursive(Order** arr, int low, int high, long long& swapCount);
int partition(Order** arr, int low, int high, long long& swapCount);
bool compareOrders(Order* a, Order* b);
void swapOrders(Order** a, Order** b);
Order** convertToArray();
void updateListFromArray(Order** arr);
void linearSearch();
void binarySearch();
void optimizedLinearSearch();
void performMultipleSearches();
void calculateTotalSales();
void totalQuantitySold();
void salesByCategory();
void clearScreen();
void pause();
void freeMemory();
void reloadData();
string formatNumber(double num);

int main() {
    
    // Auto-load data from file silently
    ifstream checkFile("orders_data.txt");
    if(checkFile.good()) {
        checkFile.close();
        
        string line;
        ifstream file("orders_data.txt");
        Order* tail = nullptr;
        while(getline(file, line)) {
            Order* newOrder = new Order();
            stringstream ss(line);
            string temp;
            
            getline(ss, temp, '|'); newOrder->orderID = stoi(temp);
            getline(ss, newOrder->customerName, '|');
            getline(ss, newOrder->phoneNumber, '|');
            getline(ss, newOrder->productName, '|');
            getline(ss, newOrder->productCategory, '|');
            getline(ss, temp, '|'); newOrder->quantity = stoi(temp);
            getline(ss, temp, '|'); newOrder->unitPrice = stod(temp);
            getline(ss, temp, '|'); newOrder->totalAmount = stod(temp);
            
            newOrder->next = nullptr;
            if (orderList == nullptr) {
                orderList = newOrder;
                tail = newOrder;
            } else {
                tail->next = newOrder;
                tail = newOrder;
            }
            orderCount++;
        }
        file.close();
    }
    
    mainMenu();
    
    freeMemory();
    return 0;
}

void mainMenu() {
    int choice;
    
    do {
        clearScreen();
        cout << "\n============================================================\n";
        cout << "                        MAIN MENUa                          \n";
        cout << "============================================================\n";
        cout << "  [1] Display Orders\n";
        cout << "  [2] Sorting\n";
        cout << "  [3] Searching\n";
        cout << "  [4] Reports\n";
        cout << "  [0] Exit\n";
        cout << "------------------------------------------------------------\n";
        cout << "  Total Orders in System: " << orderCount << "\n";
        cout << "============================================================\n";
        cout << "  Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: displayOrdersMenu(); break;
            case 2: sortingMenu(); break;
            case 3: searchingMenu(); break;
            case 4: reportsMenu(); break;
            case 0: cout << "\n  Thank you for using OSOMS!\n\n"; break;
            default: cout << "\n  Invalid choice!\n"; pause();
        }
    } while(choice != 0);
}



void displayOrdersMenu() {
    if(orderCount == 0) {
        clearScreen();
        cout << "\n  No data available! Please generate or load data first.\n";
        pause();
        return;
    }
    
    int choice;
    clearScreen();
    cout << "\n============================================================\n";
    cout << "                    DISPLAY ORDERS MENU                    \n";
    cout << "============================================================\n";
    cout << "  [1] Display First 100 Unsorted Orders\n";
    cout << "  [2] Display First 100 Sorted Orders\n";
    cout << "  [0] Back to Main Menu\n";
    cout << "============================================================\n";
    cout << "  Enter choice: ";
    cin >> choice;
    
    switch(choice) {
        case 1: displayFirstNOrders(DISPLAY_LIMIT, false); break;
        case 2: displayFirstNOrders(DISPLAY_LIMIT, true); break;
        case 0: break;
        default: cout << "\n  Invalid choice!\n"; pause();
    }
}

void sortingMenu() {
    if(orderCount == 0) {
        clearScreen();
        cout << "\n  No data available! Please generate or load data first.\n";
        pause();
        return;
    }
    
    int choice;
    do {
        clearScreen();
        cout << "\n============================================================\n";
        cout << "                      SORTING MENU                         \n";
        cout << "============================================================\n";
        cout << "  [1] Insertion Sort\n";
        cout << "  [2] Quick Sort\n";
        cout << "  [3] Compare Both Algorithms\n";
        cout << "  [0] Back to Main Menu\n";
        cout << "============================================================\n";
        cout << "  Enter choice: ";
        cin >> choice;
        
        SortStats stats;
        
        switch(choice) {
            case 1:
                clearScreen();
                cout << "\n  Performing Insertion Sort...\n";
                stats = insertionSort();
                cout << "\n  ========================================================\n";
                cout << "              INSERTION SORT RESULTS                     \n";
                cout << "  ========================================================\n";
                cout << "    Execution Time: " << fixed << setprecision(2) << stats.executionTime << " ms\n";
                cout << "    Number of Swaps: " << stats.swapCount << "\n";
                cout << "    Worst Case Complexity: O(n^2)\n";
                cout << "  ========================================================\n";
                displayFirstNOrders(DISPLAY_LIMIT, true);
                break;
                
            case 2:
                clearScreen();
                cout << "\n  Performing Quick Sort...\n";
                stats = quickSort();
                cout << "\n  ========================================================\n";
                cout << "                QUICK SORT RESULTS                       \n";
                cout << "  ========================================================\n";
                cout << "    Execution Time: " << fixed << setprecision(2) << stats.executionTime << " ms\n";
                cout << "    Number of Swaps: " << stats.swapCount << "\n";
                cout << "    Worst Case Complexity: O(n^2)\n";
                cout << "  ========================================================\n";
                displayFirstNOrders(DISPLAY_LIMIT, true);
                break;
                
            case 3: {
                clearScreen();
                cout << "\n  Comparing Sorting Algorithms...\n\n";
                cout << "  Running Insertion Sort on unsorted data...\n";
                SortStats insertionStats = insertionSort();
                
                cout << "  Reloading data for fair comparison...\n";
                reloadData();
                
                cout << "  Running Quick Sort on unsorted data...\n";
                SortStats quickStats = quickSort();
                
                cout << "\n  ==============================================================\n";
                cout << "                SORTING ALGORITHMS COMPARISON                   \n";
                cout << "  ==============================================================\n";
                cout << "  +------------------+-----------------+-------------------------+\n";
                cout << "  | Algorithm        | Time (ms)       | Swaps                   |\n";
                cout << "  +------------------+-----------------+-------------------------+\n";
                cout << "  | Insertion Sort   | " << setw(15) << fixed << setprecision(2) << insertionStats.executionTime 
                     << " | " << setw(23) << insertionStats.swapCount << " |\n";
                cout << "  | Quick Sort       | " << setw(15) << fixed << setprecision(2) << quickStats.executionTime 
                     << " | " << setw(23) << quickStats.swapCount << " |\n";
                cout << "  +------------------+-----------------+-------------------------+\n";
                
                if(quickStats.executionTime < insertionStats.executionTime) {
                    double improvement = ((insertionStats.executionTime - quickStats.executionTime) / insertionStats.executionTime) * 100;
                    cout << "\n  Quick Sort is faster by " << fixed << setprecision(2) << improvement << "%\n";
                } else {
                    double improvement = ((quickStats.executionTime - insertionStats.executionTime) / quickStats.executionTime) * 100;
                    cout << "\n  Insertion Sort is faster by " << fixed << setprecision(2) << improvement << "%\n";
                }
                cout << "  ==============================================================\n";
                pause();
                break;
            }
                
            case 0: break;
            default: cout << "\n  Invalid choice!\n"; pause();
        }
    } while(choice != 0);
}

void searchingMenu() {
    if(orderCount == 0) {
        clearScreen();
        cout << "\n  No data available! Please generate or load data first.\n";
        pause();
        return;
    }
    
    int choice;
    do {
        clearScreen();
        cout << "\n============================================================\n";
        cout << "                     SEARCHING MENU                        \n";
        cout << "============================================================\n";
        cout << "  [1] Linear Search\n";
        cout << "  [2] Binary Search\n";
        cout << "  [3] Optimized Linear Search (Sentinel)\n";
        cout << "  [0] Back to Main Menu\n";
        cout << "============================================================\n";
        cout << "  Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: linearSearch(); break;
            case 2: binarySearch(); break;
            case 3: optimizedLinearSearch(); break;
            case 0: break;
            default: cout << "\n  Invalid choice!\n"; pause();
        }
    } while(choice != 0);
}


void reportsMenu() {
    if(orderCount == 0) {
        clearScreen();
        cout << "\n  No data available! Please generate or load data first.\n";
        pause();
        return;
    }
    
    int choice;
    do {
        clearScreen();
        cout << "\n============================================================\n";
        cout << "                      REPORTS MENU                         \n";
        cout << "============================================================\n";
        cout << "  [1] Calculate Total Sales (Per Order)\n";
        cout << "  [2] Total Quantity Sold (By Product)\n";
        cout << "  [3] Sales Analysis (By Category)\n";
        cout << "  [0] Back to Main Menu\n";
        cout << "============================================================\n";
        cout << "  Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: calculateTotalSales(); break;
            case 2: totalQuantitySold(); break;
            case 3: salesByCategory(); break;
            case 0: break;
            default: cout << "\n  Invalid choice!\n"; pause();
        }
    } while(choice != 0);
}



void saveToFile() {
    if(orderCount == 0) {
        cout << "\n  No data to save!\n";
        pause();
        return;
    }
    
    ofstream file("orders_data.txt");
    if(!file) {
        cout << "\n  Error creating file!\n";
        pause();
        return;
    }
    
    Order* current = orderList;
    while(current != nullptr) {
        file << current->orderID << "|"
             << current->customerName << "|"
             << current->phoneNumber << "|"
             << current->productName << "|"
             << current->productCategory << "|"
             << current->quantity << "|"
             << current->unitPrice << "|"
             << current->totalAmount << "\n";
        current = current->next;
    }
    
    file.close();
    cout << "\n  Data saved successfully to orders_data.txt!\n";
    pause();
}

void loadFromFile() {
    ifstream file("orders_data.txt");
    if(!file) {
        cout << "\n  Error: File not found!\n";
        pause();
        return;
    }
    
    freeMemory();
    orderCount = 0;
    
    string line;
    Order* tail = nullptr;
    while(getline(file, line)) {
        Order* newOrder = new Order();
        stringstream ss(line);
        string temp;
        
        getline(ss, temp, '|'); newOrder->orderID = stoi(temp);
        getline(ss, newOrder->customerName, '|');
        getline(ss, newOrder->phoneNumber, '|');
        getline(ss, newOrder->productName, '|');
        getline(ss, newOrder->productCategory, '|');
        getline(ss, temp, '|'); newOrder->quantity = stoi(temp);
        getline(ss, temp, '|'); newOrder->unitPrice = stod(temp);
        getline(ss, temp, '|'); newOrder->totalAmount = stod(temp);
        
        newOrder->next = nullptr;
        if (orderList == nullptr) {
            orderList = newOrder;
            tail = newOrder;
        } else {
            tail->next = newOrder;
            tail = newOrder;
        }
        orderCount++;
    }
    
    file.close();
    cout << "\n  Successfully loaded " << orderCount << " orders from file!\n";
    pause();
}

void displayFirstNOrders(int n, bool sorted) {
    clearScreen();
    
    auto startTime = high_resolution_clock::now();
    
    if(sorted) {
        cout << "\n  Sorting data first...\n";
        quickSort();
    }
    
    cout << "\n" << (sorted ? "SORTED" : "UNSORTED") << " ORDERS (First " << min(n, orderCount) << ")\n";
    cout << "========================================================================================================================\n";
    
    cout << " No  OrderID  Customer Name        Phone Number   Product Name         Category          Qty    UnitPrice  TotalAmt\n";
    cout << "========================================================================================================================\n";
    
    Order* current = orderList;
    int count = 0;
    
    while(current != nullptr && count < n) {
        displayOrder(current, count + 1);
        current = current->next;
        count++;
    }
    
    auto endTime = high_resolution_clock::now();
    double executionTime = duration_cast<microseconds>(endTime - startTime).count() / 1000.0;
    
    cout << "========================================================================================================================\n";
    cout << "\n  Total orders displayed: " << count << " / " << orderCount << "\n";
    cout << "  Execution Time: " << fixed << setprecision(4) << executionTime << " ms\n";
    pause();
}

void displayOrder(Order* order, int index) {
    cout << setw(3) << right << index << "    "
         << setw(5) << right << order->orderID << "   "
         << setw(20) << left << order->customerName.substr(0, 20) << " "
         << setw(14) << left << order->phoneNumber.substr(0, 14) << " "
         << setw(20) << left << order->productName.substr(0, 20) << " "
         << setw(15) << left << order->productCategory.substr(0, 15) << " "
         << setw(4) << right << order->quantity << " "
         << setw(10) << right << fixed << setprecision(2) << order->unitPrice << " "
         << setw(11) << right << fixed << setprecision(2) << order->totalAmount
         << "\n";
}

SortStats insertionSort() {
    SortStats stats;
    stats.swapCount = 0;
    
    Order** arr = convertToArray();
    auto start = high_resolution_clock::now();
    
    for(int i = 1; i < orderCount; i++) {
        Order* key = arr[i];
        int j = i - 1;
        
        while(j >= 0 && compareOrders(key, arr[j])) {
            arr[j + 1] = arr[j];
            stats.swapCount++;
            j--;
        }
        arr[j + 1] = key;
    }
    
    auto end = high_resolution_clock::now();
    stats.executionTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    updateListFromArray(arr);
    delete[] arr;
    
    return stats;
}

SortStats quickSort() {
    SortStats stats;
    stats.swapCount = 0;
    
    Order** arr = convertToArray();
    
    auto start = high_resolution_clock::now();
    quickSortRecursive(arr, 0, orderCount - 1, stats.swapCount);
    auto end = high_resolution_clock::now();
    
    stats.executionTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    updateListFromArray(arr);
    delete[] arr;
    
    return stats;
}

void quickSortRecursive(Order** arr, int low, int high, long long& swapCount) {
    if(low < high) {
        int pi = partition(arr, low, high, swapCount);
        quickSortRecursive(arr, low, pi - 1, swapCount);
        quickSortRecursive(arr, pi + 1, high, swapCount);
    }
}

int partition(Order** arr, int low, int high, long long& swapCount) {
    Order* pivot = arr[high];
    int i = low - 1;
    
    for(int j = low; j < high; j++) {
        if(compareOrders(arr[j], pivot)) {
            i++;
            swapOrders(&arr[i], &arr[j]);
            swapCount++;
        }
    }
    swapOrders(&arr[i + 1], &arr[high]);
    swapCount++;
    return i + 1;
}

bool compareOrders(Order* a, Order* b) {
    if(a->totalAmount != b->totalAmount) {
        return a->totalAmount > b->totalAmount;
    }
    return a->orderID < b->orderID;
}

void swapOrders(Order** a, Order** b) {
    Order* temp = *a;
    *a = *b;
    *b = temp;
}

Order** convertToArray() {
    Order** arr = new Order*[orderCount];
    Order* current = orderList;
    int index = 0;
    
    while(current != nullptr) {
        arr[index++] = current;
        current = current->next;
    }
    
    return arr;
}

void updateListFromArray(Order** arr) {
    orderList = arr[0];
    for(int i = 0; i < orderCount - 1; i++) {
        arr[i]->next = arr[i + 1];
    }
    arr[orderCount - 1]->next = nullptr;
}

void linearSearch() {
    clearScreen();
    int searchOrderID;
    string searchName;
    
    cout << "\n============================================================\n";
    cout << "                     LINEAR SEARCH                         \n";
    cout << "============================================================\n";
    cout << "  Enter Order ID: ";
    cin >> searchOrderID;
    cin.ignore();
    cout << "  Enter Customer Name: ";
    getline(cin, searchName);
    
    auto start = high_resolution_clock::now();
    
    Order* current = orderList;
    bool found = false;
    int position = 0;
    
    while(current != nullptr) {
        position++;
        if(current->orderID == searchOrderID && current->customerName == searchName) {
            found = true;
            break;
        }
        current = current->next;
    }
    
    auto end = high_resolution_clock::now();
    double executionTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    cout << "\n  ============================================================\n";
    if(found) {
        cout << "  Status: FOUND\n";
        cout << "  Position: " << position << "\n";
        cout << "  ------------------------------------------------------------\n";
        cout << "  Order ID: " << current->orderID << "\n";
        cout << "  Customer: " << current->customerName << "\n";
        cout << "  Phone: " << current->phoneNumber << "\n";
        cout << "  Product: " << current->productName << "\n";
        cout << "  Category: " << current->productCategory << "\n";
        cout << "  Total Amount: RM " << fixed << setprecision(2) << current->totalAmount << "\n";
    } else {
        cout << "  Status: NOT FOUND\n";
    }
    cout << "  Execution Time: " << fixed << setprecision(4) << executionTime << " ms\n";
    cout << "  ============================================================\n";
    pause();
}

void binarySearch() {
    clearScreen();
    int searchOrderID;
    string searchName;
    
    cout << "\n============================================================\n";
    cout << "                     BINARY SEARCH                         \n";
    cout << "============================================================\n";
    cout << "  Note: Data will be sorted first for binary search.\n\n";
    cout << "  Enter Order ID: ";
    cin >> searchOrderID;
    cin.ignore();
    cout << "  Enter Customer Name: ";
    getline(cin, searchName);
    
    quickSort();
    Order** arr = convertToArray();
    
    auto start = high_resolution_clock::now();
    
    int left = 0, right = orderCount - 1;
    bool found = false;
    int position = -1;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;
        
        if(arr[mid]->orderID == searchOrderID && arr[mid]->customerName == searchName) {
            found = true;
            position = mid + 1;
            break;
        }
        
        if(arr[mid]->orderID < searchOrderID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    auto end = high_resolution_clock::now();
    double executionTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    cout << "\n  ============================================================\n";
    if(found) {
        cout << "  Status: FOUND\n";
        cout << "  Position: " << position << "\n";
        cout << "  ------------------------------------------------------------\n";
        cout << "  Order ID: " << arr[position-1]->orderID << "\n";
        cout << "  Customer: " << arr[position-1]->customerName << "\n";
        cout << "  Phone: " << arr[position-1]->phoneNumber << "\n";
        cout << "  Product: " << arr[position-1]->productName << "\n";
        cout << "  Category: " << arr[position-1]->productCategory << "\n";
        cout << "  Total Amount: RM " << fixed << setprecision(2) << arr[position-1]->totalAmount << "\n";
    } else {
        cout << "  Status: NOT FOUND\n";
    }
    cout << "  Execution Time: " << fixed << setprecision(4) << executionTime << " ms\n";
    cout << "  ============================================================\n";
    
    delete[] arr;
    pause();
}

void optimizedLinearSearch() {
    clearScreen();
    int searchOrderID;
    string searchName;
    
    cout << "\n============================================================\n";
    cout << "           OPTIMIZED LINEAR SEARCH (Sentinel)              \n";
    cout << "============================================================\n";
    cout << "  Enter Order ID: ";
    cin >> searchOrderID;
    cin.ignore();
    cout << "  Enter Customer Name: ";
    getline(cin, searchName);
    
    Order** arr = convertToArray();
    Order sentinel;
    sentinel.orderID = searchOrderID;
    sentinel.customerName = searchName;
    
    auto start = high_resolution_clock::now();
    
    Order** tempArr = new Order*[orderCount + 1];
    for(int i = 0; i < orderCount; i++) {
        tempArr[i] = arr[i];
    }
    tempArr[orderCount] = &sentinel;
    
    int i = 0;
    while(!(tempArr[i]->orderID == searchOrderID && tempArr[i]->customerName == searchName)) {
        i++;
    }
    
    bool found = (i < orderCount);
    
    auto end = high_resolution_clock::now();
    double executionTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    cout << "\n  ============================================================\n";
    if(found) {
        cout << "  Status: FOUND\n";
        cout << "  Position: " << (i + 1) << "\n";
        cout << "  ------------------------------------------------------------\n";
        cout << "  Order ID: " << tempArr[i]->orderID << "\n";
        cout << "  Customer: " << tempArr[i]->customerName << "\n";
        cout << "  Phone: " << tempArr[i]->phoneNumber << "\n";
        cout << "  Product: " << tempArr[i]->productName << "\n";
        cout << "  Category: " << tempArr[i]->productCategory << "\n";
        cout << "  Total Amount: RM " << fixed << setprecision(2) << tempArr[i]->totalAmount << "\n";
    } else {
        cout << "  Status: NOT FOUND\n";
    }
    cout << "  Execution Time: " << fixed << setprecision(4) << executionTime << " ms\n";
    cout << "  Improvement: Sentinel technique eliminates boundary checking\n";
    cout << "  ============================================================\n";
    
    delete[] arr;
    delete[] tempArr;
    pause();
}



void calculateTotalSales() {
    clearScreen();
    cout << "\n============================================================\n";
    cout << "                 TOTAL SALES REPORT                        \n";
    cout << "============================================================\n\n";
    
    double totalSales = 0;
    int totalOrders = 0;
    
    Order* current = orderList;
    while(current != nullptr) {
        totalSales += current->totalAmount;
        totalOrders++;
        current = current->next;
    }
    
    cout << "  +------------------------------------------------------------+\n";
    cout << "  | Sales Summary                                              |\n";
    cout << "  +------------------------------------------------------------+\n";
    cout << "  | Total Orders:   " << setw(6) << totalOrders << " orders                             |\n";
    cout << "  | Total Sales:    RM " << setw(18) << right << formatNumber(totalSales) << "                   |\n";
    cout << "  +------------------------------------------------------------+\n";
    
    pause();
}

void totalQuantitySold() {
    clearScreen();
    cout << "\n============================================================\n";
    cout << "          TOTAL QUANTITY SOLD (BY PRODUCT)                \n";
    cout << "============================================================\n\n";
    
    struct ProductData {
        string name;
        int totalQuantity;
    };
    
    ProductData products[50];
    int productCount = 0;
    
    // Aggregate quantities by product
    Order* current = orderList;
    while(current != nullptr) {
        bool found = false;
        for(int i = 0; i < productCount; i++) {
            if(products[i].name == current->productName) {
                products[i].totalQuantity += current->quantity;
                found = true;
                break;
            }
        }
        
        if(!found && productCount < 50) {
            products[productCount].name = current->productName;
            products[productCount].totalQuantity = current->quantity;
            productCount++;
        }
        current = current->next;
    }
    
    // Sort by quantity (descending)
    for(int i = 0; i < productCount - 1; i++) {
        for(int j = 0; j < productCount - i - 1; j++) {
            if(products[j].totalQuantity < products[j + 1].totalQuantity) {
                ProductData temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
    
    cout << "  +----+-------------------------+------------------+\n";
    cout << "  | No | Product Name            | Total Quantity   |\n";
    cout << "  +----+-------------------------+------------------+\n";
    
    // Show highest selling product
    if(productCount > 0) {
        cout << "  | " << setw(2) << 1
             << " | " << setw(23) << left << products[0].name.substr(0, 23)
             << " | " << setw(16) << right << products[0].totalQuantity << " |\n";
    }
    
    // Show lowest selling product
    if(productCount > 1) {
        cout << "  | " << setw(2) << productCount
             << " | " << setw(23) << left << products[productCount - 1].name.substr(0, 23)
             << " | " << setw(16) << right << products[productCount - 1].totalQuantity << " |\n";
    }
    
    cout << "  +----+-------------------------+------------------+\n";
    cout << "\n  Note: Showing highest and lowest selling products only.\n";
    cout << "        Total products tracked: " << productCount << "\n";
    
    pause();
}


void salesByCategory() {
    clearScreen();
    cout << "\n============================================================\n";
    cout << "            SALES ANALYSIS BY CATEGORY                     \n";
    cout << "============================================================\n\n";
    
    // Only include categories that are actually used in the data
    struct CategoryData {
        string name;
        int orderCount;
        double totalSales;
    };
    
    CategoryData categories[6] = {
        {"Food", 0, 0.0},
        {"Clothing", 0, 0.0},
        {"Books", 0, 0.0},
        {"Toys", 0, 0.0},
        {"Sports", 0, 0.0},
        {"Beauty", 0, 0.0}
    };
    
    int totalOrders = 0;
    Order* current = orderList;
    while(current != nullptr) {
        for(int i = 0; i < 6; i++) {
            if(current->productCategory == categories[i].name) {
                categories[i].totalSales += current->totalAmount;
                categories[i].orderCount++;
                totalOrders++;
                break;
            }
        }
        current = current->next;
    }
    
    // Sort by order count (descending - most orders first)
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5 - i; j++) {
            if(categories[j].orderCount < categories[j + 1].orderCount) {
                CategoryData temp = categories[j];
                categories[j] = categories[j + 1];
                categories[j + 1] = temp;
            }
        }
    }
    
    cout << "  +-----------------+------------+------------------+--------------+\n";
    cout << "  | Category        | Orders     | Total Sales (RM) | Percentage % |\n";
    cout << "  +-----------------+------------+------------------+--------------+\n";
    
    for(int i = 0; i < 6; i++) {
        double percentage = (totalOrders > 0) ? (categories[i].orderCount * 100.0 / totalOrders) : 0.0;
        cout << "  | " << setw(15) << left << categories[i].name 
             << " | " << setw(10) << right << categories[i].orderCount
             << " | " << setw(16) << right << formatNumber(categories[i].totalSales)
             << " | " << setw(12) << fixed << setprecision(2) << percentage << " |\n";
    }
    
    // Calculate grand total sales
    double grandTotal = 0.0;
    for(int i = 0; i < 6; i++) {
        grandTotal += categories[i].totalSales;
    }
    
    cout << "  +-----------------+------------+------------------+--------------+\n";
    cout << "  | TOTAL           | " << setw(10) << right << totalOrders 
         << " | " << setw(16) << right << formatNumber(grandTotal) << " | " << setw(12) << "100.00" << " |\n";
    cout << "  +-----------------+------------+------------------+--------------+\n";
    
    pause();
}

void topCustomers() {
    clearScreen();
    cout << "\n============================================================\n";
    cout << "              TOP 10 CUSTOMERS BY SPENDING                 \n";
    cout << "============================================================\n\n";
    
    struct CustomerSpending {
        string customerName;
        double totalSpending;
        int orderCount;
    };
    
    CustomerSpending customers[2000];
    int uniqueCustomers = 0;
    
    Order* current = orderList;
    while(current != nullptr) {
        bool found = false;
        for(int i = 0; i < uniqueCustomers; i++) {
            if(customers[i].customerName == current->customerName) {
                customers[i].totalSpending += current->totalAmount;
                customers[i].orderCount++;
                found = true;
                break;
            }
        }
        
        if(!found && uniqueCustomers < 2000) {
            customers[uniqueCustomers].customerName = current->customerName;
            customers[uniqueCustomers].totalSpending = current->totalAmount;
            customers[uniqueCustomers].orderCount = 1;
            uniqueCustomers++;
        }
        current = current->next;
    }
    
    for(int i = 0; i < uniqueCustomers - 1; i++) {
        for(int j = 0; j < uniqueCustomers - i - 1; j++) {
            if(customers[j].totalSpending < customers[j + 1].totalSpending) {
                CustomerSpending temp = customers[j];
                customers[j] = customers[j + 1];
                customers[j + 1] = temp;
            }
        }
    }
    
    cout << "  +----+--------------------+--------+------------------+\n";
    cout << "  | No | Customer Name      | Orders | Total Spent (RM) |\n";
    cout << "  +----+--------------------+--------+------------------+\n";
    
    int displayCount = (uniqueCustomers < 10) ? uniqueCustomers : 10;
    for(int i = 0; i < displayCount; i++) {
        cout << "  | " << setw(2) << (i + 1)
             << " | " << setw(18) << left << customers[i].customerName.substr(0, 18)
             << " | " << setw(6) << right << customers[i].orderCount
             << " | " << setw(16) << fixed << setprecision(2) << customers[i].totalSpending << " |\n";
    }
    
    cout << "  +----+--------------------+--------+------------------+\n";
    
    pause();
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore();
    cin.get();
}



void freeMemory() {
    Order* current = orderList;
    while(current != nullptr) {
        Order* temp = current;
        current = current->next;
        delete temp;
    }
    orderList = nullptr;
    orderCount = 0;
}

void reloadData() {
    // Free existing data
    freeMemory();
    
    // Reload from file
    ifstream file("orders_data.txt");
    if(file.is_open()) {
        string line;
        while(getline(file, line)) {
            Order* newOrder = new Order();
            stringstream ss(line);
            string temp;
            
            getline(ss, temp, '|'); newOrder->orderID = stoi(temp);
            getline(ss, newOrder->customerName, '|');
            getline(ss, newOrder->phoneNumber, '|');
            getline(ss, newOrder->productName, '|');
            getline(ss, newOrder->productCategory, '|');
            getline(ss, temp, '|'); newOrder->quantity = stoi(temp);
            getline(ss, temp, '|'); newOrder->unitPrice = stod(temp);
            getline(ss, temp, '|'); newOrder->totalAmount = stod(temp);
            
            newOrder->next = orderList;
            orderList = newOrder;
            orderCount++;
        }
        file.close();
    }
}

string formatNumber(double num) {
    stringstream ss;
    ss << fixed << setprecision(2) << num;
    string numStr = ss.str();
    
    // Find decimal point
    size_t decimalPos = numStr.find('.');
    string intPart = (decimalPos != string::npos) ? numStr.substr(0, decimalPos) : numStr;
    string decPart = (decimalPos != string::npos) ? numStr.substr(decimalPos) : "";
    
    // Add commas to integer part
    string result = "";
    int count = 0;
    for(int i = intPart.length() - 1; i >= 0; i--) {
        if(count == 3) {
            result = "," + result;
            count = 0;
        }
        result = intPart[i] + result;
        count++;
    }
    
    return result + decPart;
}

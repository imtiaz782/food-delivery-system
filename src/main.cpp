#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstdlib>

using namespace std;

// ======================= DATA STRUCTURES =======================

// Hash Table Implementation
template<typename T>
class HashTable {
private:
    struct HashNode {
        string key;
        T value;
        HashNode* next;
        HashNode(const string& k, const T& v) : key(k), value(v), next(nullptr) {}
    };
    
    vector<HashNode*> table;
    int capacity;
    
    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % capacity;
        }
        return hash;
    }
    
public:
    HashTable(int cap = 100) : capacity(cap) {
        table.resize(capacity, nullptr);
    }
    
    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode* current = table[i];
            while (current) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
    
    void insert(const string& key, const T& value) {
        int index = hashFunction(key);
        HashNode* newNode = new HashNode(key, value);
        
        if (!table[index]) {
            table[index] = newNode;
        } else {
            HashNode* current = table[index];
            while (current->next && current->key != key) {
                current = current->next;
            }
            if (current->key == key) {
                current->value = value;
                delete newNode;
            } else {
                current->next = newNode;
            }
        }
    }
    
    T* search(const string& key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        
        while (current) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }
    
    bool remove(const string& key) {
        int index = hashFunction(key);
        HashNode* current = table[index];
        HashNode* prev = nullptr;
        
        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
};

// Stack Implementation
template<typename T>
class Stack {
private:
    struct StackNode {
        T data;
        StackNode* next;
        StackNode(const T& item) : data(item), next(nullptr) {}
    };
    
    StackNode* top;
    
public:
    Stack() : top(nullptr) {}
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(const T& item) {
        StackNode* newNode = new StackNode(item);
        newNode->next = top;
        top = newNode;
    }
    
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        T data = top->data;
        StackNode* temp = top;
        top = top->next;
        delete temp;
        return data;
    }
    
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        return top->data;
    }
    
    bool isEmpty() const { return top == nullptr; }
};

// Linked List Implementation
template<typename T>
class LinkedList {
private:
    struct ListNode {
        T data;
        ListNode* next;
        ListNode(const T& item) : data(item), next(nullptr) {}
    };
    
    ListNode* head;
    
public:
    LinkedList() : head(nullptr) {}
    
    ~LinkedList() {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void insert(const T& item) {
        ListNode* newNode = new ListNode(item);
        if (!head) {
            head = newNode;
        } else {
            ListNode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    
    bool remove(const T& item) {
        if (!head) return false;
        
        if (head->data == item) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        
        ListNode* current = head;
        while (current->next && current->next->data != item) {
            current = current->next;
        }
        
        if (current->next) {
            ListNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return true;
        }
        return false;
    }
    
    vector<T> traverse() const {
        vector<T> result;
        ListNode* current = head;
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }
    
    bool isEmpty() const { return head == nullptr; }
};

// Binary Search Tree Implementation
template<typename T>
class BST {
private:
    struct TreeNode {
        T data;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const T& item) : data(item), left(nullptr), right(nullptr) {}
    };
    
    TreeNode* root;
    
    TreeNode* insertHelper(TreeNode* node, const T& item) {
        if (!node) {
            return new TreeNode(item);
        }
        if (item < node->data) {
            node->left = insertHelper(node->left, item);
        } else if (item > node->data) {
            node->right = insertHelper(node->right, item);
        }
        return node;
    }
    
    void inorderHelper(TreeNode* node, vector<T>& result) {
        if (node) {
            inorderHelper(node->left, result);
            result.push_back(node->data);
            inorderHelper(node->right, result);
        }
    }
    
    void deleteAll(TreeNode* node) {
        if (node) {
            deleteAll(node->left);
            deleteAll(node->right);
            delete node;
        }
    }
    
public:
    BST() : root(nullptr) {}
    
    ~BST() {
        deleteAll(root);
    }
    
    void insert(const T& item) {
        root = insertHelper(root, item);
    }
    
    vector<T> inorderTraversal() {
        vector<T> result;
        inorderHelper(root, result);
        return result;
    }
};

// ======================= DOMAIN CLASSES =======================

class MenuItem {
public:
    string name;
    double price;
    string category;
    
    MenuItem() : name(""), price(0.0), category("") {}
    MenuItem(const string& n, double p, const string& cat) : name(n), price(p), category(cat) {}
    
    bool operator<(const MenuItem& other) const { return price < other.price; }
    bool operator>(const MenuItem& other) const { return price > other.price; }
    bool operator==(const MenuItem& other) const { return name == other.name; }
    
    string toString() const {
        return name + "," + to_string(price) + "," + category;
    }
    
    static MenuItem fromString(const string& str) {
        stringstream ss(str);
        string name, category, priceStr;
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, category, ',');
        return MenuItem(name, stod(priceStr), category);
    }
};

class Customer {
public:
    string id;
    string name;
    string phone;
    string address;
    
    Customer() : id(""), name(""), phone(""), address("") {}
    Customer(const string& i, const string& n, const string& p, const string& addr)
        : id(i), name(n), phone(p), address(addr) {}
    
    void saveToFile() {
        ofstream file("customers.txt", ios::app);
        if (file.is_open()) {
            file << id << "," << name << "," << phone << "," << address << "\n";
            file.close();
        }
    }
    
    static vector<Customer> loadAllCustomers() {
        vector<Customer> customers;
        ifstream file("customers.txt");
        string line;
        
        while (getline(file, line) && !line.empty()) {
            stringstream ss(line);
            string id, name, phone, address;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, phone, ',');
            getline(ss, address, ',');
            customers.emplace_back(id, name, phone, address);
        }
        file.close();
        return customers;
    }
};

class Restaurant {
public:
    string id;
    string name;
    double rating;
    string address;
    vector<MenuItem> menu;
    
    Restaurant() : id(""), name(""), rating(0.0), address("") {}
    Restaurant(const string& i, const string& n, double r, const string& addr)
        : id(i), name(n), rating(r), address(addr) {}
    
    void addMenuItem(const MenuItem& item) {
        menu.push_back(item);
    }
    
    void saveToFile() {
        ofstream file("restaurants.dat", ios::app);
        if (file.is_open()) {
            file << id << "," << name << "," << rating << "," << address << "\n";
            for (const auto& item : menu) {
                file << "MENU," << id << "," << item.toString() << "\n";
            }
            file.close();
        }
    }
    
    static vector<Restaurant> loadAllRestaurants() {
        vector<Restaurant> restaurants;
        HashTable<Restaurant> restaurantMap;
        
        ifstream file("restaurants.dat");
        string line;
        
        while (getline(file, line) && !line.empty()) {
            if (line.substr(0, 4) == "MENU") {
                stringstream ss(line.substr(5));
                string restId, itemData;
                getline(ss, restId, ',');
                getline(ss, itemData);
                
                Restaurant* rest = restaurantMap.search(restId);
                if (rest) {
                    rest->addMenuItem(MenuItem::fromString(itemData));
                }
            } else {
                stringstream ss(line);
                string id, name, address, ratingStr;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, ratingStr, ',');
                getline(ss, address, ',');
                
                Restaurant rest(id, name, stod(ratingStr), address);
                restaurants.push_back(rest);
                restaurantMap.insert(id, rest);
            }
        }
        file.close();
        return restaurants;
    }
    
    bool operator<(const Restaurant& other) const {
        return rating > other.rating;
    }
};

class Order {
public:
    string orderId;
    string customerId;
    string restaurantId;
    vector<MenuItem> items;
    double totalAmount;
    string status;
    string timestamp;
    
    Order() : orderId(""), customerId(""), restaurantId(""), totalAmount(0.0), status("Pending") {
        updateTimestamp();
    }
    
    Order(const string& oid, const string& cid, const string& rid)
        : orderId(oid), customerId(cid), restaurantId(rid), totalAmount(0.0), status("Pending") {
        updateTimestamp();
    }
    
    void addItem(const MenuItem& item) {
        items.push_back(item);
        totalAmount += item.price;
    }
    
    void updateStatus(const string& newStatus) {
        status = newStatus;
        updateTimestamp();
        logStatusChange();
    }
    
    void saveToFile() {
        ofstream file("orders.log", ios::app);
        if (file.is_open()) {
            file << orderId << "," << customerId << "," << restaurantId << ","
                 << totalAmount << "," << status << "," << timestamp << "\n";
            for (const auto& item : items) {
                file << "ITEM," << orderId << "," << item.toString() << "\n";
            }
            file.close();
        }
    }
    
    bool operator==(const Order& other) const {
        return orderId == other.orderId;
    }
    
private:
    void updateTimestamp() {
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        timestamp = ctime(&time);
        timestamp.pop_back();
    }
    
    void logStatusChange() {
        ofstream file("order_tracking.log", ios::app);
        if (file.is_open()) {
            file << orderId << " status changed to " << status << " at " << timestamp << "\n";
            file.close();
        }
    }
};

// ======================= ALGORITHMS =======================

class SortingAlgorithms {
public:
    template<typename T>
    static void mergeSort(vector<T>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }
    
    template<typename T>
    static void merge(vector<T>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        vector<T> leftArr(n1), rightArr(n2);
        
        for (int i = 0; i < n1; i++)
            leftArr[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            rightArr[j] = arr[mid + 1 + j];
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if (leftArr[i] < rightArr[j]) {
                arr[k] = leftArr[i];
                i++;
            } else {
                arr[k] = rightArr[j];
                j++;
            }
            k++;
        }
        
        while (i < n1) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }
        
        while (j < n2) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }
    }
    
    template<typename T>
    static void bubbleSort(vector<T>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
    
    static void performanceAnalysis() {
        cout << "\n=== SORTING ALGORITHM PERFORMANCE ANALYSIS ===\n";
        
        vector<int> testData1(1000), testData2(1000);
        for (int i = 0; i < 1000; i++) {
            testData1[i] = testData2[i] = rand() % 1000;
        }
        
        auto start = chrono::high_resolution_clock::now();
        mergeSort(testData1, 0, testData1.size() - 1);
        auto end = chrono::high_resolution_clock::now();
        auto mergeDuration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        start = chrono::high_resolution_clock::now();
        bubbleSort(testData2);
        end = chrono::high_resolution_clock::now();
        auto bubbleDuration = chrono::duration_cast<chrono::microseconds>(end - start);
        
        cout << "Merge Sort (1000 elements): " << mergeDuration.count() << " microseconds\n";
        cout << "Bubble Sort (1000 elements): " << bubbleDuration.count() << " microseconds\n";
        cout << "Merge Sort is " << (double)bubbleDuration.count() / mergeDuration.count() 
             << "x faster than Bubble Sort\n";
    }
};

class SearchAlgorithms {
public:
    template<typename T>
    static int linearSearch(const vector<T>& arr, const T& target) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] == target) {
                return i;
            }
        }
        return -1;
    }
    
    static void dfsRoute(vector<vector<int>>& graph, int start, vector<bool>& visited, vector<int>& path) {
        visited[start] = true;
        path.push_back(start);
        cout << "Location " << start << " -> ";
        
        for (int i = 0; i < graph[start].size(); i++) {
            if (graph[start][i] == 1 && !visited[i]) {
                dfsRoute(graph, i, visited, path);
            }
        }
    }
    
    static void demonstrateDFS() {
        cout << "\n=== DELIVERY ROUTE OPTIMIZATION (DFS) ===\n";
        cout << "Locations: 0=Restaurant, 1=Hub, 2=Customer1, 3=Customer2\n";
        
        vector<vector<int>> deliveryMap = {
            {0, 1, 0, 1},
            {1, 0, 1, 1},
            {0, 1, 0, 0},
            {1, 1, 0, 0}
        };
        
        vector<bool> visited(4, false);
        vector<int> path;
        
        cout << "DFS Route: ";
        dfsRoute(deliveryMap, 0, visited, path);
        cout << "END\n";
    }
};

// ======================= MAIN SYSTEM =======================

class FoodDeliverySystem {
private:
    HashTable<Restaurant> restaurants;
    HashTable<Customer> customers;
    LinkedList<Order> pendingOrders;
    LinkedList<Order> completedOrders;
    Stack<string> orderHistory;
    BST<MenuItem> menuSearchTree;
    
    static string generateId() {
        return to_string(chrono::duration_cast<chrono::milliseconds>(
            chrono::system_clock::now().time_since_epoch()).count());
    }
    
public:
    FoodDeliverySystem() {
        loadSystemData();
    }
    
    void loadSystemData() {
        cout << "Loading system data from files...\n";
        
        vector<Restaurant> restaurantList = Restaurant::loadAllRestaurants();
        for (const auto& restaurant : restaurantList) {
            restaurants.insert(restaurant.id, restaurant);
            for (const auto& item : restaurant.menu) {
                menuSearchTree.insert(item);
            }
        }
        
        vector<Customer> customerList = Customer::loadAllCustomers();
        for (const auto& customer : customerList) {
            customers.insert(customer.id, customer);
        }
        
        cout << "System loaded: " << restaurantList.size() << " restaurants, " 
             << customerList.size() << " customers\n\n";
    }
    
    void addRestaurant() {
        string name, address;
        double rating;
        
        cout << "\n=== ADD NEW RESTAURANT ===\n";
        cout << "Enter restaurant name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter rating (0.0-5.0): ";
        cin >> rating;
        
        string id = generateId();
        Restaurant restaurant(id, name, rating, address);
        
        restaurant.addMenuItem(MenuItem("Burger", 12.99, "Fast Food"));
        restaurant.addMenuItem(MenuItem("Pizza", 18.50, "Italian"));
        restaurant.addMenuItem(MenuItem("Salad", 8.75, "Healthy"));
        
        restaurants.insert(id, restaurant);
        restaurant.saveToFile();
        
        for (const auto& item : restaurant.menu) {
            menuSearchTree.insert(item);
        }
        
        cout << "Restaurant added successfully with ID: " << id << "\n";
    }
    
    void addCustomer() {
        string name, phone, address;
        
        cout << "\n=== ADD NEW CUSTOMER ===\n";
        cout << "Enter customer name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter phone number: ";
        getline(cin, phone);
        cout << "Enter address: ";
        getline(cin, address);
        
        string id = generateId();
        Customer customer(id, name, phone, address);
        
        customers.insert(id, customer);
        customer.saveToFile();
        
        cout << "Customer added successfully with ID: " << id << "\n";
    }
    
    void placeOrder() {
        string customerId, restaurantId;
        
        cout << "\n=== PLACE NEW ORDER ===\n";
        cout << "Enter customer ID: ";
        cin >> customerId;
        
        Customer* customer = customers.search(customerId);
        if (!customer) {
            cout << "Customer not found!\n";
            return;
        }
        
        displayRestaurants();
        cout << "Enter restaurant ID: ";
        cin >> restaurantId;
        
        Restaurant* restaurant = restaurants.search(restaurantId);
        if (!restaurant) {
            cout << "Restaurant not found!\n";
            return;
        }
        
        string orderId = generateId();
        Order order(orderId, customerId, restaurantId);
        
        cout << "\n=== MENU ===\n";
        for (int i = 0; i < restaurant->menu.size(); i++) {
            cout << i + 1 << ". " << restaurant->menu[i].name 
                 << " - $" << restaurant->menu[i].price << "\n";
        }
        
        int choice;
        do {
            cout << "Select item (1-" << restaurant->menu.size() << ", 0 to finish): ";
            cin >> choice;
            
            if (choice > 0 && choice <= restaurant->menu.size()) {
                order.addItem(restaurant->menu[choice - 1]);
                cout << "Added " << restaurant->menu[choice - 1].name << " to order\n";
            }
        } while (choice != 0);
        
        if (!order.items.empty()) {
            pendingOrders.insert(order);
            orderHistory.push(orderId);
            order.saveToFile();
            
            cout << "Order placed successfully! Order ID: " << orderId << "\n";
            cout << "Total amount: $" << order.totalAmount << "\n";
        } else {
            cout << "No items added to order.\n";
        }
    }
    
    void displayRestaurants() {
        cout << "\n=== AVAILABLE RESTAURANTS ===\n";
        
        vector<Restaurant> restaurantList = Restaurant::loadAllRestaurants();
        SortingAlgorithms::mergeSort(restaurantList, 0, restaurantList.size() - 1);
        
        for (const auto& restaurant : restaurantList) {
            cout << "ID: " << restaurant.id << " | " << restaurant.name 
                 << " | Rating: " << restaurant.rating << "\n";
        }
        cout << "\n";
    }
    
    void displayOrders() {
        cout << "\n=== ORDER STATUS ===\n";
        
        cout << "PENDING ORDERS:\n";
        vector<Order> pending = pendingOrders.traverse();
        for (const auto& order : pending) {
            cout << "Order ID: " << order.orderId << " | Status: " << order.status 
                 << " | Amount: $" << order.totalAmount << "\n";
        }
        
        cout << "\nCOMPLETED ORDERS:\n";
        vector<Order> completed = completedOrders.traverse();
        for (const auto& order : completed) {
            cout << "Order ID: " << order.orderId << " | Status: " << order.status 
                 << " | Amount: $" << order.totalAmount << "\n";
        }
        cout << "\n";
    }
    
    void searchMenuItems() {
        cout << "\n=== MENU SEARCH (BST) ===\n";
        
        vector<MenuItem> sortedMenu = menuSearchTree.inorderTraversal();
        
        cout << "Menu items sorted by price:\n";
        for (const auto& item : sortedMenu) {
            cout << item.name << " - $" << item.price << " (" << item.category << ")\n";
        }
    }
    
    void showOrderHistory() {
        cout << "\n=== ORDER HISTORY (Stack) ===\n";
        
        Stack<string> tempStack;
        cout << "Recent orders (most recent first):\n";
        
        int count = 0;
        while (!orderHistory.isEmpty() && count < 5) {
            string orderId = orderHistory.pop();
            cout << (count + 1) << ". Order ID: " << orderId << "\n";
            tempStack.push(orderId);
            count++;
        }
        
        while (!tempStack.isEmpty()) {
            orderHistory.push(tempStack.pop());
        }
    }
    
    void performanceAnalysis() {
        cout << "\n=== SYSTEM PERFORMANCE ANALYSIS ===\n";
        SortingAlgorithms::performanceAnalysis();
        SearchAlgorithms::demonstrateDFS();
    }
    
    void exportData() {
        cout << "\n=== DATA EXPORT ===\n";
        
        ofstream csvFile("restaurants_export.csv");
        csvFile << "ID,Name,Rating,Address\n";
        
        vector<Restaurant> restaurantList = Restaurant::loadAllRestaurants();
        for (const auto& restaurant : restaurantList) {
            csvFile << restaurant.id << "," << restaurant.name << "," 
                   << restaurant.rating << "," << restaurant.address << "\n";
        }
        csvFile.close();
        
        cout << "Data exported to restaurants_export.csv\n";
    }
    
    void displayMenu() {
        cout << "\n=== FOOD DELIVERY SYSTEM ===\n";
        cout << "1. Add Restaurant\n";
        cout << "2. Add Customer\n";
        cout << "3. Place Order\n";
        cout << "4. Display Restaurants\n";
        cout << "5. Display Orders\n";
        cout << "6. Search Menu Items\n";
        cout << "7. Show Order History\n";
        cout << "8. Performance Analysis\n";
        cout << "9. Export Data\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
    }
    
    void run() {
        int choice;
        
        cout << "=== FOOD DELIVERY SYSTEM ===\n";
        cout << "Air University - Data Structures & Algorithms Project\n\n";
        
        do {
            displayMenu();
            cin >> choice;
            
            switch (choice) {
                case 1: addRestaurant(); break;
                case 2: addCustomer(); break;
                case 3: placeOrder(); break;
                case 4: displayRestaurants(); break;
                case 5: displayOrders(); break;
                case 6: searchMenuItems(); break;
                case 7: showOrderHistory(); break;
                case 8: performanceAnalysis(); break;
                case 9: exportData(); break;
                case 0: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice!\n";
            }
            
            if (choice != 0) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
            }
            
        } while (choice != 0);
        
        cout << "Thank you for using Food Delivery System!\n";
    }
};

// ======================= MAIN FUNCTION =======================

int main() {
    try {
        srand(time(nullptr));
        
        FoodDeliverySystem system;
        system.run();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
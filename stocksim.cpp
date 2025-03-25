#include <iostream>
#include <stack>
#include <cstdlib> 
#include <ctime>   
using namespace std;

class Stock {
public:
    string name;
    double price;
    int quantity;
    stack<double> priceHistory;  

    Stock(string stockName, double stockPrice, int stockQuantity) {
        name = stockName;
        price = stockPrice;
        quantity = stockQuantity;
        priceHistory.push(stockPrice);
    }

    void updatePrice(double newPrice) {
        priceHistory.push(newPrice);
        price = newPrice;
    }
};

class StockMarket {
private:
    struct Node {
        Stock* stock;
        Node* left;
        Node* right;
        Node(Stock* s) : stock(s), left(NULL), right(NULL) {}
    };

    Node* root;

    Node* insert(Node* root, Stock* stock) {
        if (!root) return new Node(stock);
        if (stock->name < root->stock->name) root->left = insert(root->left, stock);
        else root->right = insert(root->right, stock);
        return root;
    }

    Stock* search(Node* root, string name) {
        if (!root) return NULL;
        if (root->stock->name == name) return root->stock;
        if (name < root->stock->name) return search(root->left, name);
        return search(root->right, name);
    }

public:
    StockMarket() : root(NULL) {
        srand(time(0)); 
    }

    void addStock(string name, double price, int quantity) {
        root = insert(root, new Stock(name, price, quantity));
    }

    Stock* findStock(string name) {
        return search(root, name);
    }

    void fluctuate(Stock* stock) {
        if (!stock) return;
        double newPrice = stock->price * (0.9 + (rand() % 20) / 100.0); 
        stock->updatePrice(newPrice);
    }
};

class Portfolio {
private:
    struct Node {
        Stock* stock;
        int ownedQuantity;
        Node* next;
        Node(Stock* s, int qty) : stock(s), ownedQuantity(qty), next(NULL) {}
    };

    Node* head;

public:
    Portfolio() : head(NULL) {}

    void buyStock(Stock* stock, int quantity) {
        Node* newNode = new Node(stock, quantity);
        newNode->next = head;
        head = newNode;
    }

    void viewPortfolio() {
        if (!head) {
            cout << "Portfolio is empty!\n";
            return;
        }

        Node* temp = head;
        while (temp) {
            cout << temp->stock->name << " - " << temp->ownedQuantity 
                 << " shares @ $" << temp->stock->price << endl;
            temp = temp->next;
        }
    }
};

class TransactionQueue {
private:
    struct Transaction {
        string type;  
        Stock* stock;
        int quantity;
        Transaction* next;
        Transaction(string t, Stock* s, int q) : type(t), stock(s), quantity(q), next(NULL) {}
    };

    Transaction* front;
    Transaction* rear;

public:
    TransactionQueue() : front(NULL), rear(NULL) {}

    void enqueue(string type, Stock* stock, int quantity) {
        Transaction* newTrans = new Transaction(type, stock, quantity);
        if (!rear) front = rear = newTrans;
        else {
            rear->next = newTrans;
            rear = newTrans;
        }
    }

    void processTransactions(Portfolio& portfolio) {
        while (front) {
            Transaction* temp = front;
            if (temp->type == "BUY") {
                portfolio.buyStock(temp->stock, temp->quantity);
                cout << "Bought " << temp->quantity << " of " << temp->stock->name << endl;
            }
            front = front->next;
            delete temp;
        }
        rear = NULL;
    }
};

void mainMenu(StockMarket& market, Portfolio& portfolio, TransactionQueue& queue) {
    int choice;
    do {
        cout << "\n--- STOCK MARKET SIMULATOR ---\n";
        cout << "1. Search Stock\n2. Buy Stock\n3. Sell Stock\n4. View Portfolio\n5. Fluctuate Prices\n6. Process Transactions\n7. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {  
            string name;
            cout << "Enter stock name: ";
            cin >> name;
            Stock* stock = market.findStock(name);
            if (stock) cout << name << " is trading at $" << stock->price << endl;
            else cout << "Stock not found!\n";
        }
        else if (choice == 2) {  
            string name;
            int quantity;
            cout << "Enter stock name: ";
            cin >> name;
            cout << "Enter quantity: ";
            cin >> quantity;
            Stock* stock = market.findStock(name);
            if (stock) {
                queue.enqueue("BUY", stock, quantity);
                cout << "Buy order placed for " << quantity << " shares of " << stock->name << endl;
            }
            else cout << "Stock not found!\n";
        }
        else if (choice == 4) {  
            portfolio.viewPortfolio();
        }
        else if (choice == 5) {  
            string name;
            cout << "Enter stock name: ";
            cin >> name;
            Stock* stock = market.findStock(name);
            if (stock) {
                market.fluctuate(stock);
                cout << "New price of " << stock->name << ": $" << stock->price << endl;
            }
            else cout << "Stock not found!\n";
        }
        else if (choice == 6) {  
            queue.processTransactions(portfolio);
        }
    } while (choice != 7);
}

int main() {
    StockMarket market;
    Portfolio portfolio;
    TransactionQueue queue;

    market.addStock("AAPL", 150, 100);
    market.addStock("GOOG", 2800, 50);
    market.addStock("TSLA", 700, 80);

    mainMenu(market, portfolio, queue);

    return 0;
}


#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
using namespace std;

struct Item
{
    string name;
    string barcode;
    double price;
    int quantity;
    Item(string name = "", string barcode = "", double price = 0, int quantity = 0)
    {
        this->name = name;
        this->barcode = barcode;
        this->price = price;
        this->quantity = quantity;
    }

    void changeItemName(string name)
    {
        this->name = name;
    }
    void changeItemPrice(double price)
    {
        if (price <= 0)
        {
            cout << "Invalid price\n";
            return;
        }
        this->price = price;
    }
    void changeQuantity(int quantity)
    {
        if (quantity < 0)
        {
            cout << "Invalid quantity\n";
            return;
        }
        this->quantity = quantity;
    }
    void printItemDetails()
    {
        cout << endl
             << endl;
        cout << "Name: " << name << endl;
        cout << "Barcode: " << barcode << endl;
        cout << "Price: " << price << endl;
        cout << "Quantity: " << quantity << endl;
    }
};
struct Inventory
{
    Item *items = nullptr;
    int capacity;
    int size;

    void loadItems()
    {
        string temp;
        ifstream File("items.txt");
        if (!File)
        {
            cout << "File not found\n";
            exit(0);
        }
        if (File.peek() == EOF)
        {
            cout << "No items available";
            exit(0);
        }
        getline(File, temp, ',');
        try
        {
            size = stoi(temp);
            capacity = size;
        }
        catch (...)
        {
            cout << "Error getting number of items in inventory\n";
            exit(0);
        }

        items = new Item[capacity];
        for (int i = 0; i < size; i++)
        {

            getline(File, temp, ',');
            items[i].name = temp;
            getline(File, temp, ',');
            items[i].barcode = temp;
            getline(File, temp, ',');
            try
            {
                items[i].price = stod(temp);
            }
            catch (...)
            {
                cout << "Error using func stod()";
                exit(0);
            }
            getline(File, temp, ',');
            try
            {
                items[i].quantity = stoi(temp);
            }
            catch (...)
            {
                cout << "Error using func stoi()";
                exit(0);
            }
        }
        File.close();
    }
    Inventory()
    {
        loadItems();
    }
    void writeItems()
    {
        ofstream File("items.txt");
        File << fixed << setprecision(2);
        File << size;
        for (int i = 0; i < size; i++)
        {
            File << ',';
            File << items[i].name << ',';
            File << items[i].barcode << ',';
            File << items[i].price << ',';
            File << items[i].quantity;
        }
        File.close();
    }
    void updateItem(string barcode, string name)
    {
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                items[i].changeItemName(name);
                writeItems();
                return;
            }
        }
        cout << "Item not found\n";
    }
    void updateItem(string barcode, double price)
    {
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                items[i].changeItemPrice(price);
                writeItems();
                return;
            }
        }
        cout << "Item not found\n";
    }
    void updateItem(string barcode, int quantity)
    {
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                items[i].changeQuantity(quantity);
                writeItems();
                return;
            }
        }
        cout << "Item not found\n";
    }
    void printItemDetails(string barcode)
    {
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                items[i].printItemDetails();
                return;
            }
        }
        cout << "Item not found\n";
    }
    void addItem(string name, string barcode, double price, int quantity)
    {
        if (quantity < 0)
        {
            cout << "Invalid quantity\n";
            return;
        }
        if (price <= 0)
        {
            cout << "Invalid price\n";
            return;
        }
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                items[i].quantity += quantity;
                writeItems();
                return;
            }
        }
        if (size == capacity)
        {
            capacity *= 2;
            Item *newItems = new Item[capacity];
            for (int i = 0; i < size; i++)
            {
                newItems[i] = items[i];
            }
            delete[] items;
            items = newItems;
        }
        items[size] = Item(name, barcode, price, quantity);
        size++;
        writeItems();
    }
    void removeItem(string barcode)
    {
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                for (int j = i; j < size - 1; j++)
                {
                    items[j] = items[j + 1];
                }
                size--;
                writeItems();
                return;
            }
        }
        cout << "Item not found\n";
    }
    void printInventory()
    {
        cout << "Inventory Items:\n";
        for (int i = 0; i < size; i++)
        {
            items[i].printItemDetails();
            cout << "\n";
        }
    }
    ~Inventory()
    {
        delete[] items;
    }
};
struct Order
{
    Item *items = nullptr;
    int size = 0;
    int capacity = 1;
    int itemQuantities = 0;
    double basePrice = 0;
    int tax = 0;
    double totalPrice = 0;
    string paymentMethod;
    bool status = false; // false for pending and true for completed

    Order()
    {
        items = new Item[capacity];
    }
    void addItemInOrder(Item item, Inventory &inventory)
    {
        if (status)
        {
            cout << "Order is completed";
            return;
        }
        if (size == capacity)
        {
            capacity = capacity * 2;
            Item *newItems = new Item[capacity];
            for (int i = 0; i < size; i++)
            {
                newItems[i] = items[i];
            }
            delete[] items;
            items = newItems;
        }
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == item.barcode)
            {
                items[i].quantity += item.quantity;
                for (int i = 0; i < inventory.size; i++)
                {
                    if (inventory.items[i].barcode == item.barcode)
                    {
                        inventory.items[i].quantity -= item.quantity;
                        break;
                    }
                }
                inventory.writeItems();
                inventory.loadItems();
                return;
            }
        }

        items[size] = item;
        size++;
        for (int i = 0; i < inventory.size; i++)
        {
            if (inventory.items[i].barcode == item.barcode)
            {
                inventory.items[i].quantity -= item.quantity;
                inventory.writeItems();
                inventory.loadItems();
                break;
            }
        }
    }
    void removeItemFromOrder(string barcode, Inventory &inventory)
    {
        if (status)
        {
            cout << "Order is completed";
            return;
        }
        for (int i = 0; i < size; i++)
        {
            if (items[i].barcode == barcode)
            {
                for (int j = 0; j < inventory.size; j++)
                {
                    if (inventory.items[j].barcode == barcode)
                    {
                        inventory.items[j].quantity += items[i].quantity;
                        inventory.writeItems();
                        inventory.loadItems();
                        break;
                    }
                }
                for (int j = i; j < size - 1; j++)
                {
                    items[j] = items[j + 1];
                }
                size--;
                return;
            }
        }
        cout << "Item not found\n";
    }
    void refreshOrder(Inventory &inventory)
    {
        for (int i = 0; i < size;)
        {
            bool found = false;
            for (int j = 0; j < inventory.size; j++)
            {
                if (items[i].barcode == inventory.items[j].barcode)
                {
                    items[i].changeItemName(inventory.items[j].name);
                    items[i].changeItemPrice(inventory.items[j].price);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                for (int j = i; j < size - 1; j++)
                {
                    items[j] = items[j + 1];
                }
                size--;
            }
            else
            {
                i++;
            }
        }
    }

    void calculateBill(Inventory &inventory)
    {
        refreshOrder(inventory);
        itemQuantities = 0;
        basePrice = 0;
        int choice;
        cout << "Press 1 for card payment, 0 for cash payment : ";
        cin >> choice;
        while (choice != 1 && choice != 0)
        {
            cout << "Invalid\nEnter again: ";
            cin >> choice;
        }
        paymentMethod = (choice == 1) ? "card" : "cash";
        tax = (choice == 1) ? 5 : 15;

        for (int i = 0; i < size; i++)
        {
            itemQuantities += items[i].quantity;
            basePrice += items[i].price * items[i].quantity;
        }
        totalPrice = basePrice + (basePrice * (tax / 100.0));
    }
    void printOrderDetails(Inventory &inventory)
    {
        if (!status)
            calculateBill(inventory);
        cout << "Item details :";
        for (int i = 0; i < size; i++)
        {
            items[i].printItemDetails();
        }
        cout << "\n\n";
        cout << "Total Item quantities: " << itemQuantities << endl;
        cout << "Base Price: " << basePrice << endl;
        cout << "Tax: " << tax << endl;
        cout << "Total Price: " << totalPrice << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Status: ";
        if (status)
            cout << "Completed" << endl;
        else
            cout << "Pending" << endl;
    }

    Order &operator=(const Order &other)
    {
        if (this == &other)
            return *this;

        delete[] items;
        size = other.size;
        capacity = other.capacity;
        itemQuantities = other.itemQuantities;
        basePrice = other.basePrice;
        tax = other.tax;
        totalPrice = other.totalPrice;
        paymentMethod = other.paymentMethod;
        status = other.status;
        items = new Item[capacity];
        for (int i = 0; i < size; i++)
        {
            items[i] = other.items[i];
        }
        return *this;
    }
    Order(const Order &other)
    {
        size = other.size;
        capacity = other.capacity;
        itemQuantities = other.itemQuantities;
        basePrice = other.basePrice;
        tax = other.tax;
        totalPrice = other.totalPrice;
        paymentMethod = other.paymentMethod;
        status = other.status;

        items = new Item[capacity];
        for (int i = 0; i < size; i++)
        {
            items[i] = other.items[i];
        }
    }
    bool completeOrder()
    {
        double payedAmount;
        cout << "Enter amount payed: ";
        cin >> payedAmount;
        while (payedAmount < 0)
        {
            cout << "Invalid\nEnter again: ";
            cin >> payedAmount;
        }
        if (payedAmount < totalPrice)
        {
            cout << "Insufficient amount\n";
            return false;
        }
        status = true;
        return true;
    }
    ~Order()
    {
        delete[] items;
    }
};
struct OrderHistoryNode
{
    Order order;
    OrderHistoryNode *next;
    OrderHistoryNode(Order order)
    {
        this->order = order;
        next = nullptr;
    }
};
struct OrderHistoryList
{
    OrderHistoryNode *head;
    OrderHistoryList()
    {
        head = NULL;
    }
    void addOrder(OrderHistoryNode *node)
    {
        if (head == NULL)
            head = node;
        else
        {
            node->next = head;
            head = node;
        }
    }

    void removeOrder(int index)
    {
        if (head == NULL)
        {
            cout << "No orders to remove\n";
            return;
        }
        if (index == 0)
        {
            OrderHistoryNode *temp = head;
            head = head->next;
            delete temp;
        }
        else
        {
            OrderHistoryNode *temp = head;
            for (int i = 1; i < index; i++)
            {
                if (temp->next == NULL)
                {
                    cout << "Invalid index\n";
                    return;
                }
                temp = temp->next;
            }
            if (temp->next == NULL)
            {
                cout << "Invalid index\n";
                return;
            }
            OrderHistoryNode *temp2 = temp->next;
            temp->next = temp->next->next;
            delete temp2;
            cout << "Order removed.\n";
        }
    }
    void printOrdersHistory(Inventory &inventory)
    {
        for (OrderHistoryNode *temp = head; temp != NULL; temp = temp->next)
        {
            temp->order.printOrderDetails(inventory);
            cout << "\n\n";
        }
    }
};
struct Store
{
    Inventory inventory;
    Order *pendingOrders;
    int orderSize = 0;
    int orderCapacity = 1;
    OrderHistoryList orderHistoryList;
    double revenue;

    Store()
    {
        pendingOrders = new Order[orderCapacity];
        revenue = 0;
    }
    void createOrder()
    {
        if (orderSize == orderCapacity)
        {
            orderCapacity *= 2;
            Order *newOrder = new Order[orderCapacity];
            for (int i = 0; i < orderSize; i++)
            {
                newOrder[i] = pendingOrders[i];
            }
            delete[] pendingOrders;
            pendingOrders = newOrder;
        }
        Order order;
        pendingOrders[orderSize] = order;
        orderSize++;
    }
    void removeOrder(int index)
    {
        if (index < 0 || index >= orderSize)
        {
            cout << "Invalid index!\n";
            return;
        }
        for (int i = index; i < orderSize - 1; i++)
        {
            pendingOrders[i] = pendingOrders[i + 1];
        }
    }

    void printPendingOrders()
    {
        for (int i = 0; i < orderSize; i++)
        {
            pendingOrders[i].printOrderDetails(inventory);
        }
    }
    void printSortOrders()
    {
        cout << "First Enter payment methods for all the orders\n";
        for (int i = 0; i < orderSize; i++)
        {
            pendingOrders[i].calculateBill(inventory);
        }
        for (int i = 0; i < orderSize; i++)
        {
            for (int j = 0; j < orderSize - i - 1; j++)
            {
                if (pendingOrders[j].totalPrice > pendingOrders[j + 1].totalPrice)
                {
                    swap(pendingOrders[j], pendingOrders[j + 1]);
                }
            }
        }
        for (int i = 0; i < orderSize; i++)
        {
            cout << "Order index: " << i << endl;
            cout << "Item details : \n";
            for (int j = 0; j < pendingOrders[i].size; j++)
            {
                pendingOrders[i].items[j].printItemDetails();
            }
            cout << "\n";
            cout << "Total Item quantities: " << pendingOrders[i].itemQuantities << endl;
            cout << "Base Price: " << pendingOrders[i].basePrice << endl;
            cout << "Tax: " << pendingOrders[i].tax << endl;
            cout << "Total Price: " << pendingOrders[i].totalPrice << endl;
            cout << "Payment Method: " << pendingOrders[i].paymentMethod << endl;
            cout << "Status: ";
            if (pendingOrders[i].status)
                cout << "Completed" << endl;
            else
                cout << "Pending" << endl;
            cout << "\n\n";
        }
    }
    void calculateRevenue()
    {
        cout << "Total revenue is : " << revenue << endl;
    }
    void menu()
    {
        int choice;
        do
        {
            cout << "\n=============== STORE MANAGEMENT SYSTEM ===============\n";
            cout << "1.  Create New Order\n";
            cout << "2.  Add Item to Pending Order\n";
            cout << "3.  Remove Item from Pending Order\n";
            cout << "4.  Complete Order\n";
            cout << "5.  View Pending Orders\n";
            cout << "6.  View Pending Orders (Sorted by Total Price)\n";
            cout << "7.  Remove Pending Order\n";
            cout << "8.  View Order History\n";
            cout << "9.  Update Item in Inventory\n";
            cout << "10. Add New Item to Inventory\n";
            cout << "11. Remove Item from Inventory\n";
            cout << "12. View Item Details\n";
            cout << "13. View Inventory\n";
            cout << "14. Calculate Total Revenue\n";
            cout << "15. Remove Completed Order from Order History\n";
            cout << "0.  Exit\n";
            cout << "========================================================\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                createOrder();
                cout << "New empty order created.\n";
                break;
            }

            case 2:
            {
                if (orderSize == 0)
                {
                    cout << "No pending orders.\n";
                    break;
                }
                int orderIndex;
                cout << "Enter order index (0 to " << orderSize - 1 << "): ";
                cin >> orderIndex;
                if (orderIndex < 0 || orderIndex >= orderSize)
                {
                    cout << "Invalid order index.\n";
                    break;
                }

                string barcode;
                int quantity;
                cout << "Enter item barcode: ";
                cin >> barcode;
                while (barcode.length() != 10)
                {
                    cout << "Invalid barcode.\n";
                    cout << "Enter item barcode: ";
                    cin >> barcode;
                }
                cout << "Enter quantity: ";
                cin >> quantity;
                while (quantity < 0)
                {
                    cout << "Invalid quantity.\n";
                    cout << "Enter quantity: ";
                    cin >> quantity;
                }
                Item *item = nullptr;
                bool itemFound = false;
                for (int i = 0; i < inventory.size; i++)
                {
                    if (inventory.items[i].barcode == barcode)
                    {
                        if (inventory.items[i].quantity >= quantity)
                        {
                            item = &inventory.items[i];
                            itemFound = true;
                        }
                        else
                        {
                            cout << "Insufficient stock.\n";
                        }
                        break;
                    }
                }

                if (!itemFound)
                {
                    cout << "Item not found\n";
                }
                if (item)
                {
                    Item copy = *item;
                    copy.quantity = quantity;
                    pendingOrders[orderIndex].addItemInOrder(copy, inventory);
                    cout << "Item added to order.\n";
                }
                break;
            }

            case 3:
            {
                if (orderSize == 0)
                {
                    cout << "No pending orders.\n";
                    break;
                }
                int orderIndex;
                cout << "Enter order index: ";
                cin >> orderIndex;
                if (orderIndex < 0 || orderIndex >= orderSize)
                {
                    cout << "Invalid order index.\n";
                    break;
                }

                string barcode;
                cout << "Enter barcode to remove: ";
                cin >> barcode;
                if (barcode.length() != 10)
                {
                    cout << "Invalid barcode.\n";
                    break;
                }
                pendingOrders[orderIndex].removeItemFromOrder(barcode, inventory);
                break;
            }

            case 4:
            {
                if (orderSize == 0)
                {
                    cout << "No pending orders.\n";
                    break;
                }
                int orderIndex;
                cout << "Enter order index to complete: ";
                cin >> orderIndex;
                if (orderIndex < 0 || orderIndex >= orderSize)
                {
                    cout << "Invalid order index.\n";
                    break;
                }

                if (pendingOrders[orderIndex].size == 0)
                {
                    cout << "Cannot complete empty order.\n";
                    break;
                }
                pendingOrders[orderIndex].calculateBill(inventory);
                bool completed = pendingOrders[orderIndex].completeOrder();
                if (completed)
                {
                    OrderHistoryNode *node = new OrderHistoryNode(pendingOrders[orderIndex]);
                    revenue += node->order.totalPrice;
                    orderHistoryList.addOrder(node);

                    for (int i = orderIndex; i < orderSize - 1; i++)
                    {
                        pendingOrders[i] = pendingOrders[i + 1];
                    }
                    orderSize--;
                    cout << "Order completed and moved to history.\n";
                }
                break;
            }

            case 5:
                cout << "\n--- PENDING ORDERS ---\n";
                printPendingOrders();
                break;

            case 6:
                cout << "\n--- PENDING ORDERS (SORTED BY PRICE) ---\n";
                printSortOrders();
                break;

            case 7:
            {
                if (orderSize == 0)
                {
                    cout << "No pending orders to remove.\n";
                    break;
                }
                int index;
                cout << "Enter order index to remove: ";
                cin >> index;
                removeOrder(index);
                if (index >= 0 && index < orderSize)
                {
                    orderSize--;
                    cout << "Order removed.\n";
                }
                break;
            }

            case 8:
                cout << "\n--- ORDER HISTORY ---\n";
                orderHistoryList.printOrdersHistory(inventory);
                break;

            case 9:
            {
                string barcode, name;
                double price;
                int quantity;
                cout << "Enter item barcode: ";
                cin >> barcode;
                while (barcode.length() != 10)
                {
                    cout << "Invalid barcode.\n";
                    cout << "Enter item barcode: ";
                    cin >> barcode;
                }
                int subChoice;
                cout << "Update: 1.Name 2.Price 3.Quantity: ";
                cin >> subChoice;
                switch (subChoice)
                {
                case 1:
                    cout << "New name: ";
                    cin.ignore();
                    getline(cin, name);
                    inventory.updateItem(barcode, name);
                    break;
                case 2:
                    cout << "New price: ";
                    cin >> price;
                    while (price <= 0)
                    {
                        cout << "Price must be greater than 0\n";
                        cout << "New price: ";
                        cin >> price;
                    }
                    inventory.updateItem(barcode, price);
                    break;
                case 3:
                    cout << "New quantity: ";
                    cin >> quantity;
                    while (quantity < 0)
                    {
                        cout << "Quantity must be greater than 0\n";
                        cout << "New quantity: ";
                        cin >> quantity;
                    }

                    inventory.updateItem(barcode, quantity);
                    break;
                default:
                    cout << "Invalid choice.\n";
                }
                break;
            }

            case 10:
            {
                string name, barcode;
                double price;
                int quantity;
                cout << "Name: ";
                cin.ignore();
                getline(cin, name);
                while (name.length() == 0)
                {
                    cout << "Name cannot be empty\n";
                    cout << "Name: ";
                    cin.ignore();
                    getline(cin, name);
                }
                cout << "Barcode: ";
                cin >> barcode;
                while (barcode.length() != 10)
                {
                    cout << "Barcode must have 10 length\n";
                    cout << "Barcode: ";
                    cin >> barcode;
                }
                cout << "Price: ";
                cin >> price;
                while (price <= 0)
                {
                    cout << "Price must be greater than 0\n";
                    cout << "Price: ";
                    cin >> price;
                }
                cout << "Quantity: ";
                cin >> quantity;
                while (quantity < 0)
                {
                    cout << "Quantity must be greater than 0\n";
                    cout << "Quantity: ";
                    cin >> quantity;
                }
                inventory.addItem(name, barcode, price, quantity);
                cout << "Item added to inventory.\n";
                break;
            }

            case 11:
            {
                string barcode;
                cout << "Enter barcode to remove: ";
                cin >> barcode;
                while (barcode.length() != 10)
                {
                    cout << "Barcode must have 10 length\n";
                    cout << "Barcode: ";
                    cin >> barcode;
                }
                inventory.removeItem(barcode);
                break;
            }

            case 12:
            {
                string barcode;
                cout << "Enter barcode: ";
                cin >> barcode;
                while (barcode.length() != 10)
                {
                    cout << "Barcode must have 10 length\n";
                    cout << "Barcode: ";
                    cin >> barcode;
                }
                inventory.printItemDetails(barcode);
                break;
            }

            case 13:
                inventory.printInventory();
                break;

            case 14:
                calculateRevenue();
                break;
            case 15:
            {
                int index;
                cout << "Enter index to remove: ";
                cin >> index;
                while (index < 0)
                {
                    cout << "Invalid Index\nEnter index again: ";
                    cin >> index;
                }
                orderHistoryList.removeOrder(index);
                break;
            }
            case 0:
                cout << "Exiting...\n";
                exit(0);
                break;

            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};
int main()
{
    Store store;
    store.menu();
}
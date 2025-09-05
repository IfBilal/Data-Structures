#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *next;
    Node *prev;
    Node(int data)
    {
        this->data = data;
        next = prev = NULL;
    }
};
struct DoubleList
{
    Node *head;
    DoubleList()
    {
        head = NULL;
    }
    void push_front(int data)
    {
        Node *newNode = new Node(data);
        if (!newNode)
        {
            cout << "Memory not allocated" << endl;
            return;
        }
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void insertRandom(int data, int index)
    {
        Node *newNode = new Node(data);
        if (!newNode)
        {
            cout << "Memory not allocated!\n";
            return;
        }
        if (index < 0)
        {
            cout << "Invalid index\n";
            return;
        }
        if (index == 0)
        {
            if (head == NULL)
            {
                head = newNode;
            }
            else
            {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
        }
        else
        {
            if(head == NULL)
            {
                cout<<"Invalid index\n";
                return;
            }
            Node *temp = head;
            for (int i = 1; i < index; i++)
            {
                temp = temp->next;
                if (temp == NULL)
                {
                    cout << "Invalid Index\n";
                    return;
                }
            }
            if(temp->next == NULL)
            {
                temp->next = newNode;
                newNode->prev = temp;
            }
            else
            {
                newNode->next = temp->next;
                temp->next->prev = newNode;
                temp->next = newNode;
                newNode->prev = temp;
            }
        }
    }

    void pop_front()
    {
        if(head == NULL)
        {
            cout<<"List is empty\n";
            return;
        }
        if(head->next == NULL){
            delete head;
            head = NULL;
        }
        else{
            Node *temp = head;
            head = head->next;
            delete temp;
            head->prev = NULL;
        }
    }

    void pop_back()
    {
        if(head == NULL)
        {
            cout<<"List is empty\n";
            return;
        }
        if(head->next == NULL)
        {
            delete head;
            head = NULL;
        }
        else
        {
            Node* temp = head;
            while(temp->next->next!=NULL)
            {
                temp=temp->next;
            }
            delete temp->next;
            temp->next = NULL;
        }
    }

    void push_back(int data)
    {
        Node *newNode = new Node(data);
        if (!newNode)
        {
            cout << "Memory not allocated\n";
            return;
        }
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            Node *temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            newNode->prev = temp;
            temp->next = newNode;
        }
    }
    void display()
    {
        cout << "List data is : ";
        for (Node *temp = head; temp != NULL; temp = temp->next)
        {
            cout << temp->data << "  ";
        }
        cout << endl;
    }
};
int main()
{
}
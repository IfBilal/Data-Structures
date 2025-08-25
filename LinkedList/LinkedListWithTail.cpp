#include <iostream>
using namespace std;
class Node
{
public:
    int data;
    Node *next;
    Node(int val)
    {
        data = val;
        next = NULL;
    }
};

class LinkedList
{
private:
    Node *head;
    Node *tail;

public:
    LinkedList()
    {
        head = tail = NULL;
    }
    void push_front(int val)
    {
        Node *newNode = new Node(val);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }
    void push_back(int val)
    {
        Node *newNode = new Node(val);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void pop_front()
    {
        if (head == NULL)
        {
            return;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            if (head == NULL)
                tail = NULL;
            delete temp;
        }
    }
    void pop_back()
    {
        if (head == NULL)
        {
            return;
        }
        else
        {
            if (head == tail)
            {
                delete head;
                head = tail = NULL;
                return;
            }
            Node *temp = head;
            while (temp->next != tail)
            {
                temp = temp->next;
            }
            temp->next = NULL;
            delete tail;
            tail = temp;
        }
    }
    void removeByIndex(int index)
    {
        if (index < 0)
        {
            return;
        }
    }
    void printLinkedList()
    {
        for (Node *temp = head; temp != NULL; temp = temp->next)
        {
            cout << temp->data << "  ";
        }
        cout << endl;
    }
    void deleteAtIndex(int index)
    {
        if (index < 0 || head == NULL)
            return;
        Node *temp = head;
        if (index == 0)
        {
            head = temp->next;
            if (head == NULL)
                tail = NULL;
            delete temp;
            return;
        }
        for (int i = 0; i < index - 1; i++)
        {
            if (temp->next == NULL)
            {
                cout << "No node at given index\n";
                return;
            }
            temp = temp->next;
        }
        if (temp->next == NULL)
        {
            cout << "No node at given index\n";
            return;
        }
        if (temp->next == tail)
        {
            delete tail;
            tail = temp;
            tail->next = NULL;
            return;
        }
        Node *delOne = temp->next;
        temp->next = temp->next->next;
        delete delOne;
    }
};
int main()
{
}
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
    void printLinkedList()
    {
        for (Node *temp = head; temp != NULL; temp = temp->next)
        {
            cout << temp->data << "  ";
        }
        cout << endl;
    }
    void reverseLinkedList()
    {
        Node *prev = NULL;
        Node *current = head;
        Node *next;
        tail = head;
        while (current != NULL)
        {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }
};
int main()
{
    LinkedList list;
    list.push_back(5);
    list.push_back(4);
    list.push_back(3);
    list.push_back(2);
    list.push_back(1);
    list.printLinkedList();
    list.reverseLinkedList();
    list.printLinkedList();
}

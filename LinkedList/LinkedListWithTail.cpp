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
            temp->next = NULL;
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
        if (index < 0)
            return;
        if (head == NULL)
            return;
        Node *temp = head;
        for (int i = 0; i < index - 1; i++)
        {
            if (temp->next == NULL)
            {
                cout << "No node at given index\n";
                return;
            }
            temp = temp->next;
        }
        if (temp != head && temp->next == NULL)
        {
            cout << "No node at given index\n";
            return;
        }
        if (temp == head)
        {
            head = temp->next;
            if (head == NULL)
                tail = NULL;
            delete temp;
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
    LinkedList List;
    List.push_front(5);
    List.push_front(6);
    List.push_back(1);
    List.push_back(2);
    List.push_back(7);
    List.push_back(8);
    List.printLinkedList();
    List.pop_front();
    List.printLinkedList();
    List.pop_back();
    List.printLinkedList();
    List.deleteAtIndex(-1);
    List.printLinkedList();
    List.deleteAtIndex(3);
    List.printLinkedList();
    List.deleteAtIndex(2);
    List.printLinkedList();
    List.deleteAtIndex(0);
    List.printLinkedList();
}
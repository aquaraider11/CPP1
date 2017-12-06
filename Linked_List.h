#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <iostream>

template <typename T>
class Linked_List
{
    struct Node
    {
        Node(T& data)
        :   data (data)
        { }

        Node* next = nullptr;
        Node* prev = nullptr;

        T data;
    };

    public:
        ~Linked_List()
        {
            clear();
        }

        Linked_List() = default;

        Linked_List (const Linked_List& other)
        {
            *this = other;
        }

        Linked_List& operator = (const Linked_List& other)
        {
            clear();

            Node* itr = other.first;

            while(itr->next)
            {
                push_back(itr->data);
                itr = itr->next;
            }

            return *this;
        }

        Linked_List& operator = (Linked_List&& other)
        {
            clear();

            first = other.first;
            last  = other.last;
            count = other.count;

            other.first = nullptr;
            other.last  = nullptr;
            other.count = 0;

            return *this;
        }

        Linked_List (Linked_List&& other)
        {
            *this = std::move(other);
        }

        void clear()
        {
            while(count != 0) erase(0);
        }

        void push_back(T& data)
        {
            Node* node = new Node(data);

            if (count == 0)
            {
                last = node;
                first = node;
                count++;
                return;
            }

            last->next = node;
            node->prev = last;
            last = node;

            count++;
        }

        void push_front(T& data)
        {
            Node* node = new Node(data);

            if (count == 0)
            {
                last = node;
                first = node;
                count++;
                return;
            }

            first->prev = node;
            node->next = first;
            first = node;

            count++;
        }

        void insert (T& data, int where)
        {
            if (where == 0)
                push_front(data);
            else if (where == count - 1)
                push_back(data);


            Node* node = new Node(data);

            Node* itr = first;
            for (int i = 0 ; i < where - 1 ; i++)
            {
                itr = itr->next;
            }

            itr->next->prev = node;
            node->next = itr->next;

            itr->next = node;
            node->prev = itr;

            count++;
        }

        T& get(int where)
        {
            Node* itr = first;

            for (int i = 0 ; i < where ; i++)
            {
                itr = itr->next;
            }

            return itr->data;
        }

        void erase(int where)
        {
            if (empty())
                return;

            Node* itr = first;

            count--;

            if (where == 0)
            {
                Node* temp = first;
                first = first->next;
                delete temp;
                return;
            }
            if (where == count - 1)
            {
                Node* temp = last;
                last = last->prev;
                delete temp;
                return;
            }

            for (int i = 0 ; i < where ; i++)
            {
                itr = itr->next;
            }

            itr->prev->next = itr->next;
            itr->next->prev = itr->prev;

            delete itr;
        }

        //Returns number of elements in list
        int size()
        {
            return count;
        }

        //Returns true if list is empty
        bool empty()
        {
            return count == 0;
        }


    private:
        Node* first = nullptr;
        Node* last  = nullptr;

        int count = 0;
};

#endif // LINKED_LIST_H_INCLUDED

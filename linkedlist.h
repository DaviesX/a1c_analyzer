/* @author: Zhaonian Luan, Copyright 2016 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <ostream>
#include <vector>


template <class T>
class LinkedList
{
        template <typename K> friend std::ostream& operator<<(std::ostream& os, const LinkedList<K>& ln);
public:
        LinkedList();
        ~LinkedList();

        bool            is_empty() const;
        T*              front();
        void            push_front(const T& value);
        T*              find(const T& value);
        void            remove(const T& value);
        void            extract(std::vector<T>& values) const;
        void            clear();
private:
        class Node
        {
        public:
                Node(T v): value(v) {}
                T       value;
                Node*   next;
        };

        Node*           head = nullptr;
};

template <class T>
LinkedList<T>::LinkedList()
{
}

template <class T>
LinkedList<T>::~LinkedList()
{
        clear();
}

template <class T>
bool LinkedList<T>::is_empty() const
{
        return head == nullptr;
}

template <class T>
T* LinkedList<T>::front()
{
        return head != nullptr ? &head->value : nullptr;
}

template <class T>
void LinkedList<T>::push_front(const T& value)
{
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
}


template <class T>
T* LinkedList<T>::find(const T& value)
{       Node* new_node = nullptr;
        new_node = head;
        while (new_node != nullptr) {
                if (new_node->value == value) {
                        return &new_node->value;
                }
                new_node = new_node->next;
        }
        return nullptr;
}

template <class T>
void LinkedList<T>::remove(const T& value)
{
        Node* current_node = head;
        Node* previous_node = nullptr;
        while (current_node != nullptr) {
                if (current_node->value == value) {
                        if (previous_node != nullptr) {
                                previous_node->next = current_node->next;
                                delete previous_node->next;
                        } else {
                                head = nullptr;
                                delete current_node;
                        }
                }
                previous_node = current_node;
                current_node = current_node->next;
        }
}

template <class T>
void LinkedList<T>::extract(std::vector<T>& values) const
{
        auto node = head;
        while (node != nullptr) {
                values.push_back(node->value);
                node = node->next;
        }
}

template <class T>
void LinkedList<T>::clear()
{
        Node* node = head;
        while (node != nullptr) {
                Node* tmp = node;
                node = node->next;
                delete tmp;
        }
        head = nullptr;
}

template <typename K>
std::ostream& operator<<(std::ostream& os, const LinkedList<K>& ln)
{
        os << "LinkedList = [";
        auto new_node = ln.head;
        while (new_node != nullptr) {
                if (new_node->next != nullptr)
                        os << new_node->value << ",";
                else
                        os << new_node->value;
                new_node = new_node->next;
        }
        os << "]";
        return os;
}

#endif // LINKEDLIST_H

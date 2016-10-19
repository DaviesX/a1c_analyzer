#ifndef LINKEDLinkedBST_H
#define LINKEDLinkedBST_H

#include "linkedlist.h"


template <class T>
class LinkedBST
{
        template <typename K> friend std::ostream& operator<<(std::ostream& os, const LinkedBST<K>& bst);
        template <typename K> friend void bst_inorder_print(std::ostream& os, const typename LinkedBST<K>::Node* node);

public:
        LinkedBST();
        ~LinkedBST();

        void            insert(const T& value);
        LinkedList<T>*  find(const T& value);
        void            remove(const T& value);
        void            clear();
        unsigned        size() const;
        bool            is_empty() const;

private:
        struct Node
        {
                LinkedList<T>   value;
                Node*           left;
                Node*           right;

               bool operator < (const T& value)
               {
                       return *this->value.front() < value;
               }

               bool operator > (const T& value)
               {
                       return *this->value.front() > value;
               }
        };

        void    destroy_subtree(Node* node);

        Node*           root = nullptr;
        unsigned        n_elems = 0;
};

template <class T>
LinkedBST<T>::LinkedBST()
{

}

template <class T>
LinkedBST<T>::~LinkedBST()
{
        clear();
}

template <class T>
unsigned LinkedBST<T>::size() const
{
        return n_elems;
}

template <class T>
bool LinkedBST<T>::is_empty() const
{
        return root == nullptr;
}

template <class T>
void LinkedBST<T>::insert(const T& value)
{
        n_elems ++;

        Node* current_node = root;
        Node* old_node = nullptr;
        while (current_node != nullptr) {
                if (*current_node > value) {
                        old_node = current_node;
                        current_node = current_node->left;
                } else if (*current_node < value) {
                        old_node = current_node;
                        current_node = current_node->right;
                } else {
                        // Insert to this position.
                        current_node->value.push_front(value);
                        return ;
                }
        }
        // Insert to a new position.
        // Since the node doesn't exit, create a new node.
        Node* new_node = new Node;
        new_node->value.push_front(value);
        new_node->left = nullptr;
        new_node->right = nullptr;
        if (old_node == nullptr)
                root = new_node;
        else {
                if (*old_node > value)
                        old_node->left = new_node;
                else
                        old_node->right = new_node;
        }
}

template <class T>
LinkedList<T>* LinkedBST<T>::find(const T& value)
{
        Node* current_node = root;
        while (current_node != nullptr) {
                if (*current_node > value) {
                        current_node = current_node->left;
                } else if (*current_node < value) {
                        current_node = current_node->right;
                } else {
                        return &current_node->value;
                }
        }
        return nullptr;
}

template<class T>
void LinkedBST<T>::destroy_subtree(typename LinkedBST<T>::Node* node)
{
        if (node == nullptr)
                return;
        else {
                destroy_subtree(node->left);
                destroy_subtree(node->right);
                node->value.clear();
                delete node;
        }
}

template <class T>
void LinkedBST<T>::clear()
{
        destroy_subtree(root);
        root = nullptr;
        n_elems = 0;
}

template <typename K>
void linked_bst_inorder_print(std::ostream& os, const typename LinkedBST<K>::Node* node)
{
        if (node == nullptr) {
        } else {
                ::linked_bst_inorder_print<K>(os, node->left);
                os << node->value << ",";
                ::linked_bst_inorder_print<K>(os, node->right);
        }
}

template <typename K>
std::ostream& operator<<(std::ostream& os, const LinkedBST<K>& lbst)
{
        os << "LinkedBST = [";
        ::linked_bst_inorder_print<K>(os, lbst.root);
        os << "]";
        return os;
}

#endif // LINKEDLinkedBST_H

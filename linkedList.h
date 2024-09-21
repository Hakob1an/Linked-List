// linkedlist.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"
#include <iostream>

template <typename T>
class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void addAfter(Node<T>* node, const T& value);
    void addBefore(Node<T>* node, const T& value);
    void remove(Node<T>* node);
    Node<T>* access(int index);
    void clear();
    int size() const;
    int capacity() const; // For linked list, capacity is dynamic
    LinkedList<T>* copy() const;

    Node<T>* head() const { return head_; }

    void printList() const;

private:
    Node<T>* head_;
    int size_;
};

// Implementations should be included in the header for templates
#include "linkedList_impl.h"

#endif // LINKEDLIST_H

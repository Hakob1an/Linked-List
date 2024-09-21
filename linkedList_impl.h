// linkedlist_impl.h
#ifndef LINKEDLIST_IMPL_H
#define LINKEDLIST_IMPL_H

#include "linkedList.h"

template <typename T>
LinkedList<T>::LinkedList() : head_(nullptr), size_(0) {}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::addAfter(Node<T>* node, const T& value) {
    if (!node) {
        // Add at the beginning if node is null
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head_;
        head_ = newNode;
    } else {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = node->next;
        node->next = newNode;
    }
    ++size_;
}

template <typename T>
void LinkedList<T>::addBefore(Node<T>* node, const T& value) {
    if (node == head_ || !head_) {
        // Add at the beginning
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head_;
        head_ = newNode;
    } else {
        Node<T>* prev = head_;
        while (prev && prev->next != node) {
            prev = prev->next;
        }
        if (prev) {
            Node<T>* newNode = new Node<T>(value);
            newNode->next = node;
            prev->next = newNode;
        }
    }
    ++size_;
}

template <typename T>
void LinkedList<T>::remove(Node<T>* node) {
    if (!node || !head_) return;

    if (node == head_) {
        Node<T>* temp = head_;
        head_ = head_->next;
        delete temp;
    } else {
        Node<T>* prev = head_;
        while (prev && prev->next != node) {
            prev = prev->next;
        }
        if (prev && prev->next == node) {
            prev->next = node->next;
            delete node;
        }
    }
    --size_;
}

template <typename T>
Node<T>* LinkedList<T>::access(int index) {
    if (index < 0 || index >= size_) return nullptr;
    Node<T>* current = head_;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current;
}

template <typename T>
void LinkedList<T>::clear() {
    Node<T>* current = head_;
    while (current) {
        Node<T>* temp = current;
        current = current->next;
        delete temp;
    }
    head_ = nullptr;
    size_ = 0;
}

template <typename T>
int LinkedList<T>::size() const {
    return size_;
}

template <typename T>
int LinkedList<T>::capacity() const {
    return size_; // For linked list, capacity equals size
}

template <typename T>
LinkedList<T>* LinkedList<T>::copy() const {
    LinkedList<T>* newList = new LinkedList<T>();
    Node<T>* current = head_;
    Node<T>* prevNewNode = nullptr;
    while (current) {
        Node<T>* newNode = new Node<T>(current->data);
        if (!newList->head_) {
            newList->head_ = newNode;
        } else {
            prevNewNode->next = newNode;
        }
        prevNewNode = newNode;
        current = current->next;
    }
    newList->size_ = size_;
    return newList;
}

template <typename T>
void LinkedList<T>::printList() const {
    std::cout << "LinkedList (size = " << size_ << "): ";
    Node<T>* current = head_;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << " -> ";
        current = current->next;
    }
    std::cout << std::endl;
}

#endif // LINKEDLIST_IMPL_H

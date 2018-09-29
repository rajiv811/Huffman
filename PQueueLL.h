/**
 * @file PQueueLL.h - Linked list implementation of PriorityQueue ADT.
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 2430, Spring 2018"
 */

#pragma once
#include <iostream>
#include <stdexcept>
#include "adt/PriorityQueue.h"

/**
 * @class PQueueLL - Linked list implementation of PriorityQueue ADT
 * @tparam T data element type, must have 0-arg ctor, copy-ctor,
 *           < operator, and << operator to std::ostream.
 */
template <typename T>
class PQueueLL : public PriorityQueue<T> {
public:
    // Big 5
    PQueueLL();
    ~PQueueLL();
    PQueueLL(const PQueueLL& other);
    PQueueLL(PQueueLL&& temp);
    PQueueLL& operator=(const PQueueLL& other);
    PQueueLL& operator=(PQueueLL&& temp);

    const T& peek() const;
    void enqueue(const T& datum);
    void dequeue();
    bool empty() const;
    void clear();
    std::ostream& print(std::ostream& out) const;

private:
    struct Node {
        T data;
        Node *next;
        Node(const T& data, Node *next) : data(data), next(next) {}
    };
    Node *head;
};

template <typename T>
PQueueLL<T>::PQueueLL() : head(nullptr) {
}

template <typename T>
PQueueLL<T>::~PQueueLL() {
    clear();
}

template <typename T>
PQueueLL<T>::PQueueLL(const PQueueLL<T> &other) : PQueueLL() {
    *this = other;
}

template <typename T>
PQueueLL<T>::PQueueLL(PQueueLL<T> &&temp) : PQueueLL() {
    *this = temp;
}

template <typename T>
PQueueLL<T>& PQueueLL<T>::operator=(const PQueueLL<T> &other) {
    if (this != &other) {
        clear();
        Node *p = other.head;
        if (p != nullptr) {
            Node *tail = new Node(p->data, nullptr);
            head = tail;
            for (p = p->next; p != nullptr; p = p->next) {
                tail->next = new Node(p->data, nullptr);
				tail = tail->next;
			}
        }
    }
    return *this;
}

template <typename T>
PQueueLL<T>& PQueueLL<T>::operator=(PQueueLL<T> &&temp) {
    std::swap(head, temp.head);
}

template <typename T>
bool PQueueLL<T>::empty() const {
    return head == nullptr;
}

template <typename T>
const T& PQueueLL<T>::peek() const {
    if (empty())
        throw std::out_of_range("cannot peek in empty priority queue");
    return head->data;
}

template <typename T>
void PQueueLL<T>::dequeue() {
    if (empty())
        throw std::out_of_range("cannot dequeue from empty priority queue");
	Node *p = head;
    head = head->next;
	delete p;
}

template <typename T>
void PQueueLL<T>::enqueue(const T& datum) {
    if (empty() || datum < head->data)
        head = new Node(datum, head);
    else {
        Node *p = head;
        while (p->next != nullptr && p->next->data < datum)
            p = p->next;
        p->next = new Node(datum, p->next);
    }
}

template <typename T>
void PQueueLL<T>::clear() {
    while (head != nullptr) {
        Node *p = head;
        head = head->next;
        delete p;
    }
}

template <typename T>
std::ostream& PQueueLL<T>::print(std::ostream &out) const {
    std::string delim = "";
    for (Node *p = head; p != nullptr; p = p->next) {
        out << delim << p->data;
        delim = ", ";
    }
    return out;
}

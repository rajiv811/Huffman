/**
 * @file QueueL.h - Implementation of Queue ADT using std::list.
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 2430, Spring 2018"
 */

#pragma once
#include <list>
#include "adt/Queue.h"

/**
 * @class QueueL - Implementation of Queue ADT using std::list.
 * @tparam T data element type, must have 0-arg ctor, copy-ctor,
 *           and << operator to std::ostream.
 */
template <typename T>
class QueueL : public Queue<T> {
public:
    QueueL();
    ~QueueL();
    QueueL(const QueueL<T>& other);
    QueueL(QueueL<T>&& temp);
    QueueL<T>& operator=(const QueueL &other);
    QueueL<T>& operator=(QueueL&& temp);

    const T& peek() const;
    void enqueue(const T& datum);
    void dequeue();
    bool empty() const;
    void clear();
    std::ostream& print(std::ostream& out) const;
private:
    std::list<T> qlist;
};

// zero-arg constructor -- construct each data member
template <typename T>
QueueL<T>::QueueL() : qlist() {
}

// destructor -- compiler will destroy each data member no matter what we do here
template <typename T>
QueueL<T>::~QueueL() {
}

// copy constructor -- copy construct each data member
template <typename T>
QueueL<T>::QueueL(const QueueL<T>& other) : qlist(other.qlist) {
}

// copy assignment operator -- assign each data member
template <typename T>
QueueL<T>& QueueL<T>::operator=(const QueueL<T>& other) {
    qlist = other.qlist;
    return *this;
}

// move constructor -- minimally construct then swap each data member
template <typename T>
QueueL<T>::QueueL(QueueL<T>&& temp) : qlist() {
    std::swap(qlist, temp.qlist);
}

// move assignment operator -- swap each data member
template <typename T>
QueueL<T>& QueueL<T>::operator=(QueueL<T>&& temp) {
    std::swap(qlist, temp.qlist);
}

template <typename T>
const T& QueueL<T>::peek() const {
    return qlist.front();
}

template <typename T>
void QueueL<T>::enqueue(const T &datum) {
    qlist.push_back(datum);
}

template <typename T>
void QueueL<T>::dequeue() {
    qlist.pop_front();
}

template <typename T>
bool QueueL<T>::empty() const {
    return qlist.empty();
}

template <typename T>
void QueueL<T>::clear() {
    return qlist.clear();
}

template <typename T>
std::ostream& QueueL<T>::print(std::ostream &out) const {
    for (const auto element: qlist)
        out << element << " ";
    return out;
}

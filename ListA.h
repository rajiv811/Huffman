/**
 * @file ListA.h -- Array implementation of List ADT
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */
#pragma once
#include <iostream>
#include <stdexcept>
#include "adt/List.h"

/**
 * @class ListA<T> - template class for array implementation of List ADT.
 *
 * Public interface is the same as the ADT. Implementations are conformant
 * with efficiency guarantees in ADT comments. Additionally:
 * set, get:  O(1)
 * remove():  O(1)
 * remove(i): O(n)
 * insert(i): O(n)
 * append:    typically O(1), worst case is O(n), but amortized still O(1)
 */
template <typename T>
class ListA : public List<T> {
public:
    ListA();
    // "Big five":
    ~ListA();
    ListA(const ListA<T>& other);
    ListA& operator=(const ListA<T>& other);
    ListA(ListA<T>&& temp);
    ListA& operator=(ListA<T>&& temp);

    // The following methods implement the List ADT:
    int size() const;
    void set(int i, const T& element);
    const T& get(int i) const;
    int append(const T& element);
    void insert(int i, const T& element);
    void remove();
    void remove(int i);
    void clear();
    std::ostream& print(std::ostream& out) const;
private:
    static const int DEFAULT_CAPACITY = 7;	// initial capacity for a new ListA
    int capacity; // number of elements allocated in array
    int length;  // number of elements currently being used in array
    T *array;  // data storage of the elements

    void resize();
};

/*
 * Following are the templated implementations of the methods
 * (this is done in the header file because it is a template class).
 */

template <typename T>
ListA<T>::ListA() : capacity(0), length(0), array(nullptr) {
}

template <typename T>
ListA<T>::~ListA() {
    delete[] array;
}

template <typename T>
ListA<T>::ListA(const ListA<T>& other) : capacity(0), length(0), array(nullptr) {
    *this = other;  // just use the lvalue = operator
}

template <typename T>
ListA<T>::ListA(ListA<T>&& temp) : capacity(0), length(0), array(nullptr) {
    *this = temp;  // just use the rvalue = operator
}

template <typename T>
ListA<T>& ListA<T>::operator=(const ListA<T>& other) {
    // first check if we are doing something like x = x
    if (this != &other) {
        // see if we are big enough to get a copy of all other's elements
        if (capacity < other.length) {
            // we are not big enough, so resize to fit
            capacity = other.capacity;
            delete[] array;
            array = new T[capacity];
        }
        // copy the other's elements to this
        length = other.length;
        for (int i = 0; i < length; i++)
            array[i] = other.array[i];
    }
    return *this;
}

template <typename T>
ListA<T>& ListA<T>::operator=(ListA<T>&& temp) {
    std::swap(array, temp.array);
	std::swap(capacity, temp.capacity);
	std::swap(length, temp.length);
    return *this;
}

template <typename T>
int ListA<T>::size() const {
    return length;
}

template <typename T>
void ListA<T>::set(int i, const T& element) {
    if (i < 0 || i >= length)
        throw std::out_of_range("set past bounds");
    array[i] = element;
}

template <typename T>
const T& ListA<T>::get(int i) const {
    if (i < 0 || i >= length)
        throw std::out_of_range("get past bounds");
    return array[i];
}

template <typename T>
int ListA<T>::append(const T& element) {
    // if we don't have the capacity, then resize bigger
    if (length == capacity)
        resize();
    array[length++] = element;
    return length-1;
}

template <typename T>
void ListA<T>::insert(int i, const T& element) {
    if (i > length || i < 0)
        throw std::out_of_range("insert past bounds");
    // if we don't have the capacity, then resize bigger
    if (length == capacity)
        resize();
    length++;
    for (int j = i+1; j < length; j++)
        array[j] = array[j-1];
    array[i] = element;
}

template <typename T>
void ListA<T>::resize() {
    capacity = capacity*2 + DEFAULT_CAPACITY;
    T *bigger = new T[capacity];
    for (int i = 0; i < length; i++)
        bigger[i] = array[i];
    delete[] array;
    array = bigger;
}

template <typename T>
void ListA<T>::remove() {
    remove(length-1);
}

template <typename T>
void ListA<T>::remove(int i) {
    if (i >= length || i < 0)
        throw std::out_of_range("remove past bounds");
    length--;
    for (int j = i; j < length; j++)
        array[j] = array[j+1];
}

template <typename T>
void ListA<T>::clear() {
    length = 0;
}

template <typename T>
std::ostream& ListA<T>::print(std::ostream& out) const {
    std::string delim = "";
    for (int i = 0; i < length; i++) {
        out << delim << array[i];
        delim = ", ";
    }
    return out;
}

/**
 * @file adt/Queue.h - the Queue ADT
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC 2430, Spring 2018"
 */

#pragma once
#include <iostream>

/**
 * @class Queue<T> -- abstract base class for the Queue ADT
 *
 * The Queue Abstract Data Type is a list of same-datatype elements where insertions
 * happen at one end and deletions happen at the other.
 * The queue can be enlarged by enqueueing an element of type T to it. (Note that
 * T must have an accessible default constructor.) It can be shrunk by the dequeue
 * method. Only the element about to be dequeued can be accessed. It is accessed
 * with the peek method.
 *
 * @param T  Data type of the elements. Each T element is copied into the queue with
 *           the enqueue method.  A const reference to the copy is
 *           returned by the peek method. T therefore must have an accessible
 *           copy-constructor and assignment operator. Most implementations also
 *           require a default constructor. The ADT does not require that T support
 *           any comparison operators.
 */

template <typename T>
class Queue {
public:
    /**
     * Get a reference to the element stored at the beginning of this queue (oldest of it's current elements).
     *
     * Efficiency: O(1)
     * @return   a const reference to the top element in this queue
     * @throws   out_of_range if this queue is empty
     * @pre      !empty()
     */
    virtual const T& peek() const = 0;

    /**
     * Add a new element to the queue.
     *
     * Efficiency: depends on the implementation
     * @param datum  value to be copied onto the end of the queue
     */
    virtual void enqueue(const T& datum) = 0;

    /**
     * Remove the oldest element from the queue and destroy it.
     *
     * Efficiency: O(1)
     * @throws  out_of_range if empty()
     * @pre     !empty()
     * @post    one fewer item on the queue
     */
    virtual void dequeue() = 0;

    /**
     * Check if this queue has no elements.
     *
     * @return  true if there are no elements
     */
    virtual bool empty() const = 0;

    /**
     * Dequeue all the elements from the queue.
     *
     * @post  size() == 0
     */
    virtual void clear() = 0;

    /**
     * Print the contents to the given stream.
     *
     * Requires that T has << operator into std::ostream.
     * @param out  stream to print to
     * @return     modified output stream
     * @pre        out is writable
     * @post       out has a textual representation of list inserted (with <<)
     */
    virtual std::ostream& print(std::ostream& out) const = 0;

    virtual ~Queue() {} // make the destructors virtual
};

/**
 * Template operator<< overload for printing.
 *
 * This just redirects to the print method.
 */
template <typename T>
std::ostream& operator<<(std::ostream& out, const Queue<T>& queue) {
    return queue.print(out);
}

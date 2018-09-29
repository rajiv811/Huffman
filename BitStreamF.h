/**
 * @file BitsStreamF.h - Implementation of BitStream destined for a binary file.
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#pragma once

#include <list>
#include "adt/BitStream.h"
#include "Bits.h"
#include "ListA.h"

/**
 * @class BitStreamF - Implementation of BitStream destined for a binary file.
 *
 * Acts as a queue of bits.
 * May be written to a file with writeToFile() method.
 * May be written to an ostream via << operator, in which case you get the bits
 * as ASCII '0's and '1's.
 */
class BitStreamF : public BitStream {
public:
    /**
     * Construct an empty bit stream, suitable for receiving bits via enqueue.
     */
    BitStreamF();

    /**
     * Load a bit stream from a previously saved file (via writeToFile).
     * @param filename  path to file previously saved via writeToFile() method
     * @pre             filename is readable and was written with writeToFile()
     *                  method from some other instance.
     * @post            this BitStreamF is in the same state as the one that wrote out the file
     */
    BitStreamF(std::string filename);

    // implement BitStream ADT
    bool empty() const;
    bool full() const;
    bool dequeue();
    void enqueue(bool bit);
    BitStreamF *copy() const;

    /**
     * Write the bits out to the given file.
     *
     * @param filename   name of the file to write (will overwrite any existing file of the same name)
     * @pre              filename is creatable and writable
     * @post             filename exists with the bits in this BitStreamF (final bits are written as zeros
     *                   if the final byte is not completely used)
     */
    void writeToFile(std::string filename) const;
private:
    std::list<Bits> data;
};



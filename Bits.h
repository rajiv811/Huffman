/**
 * @file Bits.h - get and set bits from an integer
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#pragma once

#include <stddef.h>
#include <cstdint>
#include "adt/BitStream.h"

/**
 * @class Bits - get and set bits from an integer
 */
class Bits : public BitStream {
public:
    static const int MAX_BITS = sizeof(unsigned int) * 8;
    Bits();
    explicit Bits(unsigned int n);
    Bits(unsigned int n, int length);
    Bits(const Bits& other);

    bool dequeue();
    void enqueue(bool bit);
    bool empty() const;
    bool full() const;
    Bits *copy() const;

    unsigned int asInteger() const;
    int bitsUsed() const;

private:
    unsigned int integer;
    int length;
};



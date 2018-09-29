/**
 * @file Bits.cpp - 
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */
#include <stdexcept>
#include "Bits.h"
using namespace std;

Bits::Bits() : Bits(0, 0) {}
Bits::Bits(unsigned int n) : Bits(n, MAX_BITS) {}
Bits::Bits(unsigned int n, int length) : integer(n), length(length) {
    if (length > MAX_BITS)
        throw invalid_argument("exceeded max length of Bits: " + to_string(MAX_BITS));
}
Bits::Bits(const Bits &other) : integer(other.integer), length(other.length) {}

bool Bits::dequeue() {
    if (empty())
        throw out_of_range("cannot dequeue from an empty Bits");
    bool bit = (integer & 1u) == 1u;
    integer >>= 1;
    length--;
    return bit;
}

void Bits::enqueue(bool value) {
    if (full())
        throw overflow_error("Bits full");
    if (value)
        integer |= (1u << (unsigned int)length);
    else
        integer &= ~(1u << (unsigned int)length);  // should be a zero-bit already, but just in case...
    length++;
}

bool Bits::empty() const {
    return length == 0;
}

bool Bits::full() const {
    return length == MAX_BITS;
}

Bits *Bits::copy() const {
    return new Bits(*this);
}

unsigned int Bits::asInteger() const {
    return integer;
}

int Bits::bitsUsed() const {
    return length;
}


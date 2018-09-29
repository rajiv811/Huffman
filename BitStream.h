/**
 * @file adt/BitStream.h - Queue of bits
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#pragma once
#include <iostream>

class BitStream {
public:
    virtual bool empty() const = 0;
    virtual bool full() const = 0;

    virtual bool dequeue() = 0;
    virtual void enqueue(bool bit) = 0;

    virtual BitStream *copy() const = 0;

    virtual std::ostream& drain(std::ostream& out) {
        while (!empty())
            out << (dequeue() ? '1' : '0');
        return out;
    }

    virtual void append(BitStream& bits) {
        while (!bits.empty())
            enqueue(bits.dequeue());
    }

    BitStream& operator<<(const BitStream &bits) {
        BitStream *dup = bits.copy();
        append(*dup);
        delete dup;
        return *this;
    }

    BitStream& operator<<(BitStream &&bits) {
        append(bits);
        return *this;
    }

    virtual ~BitStream() = default;

    friend std::ostream& operator<<(std::ostream& out, const BitStream& bitstream) {
        BitStream *dup = bitstream.copy();
        dup->drain(out);
        delete dup;
        return out;
    }

    friend std::ostream& operator<<(std::ostream& out, BitStream&& bitstream) {
        return bitstream.drain(out);
    }
};





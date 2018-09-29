/**
 * @file BitStreamF.cpp - File implementation of BitStream ADT
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include <fstream>
#include "BitStreamF.h"
using namespace std;

BitStreamF::BitStreamF() : data() {
}

bool BitStreamF::empty() const {
    return data.empty();
}

bool BitStreamF::full() const {
    return false;  // never full
}

/*
 * Use the front to dequeue
 */
bool BitStreamF::dequeue() {
    if (empty())
        return false;
    Bits &front = data.front();
    bool bit = front.dequeue();
    if (front.empty())
        data.pop_front();
    return bit;
}

/*
 * Use the back to enqueue
 */
void BitStreamF::enqueue(bool bit) {
    if (data.empty() || data.back().full())
        data.emplace_back();
    data.back().enqueue(bit);
}

BitStreamF* BitStreamF::copy() const {
    return new BitStreamF(*this);
}

void BitStreamF::writeToFile(string filename) const {
    ofstream f;
    f.open(filename, ios::binary | ios::out);
    if (!f.is_open())
        throw invalid_argument(string("cannot open file ") + filename + " to write bit stream");

    // first write out the number of bits in the first and last bytes
    char len = static_cast<char>(data.front().bitsUsed());
    f.write(&len, 1);
    len = static_cast<char>(data.back().bitsUsed());
    f.write(&len, 1);

    // then write out all the Bits
    for (const auto& b: data) {
        unsigned int n = b.asInteger();
        f.write((char*)&n, sizeof(n));
    }
}

BitStreamF::BitStreamF(std::string filename) {
    ifstream f;
    f.open(filename, ios::binary | ios::in);
    if (!f.is_open())
        throw invalid_argument(string("cannot open file ") + filename + " to read bit stream");

    char firstLength, lastLength;
    f.read(&firstLength, 1);
    f.read(&lastLength, 1);

    // first Bits of correct length
    unsigned int datum;
    f.read((char*)&datum, sizeof(datum));
    data.emplace_back(datum, firstLength);

    // middle Bits (including last one which we will overwrite with the correct size after exiting loop)
    while (!f.eof() && f.read((char*)&datum, sizeof(datum)))
        data.emplace_back(datum);

    // replace final one with a Bits of correct length
    data.back() = Bits(datum, lastLength);
}


/**
 * @file p2.cpp - Huffman codec project
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "Huffman.h"
#include "BitStreamF.h"

using namespace std;

int main() {
  //string fnbook = "data/book.txt";  
  //string fnencoded = "data/book_encoded.dat";
  //string fnbookcopy = "data/book_copy.txt";
    string fnbook = "data/Ulysses.txt";
    string fnencoded = "data/Ulysses_encoded.dat";
    string fnbookcopy = "data/Ulysses_copy.txt";
    string fnzerosandones = "data/zerosandones.txt";
    /*
     * Construct the Huffman encoder/decoder by reading through the book
     */

    ifstream sample(fnbook);
    Huffman huffy(sample);
    cout << "Working" << endl;
    sample.close();
    /*
     * print out the code values
     */
    for (int c = 0; c <= Huffman::MAX_CHAR; c++)
        huffy.printCode(cout, c);

    /*
     * Now encode the book in the bit stream, codeout
     */
    ifstream in(fnbook);
    BitStreamF codeout;
    huffy.encode(in, codeout);

    /*
     * Now, take the bit stream, codeout, and write it out to a file (the compressed file).
     * @post  expect fnencoded file to be about 40% smaller than fnbook file
     */
    codeout.writeToFile(fnencoded);

    /*
     * for debugging we can instead write out a file of 0's and 1's that are human-readable
     * (the BitStreamF class has << overloaded to ostream as '0' and '1' for each bit)
     * @post  expect zerosandones.txt to be exactly 8x bigger than fnencoded with the corresponding bits as 0's and 1's
     */
    ofstream zao(fnzerosandones);
    zao << codeout;

    /*
     * verify the encoded file by reading it in and producing a copy of the original file
     * @post  expect fnbookcopy to be exactly identical to fnbook
     *        (to verify use $ diff Ulysses.txt Ulysses_copy.txt)
     */
    BitStreamF codein(fnencoded);
    ofstream out(fnbookcopy);
    huffy.decode(codein, out);

    return 0;
}

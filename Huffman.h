/**
 * @file Huffman.h - Huffman encoder/decoder
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#pragma once
#include <iostream>
#include <fstream>
#include "adt/BitStream.h"
#include "Bits.h"
#include "BinaryNode.h"

/**
 * @class Huffman - Huffman encoder/decoder.
 *
 */
class Huffman {
public:
    /**
     * Construct a Huffman encoder/decoder using the given stream as a sample source.
     *
     * The encoding/decoding table will be built during construction based on the frequencies
     * of characters seen in the sample source.
     * @param sampleSource  this stream will be used to gauge frequency of each character code
     * @post                only characters seen in this stream will be allowed in any text to be
     *                      encoded by this encoder
     */
    explicit Huffman(std::istream &sampleSource);

    // big 5
    ~Huffman();
    Huffman() = delete;
    Huffman(const Huffman& other) = delete;
    Huffman(Huffman&& temp) = delete;
    Huffman& operator=(const Huffman& other) = delete;
    Huffman& operator=(Huffman&& temp) = delete;

    /**
     * Encode the given source text.
     *
     * @param source       the text to be encoded into Huffman codes
     * @param codedOutput  a bit stream to hold the bit sequence of Huffman codes corresponding to
     *                     sequence of characters in the source
     * @throws invalid_argument  if there are characters in the source that were not in the original sample
     *                           used to construct this object
     * @post               codedOutput can be used to reconstruct the original text in source with decode()
     */
    void encode(std::istream& source, BitStream& codedOutput) const;

    /**
     * Decode the given bit stream into the original text.
     *
     * @param codedInput  the encoded bit stream produced from the original text
     * @param out         the original text
     * @pre               codedInput is a bit stream produced by this or equivalent Huffman object
     */
    void decode(BitStream& codedInput, std::ostream& out) const;

    /**
     * Get the Huffman code for a given character (for debugging).
     *
     * @param c  character
     * @return   the bit stream of the Huffman code that has been calculated for this character
     */
    Bits getCode(unsigned char c) const;

    /**
     * Get the frequency of the given character as seen in the sample during construction (for debugging).
     *
     * @param c  character
     * @return   number of observations of this character during construction from the sample
     */
    int getFrequency(unsigned char c) const;

    /**
     * Print out the data for a given character. Do nothing if the character was not in the sample.
     *
     * @param out   printing stream
     * @param c     character
     */
    void printCode(std::ostream& out, int ch) const {
        unsigned char c = to_unsigned(ch);
        int freq = getFrequency(c);
        if (freq > 0) {
            if (c == '\n')
                out << "'\\n'";
            else if (c == '\r')
                out << "'\\r'";
            else if (' ' <= c && c <= '~')
                out << "'" << (char) c << "'";
            else
                out << ch;
            out << "(" << freq << "): " << getCode(c) << std::endl;
        }
    }

    /**
     * A byte from the original text to be encoded might be 0..255 (which includes the
     * printable ASCII characters 32..127 and newlines (10), etc.)
     */
    static const int MAX_CHAR = 255;

private:
    /**
     * We can use CodeTree as short-hand for BinaryNode<unsigned char>
     */
    typedef BinaryNode<unsigned char> CodeTree;

    /**
     * The entries in the Priority Queue which contain a count of of all the characters
     * in the tree and the tree itself.  See the assignment write-up.
     */
    struct PQEntry {
        int frequency;
        CodeTree *codeTree;

        // convience constructors
        PQEntry(int freq, unsigned char c);
        PQEntry(int combinedFrequency, CodeTree *lessFrequent, CodeTree *moreFrequent);

        // sort operator for the Priority Queue to work
        bool operator<(const PQEntry& rhs) const;

        // for debugging it may be nice to be able to print this guy out
        friend std::ostream& operator<<(std::ostream& out, const PQEntry& entry) {
            return out << "PQEntry(" << entry.frequency << "," << entry.codeTree << ")";
        }
    };

    /**
     * bit sequence Huffman "code" for each character
     */
    Bits codes[MAX_CHAR+1];

    /**
     * observation count of each character in sample
     */
    int samplecount[MAX_CHAR+1];

    /**
     * final code tree of the Huffman codes
     *
     * For decoding, we walk down the tree to find the character corresponding to the code.
     * When we hit a leaf we output the character and return the root for the next bits in the
     * coded sequence.
     *
     * For encoding, we take all the leaves and place the corresponding codes in this->codes. Then
     * the encoding process is to just looking up each character in this->codes and outputting the
     * corresponding bit sequence.
     */
    CodeTree *root;

    /**
     * Build the code table from a sample to indicate:
     *     1. the characters to accept in encoding--any characters not in the sample
     *        if encountered during encoding will throw an exception
     *     2. the expected frequency of the characters in the full text of the text streams
     *        to be encoded
     * Called by the constructor. Does four things:
     *     1. calls clear()
     *     2. calls collectFrequencies(sampleSource)
     *     3. calls buildCodeTree()
     *     4. calls populateCodes(root)
     * @param sampleSource characters are counted from this input stream
     */
    void sample(std::istream &sampleSource);

    /**
     * Given a bit stream, pull the next character from it by using this->root.
     *
     * @param code          the bit stream that has the next character's code
     * @param c             the resulting decoded character
     * @param mustUseItAll  if true, we verify that the bit stream is entirely used up
     * @return              true if the bit stream decodes a character, false otherwise
     *                      (if mustUseItAll is true, we also return false if code is not
     *                      empty at the end of pulling out this one character)
     */
    bool translateCode(BitStream &code, unsigned char &c, bool mustUseItAll) const;

    /**
     * Reads through the sampleSource until EOF and fills in this->sampleCount table.
     *
     * @param sampleSource characters are counted from this input stream
     */
    void collectFrequencies(std::istream &sampleSource);

    /**
     * Builds this->root using a temporary PQueueLL<PQEntry>. It starts with a PQEntry for
     * each non-zero frequency character in this->sampleCount. Then it pulls pais of PQEntries
     * from the priority queue, combines them into a single BinaryNode, and enqueues that back
     * into the priority queue. When the queue has only one entry left, that contains the tree
     * of the Huffman codes and is assigned to this->root.
     */
    void buildCodeTree();

    /**
     * Recursive traversal of the code tree, this->root, to find all the codes we generated, and for
     * each (i.e., each leaf of the tree), place its corresponding Huffman code in this->codes.
     *
     * @param node         current node in the tree
     * @param code         bits so far going down the tree
     */
    void populateCodes(CodeTree *node, Bits code);

    /**
     * clear out all of our data structures, i.e., freeNodes(root)
     */
    void clear();

    /**
     * All the i/o stuff deals in (signed) char datatype (range: -128..+127), but we need
     * instead the same bit pattern interpreted as a an unsigned char (range: 0..255) for indexing into
     * our tables and for bit streams.
     *
     * @param c   the signed char we want to interpret as an unsigned char
     * @return    the same exact bits interpreted as an unsigned char
     */
    static unsigned char to_unsigned(char c) {
        return static_cast<unsigned char>(c);
    }
};



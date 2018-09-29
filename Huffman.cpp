//Rajiv Singireddy
//Huffman.cpp
//CS2430-01
/**
 * @file Huffman.cpp -
 * @author Rajiv Singireddy
 * @see "Seattle University, CPSC2430, Spring 2018"
 */


/*
  1) Downloaded book.txt and added it to my data directory. It is called
  Astronomy for Young Australians.
  2)116k - size of book
  3)68k - size of encoded book
  4)Save about 41%
  5) GZIP size - 44k

 */
#include <iostream>
#include "Huffman.h"
#include "BinaryNode.h"
#include "PQueueLL.h"
#include <stdexcept>
using namespace std;

Huffman::Huffman(istream &sampleSource) : root(nullptr){
    for(int i = 0; i <= MAX_CHAR; i++) {
      samplecount[i] = 0;
    }
    for(int j = 0; j <= MAX_CHAR; j++) {
      codes[j] = Bits();
    }
    sample(sampleSource);
}

Huffman::~Huffman() {
    clear();
}
void Huffman::encode(istream& source, BitStream& codedOutput) const {
    int a = source.get();
    while(a != EOF) {
      Bits bits = codes[to_unsigned(a)];
      codedOutput << bits;
      a = source.get();
    }
}

void Huffman::decode(BitStream& codedInput, ostream& out) const {
    while(!codedInput.empty()) {
      unsigned char c;
      if(!translateCode(codedInput,c,false)) {
        if(codedInput.empty()) {
          throw invalid_argument("Bit stream early ending");
        }
        else {
          throw invalid_argument("Code doesn't work");
        }
      }
      out << c;
    }
}

Bits Huffman::getCode(unsigned char c) const {
    return codes[c];
}

int Huffman::getFrequency(unsigned char c) const {
    return samplecount[c];
}

Huffman::PQEntry::PQEntry(int freq, unsigned char c) : frequency(freq), codeTree(new CodeTree(c)) {}

Huffman::PQEntry::PQEntry(int combinedFrequency, CodeTree *lessFrequent, CodeTree *moreFrequent)
  :frequency(combinedFrequency), codeTree(new CodeTree(moreFrequent, lessFrequent, '*')) {}

bool Huffman::PQEntry::operator<(const PQEntry& rhs) const {
    return frequency < rhs.frequency;
}

void Huffman::sample(std::istream &sampleSource) {
    clear();
    collectFrequencies(sampleSource);
    buildCodeTree();
    populateCodes(root, Bits());
}

bool Huffman::translateCode(BitStream &code, unsigned char &c, bool mustUseItAll) const {
    CodeTree *temp = root;
    while(!temp->isLeaf()) {
      if(code.empty())
        return false;
      bool codeDeq = code.dequeue();
      if(codeDeq) {
        temp = temp->right;
      }
      else {
        temp = temp->left;
      }
    }
    c= temp->data;
    if(mustUseItAll) {
      if(!code.empty()) {
        return false;
      }
    }
    return true;
}

void Huffman::collectFrequencies(std::istream &sampleSource) {
    while(!sampleSource.eof()) {
      char c;
      sampleSource.get(c);
      samplecount[(to_unsigned(c))]+=1;
    }
}

void Huffman::buildCodeTree() {
    PQueueLL<PQEntry> pq;
    for(unsigned int i = 0; i <= MAX_CHAR; i++) {
      if(samplecount[i]!=0) {
        pq.enqueue(PQEntry(samplecount[i],i));
      }
    }
    while(!pq.empty()) {
      PQEntry tree = pq.peek();
      pq.dequeue();

      if(pq.empty()) {
        pq.enqueue(tree);
        break;
      }

      PQEntry tree2 = pq.peek();
      pq.dequeue();
      PQEntry newEntry(tree.frequency + tree2.frequency, tree.codeTree, tree2.codeTree);
      pq.enqueue(newEntry);
    }
    root = pq.peek().codeTree;
}

void Huffman::populateCodes(CodeTree *node, Bits code) {
    if(node->isLeaf()) {
      codes[(int)(node->data)] = code;
    }

    else {
      Bits leftC = code;
      Bits rightC = code;

      leftC.enqueue(0);
      rightC.enqueue(1);
      populateCodes(node->left, leftC);
      populateCodes(node->right, rightC);
    }
} 
void Huffman::clear() {
    BinaryNode<unsigned char>::freeNodes(root);
}

//Rajiv Singireddy
/**
 * @file BinaryNode.h -- Binary Node adt that is similar to a Nod except
 *it has two next pointers
 * @author Rajiv Singireddy
 * @see "Seattle University, CPSC2430, Spring 2018"
 */
#pragma once
#include <iostream>

template <typename T>
struct BinaryNode {
  T data;
  BinaryNode *left;
  BinaryNode *right;

  BinaryNode() : data(), left(nullptr), right(nullptr) {}
  explicit BinaryNode(const T& data) : data(data), left(nullptr), right(nullptr) {}
  BinaryNode(BinaryNode *left, BinaryNode *right) : data(), left(left), right(right) {}
  BinaryNode(BinaryNode *left, BinaryNode *right, const T& data) : data(data), left(left), right(right) {}
  BinaryNode(const BinaryNode& other) = default;
  BinaryNode(BinaryNode&& temp) = default;
  ~BinaryNode() = default;
  BinaryNode&operator=(const BinaryNode& other) = default;
  BinaryNode&operator=(BinaryNode&& temp) = default;

  bool isLeaf() const {
    return !(hasRight() || hasLeft());
  }

  bool hasRight() const {
    return right!= nullptr;
  }
  
  bool hasLeft() const {
    return left!= nullptr;
  }

  static void freeNodes(BinaryNode*& node) {
    if(node == nullptr) {
      return;
    }
    freeNodes(node->left);
    freeNodes(node->right);
    delete node;
    node = nullptr;
  }

  static void printR(std::ostream& out, std::string prefix, const BinaryNode *node) {
    if(node == nullptr) {
      return;
    }
    printR(out, " " + prefix + "0", node->left);
    out << prefix << ": " << node->data << std::endl;
    printR(out, " " + prefix + "1", node->right);
  }

  friend std::ostream&operator<<(std::ostream& out, const BinaryNode *node) {
    printR(out, "", node);
    return out;
  }
};

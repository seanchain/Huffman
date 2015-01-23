#ifndef HUFFNODE_H
#define HUFFNODE_H

#include <string>

using namespace std;

/* Huffman tree node abstract base class */
template <typename E>
class HuffNode {
public:
    // Base destructor
    virtual ~HuffNode() {}
    // Return frequency
    virtual int weight() = 0;
    // Determine type
    virtual bool isLeaf() = 0;
};

/* Leaf node subclass */
template <typename E>
class LeafNode : public HuffNode<E> {
private:
    // Value
    E it;
    // Weight
    int wgt;
    // Code
    string encode;
public:
    // Constructor
    LeafNode(const E& val, int freq) {
        it = val;
        wgt = freq;
    }
    // Return the weight
    int weight() {
        return wgt;
    }
    // Return the value
    E val() {
        return it;
    }
    // Judge whether it is leaf
    bool isLeaf() {
        return true;
    }

    void setWeight(int weight) {
        wgt = weight;
    }

    void setCode(string code) {
        encode = code;
    }

    string getCode() {
        return encode;
    }
};

/* Internal node subclass */
template <typename E>
class IntlNode : public HuffNode<E> {
private:
    // Left child
    HuffNode<E>* lc;
    // Right child
    HuffNode<E>* rc;
    // Subtree weight
    int wgt;
public:
    // Constructor
    IntlNode(HuffNode<E>* l, HuffNode<E>* r) {
        wgt = l->weight() + r->weight();
        lc = l;
        rc = r;
    }
    // Return the weight
    int weight() {
        return wgt;
    }
    // Judge whether it is the leaf
    bool isLeaf() {
        return false;
    }
    // Return the left child
    HuffNode<E>* left() const {
        return lc;
    }
    // Set the left child
    void setLeft(HuffNode<E>* b) {
        lc = (HuffNode<E>*)b;
    }
    // Return the right child
    HuffNode<E>* right() const {
        return rc;
    }
    // Set the right child
    void setRight(HuffNode<E>* b) {
        rc = (HuffNode<E>*)b;
    }
};

#endif // HUFFNODE_H

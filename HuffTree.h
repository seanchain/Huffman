#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "HuffNode.h"
#include "Heap.h"

template <typename E>
class HuffTree {
private:
    // Tree root
    HuffNode<E>* Root;

public:
    // Leaf constructor
    HuffTree(E& val, int freq) {
        Root = new LeafNode<E>(val, freq);
    }

    // Internal node constructor
    HuffTree(HuffTree<E>* l, HuffTree<E>* r) {
        Root = new IntlNode<E>(l->root(), r->root());
    }

    // Destructor
    ~HuffTree() {}

    // Get root
    HuffNode<E>* root() {
        return Root;
    }

    // Root weight
    int weight() {
        return Root->weight();
    }


};

// Build a Huffman tree from a collection of frequencies
template <typename E>
HuffTree<E>* buildHuff(HuffTree<E>** TreeArray, int count) {
    Heap<HuffTree<E>*>* forest = new Heap<HuffTree<E>*>(TreeArray, count, count);
    HuffTree<char> *temp1, *temp2, *temp3 = NULL;

    while (forest->size() > 1) {
        temp1 = forest->removefirst(); // Pull first two trees
        temp2 = forest->removefirst(); // off the list
        temp3 = new HuffTree<E>(temp1, temp2);
        forest->insert(temp3); // Put the new tree back on list
        delete temp1; // Must delete the remnants
        delete temp2; // of the trees we created
    }

    return temp3;
}

#endif  // HUFFTREE_H

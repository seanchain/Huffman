#ifndef FILEENCODE_H
#define FILEENCODE_H

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <bitset>

#include "HuffTree.h"

using namespace std;

template <typename E>
class FileEncode {
private:
    ifstream encodedFile;
    ofstream compressFile;
    ofstream dic;

    char ch;
    string fileHuffCode;
    vector<string> codeVector;
    list< HuffTree<char>* > charList;
    HuffTree<char>** TreeArray;
    HuffTree<char>* Tree;

    void charCount(char ch) {
        list< HuffTree<char>* >::iterator charIterator = charList.begin();
        for (; charIterator != charList.end(); charIterator++) {
            LeafNode<E>* leaf = (LeafNode<E>*)(*charIterator)->root();
            if (leaf->val() == ch) {
                leaf->setWeight(leaf->weight()+1);
                return;
            }

            continue;
        }

        charList.push_back(new HuffTree<E>(ch, 1));
        return;
    }
public:
    FileEncode() {

    }
    FileEncode(string path) {
        if (!openFile(path)) {
            return;
        }
        buildForest();
        Array(charList);
        Tree = buildHuff(TreeArray, getSize());
        setHuffCode(Tree->root());
        Encode(Tree->root());
        fileWrite();
    }

    ~FileEncode() {
        closeFile();
    }

    bool openFile(string path) {
        //??1.dat???path,????????1.comp,???????1.dic
        encodedFile.open(path.c_str());
        if (!encodedFile) {
            cout << "File Error!" << endl;
            return false;
        }
        string substring = path.substr(0, path.size() - 4);
        cout << substring << endl;
        string compressedfile = substring + ".comp";
        string dicfile = substring + ".dic";
        compressFile.open(compressedfile);
        if (!compressFile) {
            //cout << "File Error!" << endl;
            return false;
        }

        dic.open(dicfile);
        if (!dic) {
            //cout << "File Error!" << endl;
            return false;
        }
        dic << 0 << ' ';

        return true;
    }

    bool closeFile() {
        encodedFile.close();
        compressFile.close();
        dic.close();

        return true;
    }

    bool buildForest() {
        if (!encodedFile) {
            return false;
        }

        while (encodedFile.get(ch)) {
            charCount(ch);
        }
        list< HuffTree<char>* >::iterator charIterator = charList.begin();
        for (; charIterator != charList.end(); charIterator++) {
            LeafNode<E>* leaf = (LeafNode<E>*)(*charIterator)->root();
            if (leaf->val() == ' ') {
                dic << '\1' << ' ';
            } else if (leaf->val() == '\t') {
                dic << '\2' << ' ';
            } else if (leaf->val() == '\n') {
                dic << '\3' << ' ';
            } else {
                dic << leaf->val() << ' ';
            }
            dic << leaf->weight() << ' ';
        }

        return true;
    }

    void Array(list< HuffTree<char>* > charList) {
        TreeArray = new HuffTree<E>*[charList.size()];
        list< HuffTree<char>* >::iterator charIterator = charList.begin();
        for (int i=0; charIterator != charList.end(); charIterator++, i++) {
            TreeArray[i] = *charIterator;
        }
    }

    HuffTree<char>** getTreeArray(list< HuffTree<char>* > charList) {
        Array(charList);
        return TreeArray;
    }

    void setHuffCode(HuffNode<E>* root) {
        if (!root->isLeaf()) {
            IntlNode<E>* node = (IntlNode<E>*)root;
            codeVector.push_back("0");
            setHuffCode(node->left());
            codeVector.pop_back();
            codeVector.push_back("1");
            setHuffCode(node->right());
            codeVector.pop_back();
        } else {
            LeafNode<E>* leaf = (LeafNode<E>*)root;

            string HuffCode;
            for (int i=0; i<codeVector.size(); i++) {
                HuffCode += codeVector[i];
            }
            leaf->setCode(HuffCode);
        }
    }

    void findHuffCode(HuffNode<E>* root, char ch) {
        if (!root->isLeaf()) {
            IntlNode<E>* node = (IntlNode<E>*)root;
            findHuffCode(node->left(), ch);
            findHuffCode(node->right(), ch);
        } else {
            LeafNode<E>* leaf = (LeafNode<E>*)root;
            if (leaf->val() == ch) {
                fileHuffCode += leaf->getCode();
            }
        }
    }

    void Encode(HuffNode<E>* root) {
        encodedFile.clear();
        encodedFile.seekg(0, ios::beg);

        while (encodedFile.get(ch)) {
            findHuffCode(root, ch);
        }
    }

    int getSize() {
        return charList.size();
    }

    void fileWrite() {
        dic.clear();
        dic.seekp(0, ios::beg);
        dic << fileHuffCode.size()%8;
        for (int i=0; i<fileHuffCode.size(); i+=8) {
            string substring;
            if (i+8 <= fileHuffCode.size()) {
                substring = fileHuffCode.substr(i, 8);
            } else {
                substring = fileHuffCode.substr(i, fileHuffCode.size()-i);
            }
            bitset<8> binary(substring);
            unsigned long temp = binary.to_ulong();
            cout << (char*)&temp;
            compressFile.write((char *)&temp, 1);
        }
    }
};

#endif // FILEENCODE_H

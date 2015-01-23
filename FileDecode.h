#ifndef FILEDECODE_H
#define FILEDECODE_H

#include "HuffTree.h"
#include "FileEncode.h"

using namespace std;

template <typename E>
class FileDecode {
private:
    ifstream dicin;
    ifstream enin;
    ofstream deout;

    list< HuffTree<char>* > charList;
    FileEncode<E> recode;
    char findchar = '\0';
public:
    FileDecode(string path) {
        openFile(path);
        Decode();
    }

    ~FileDecode() {
        closeFile();
    }

    bool openFile(string path) {
        string dicfile = path + ".dic"; //?????????
        string decompressfile = path + ".comp";//????????
        enin.open(decompressfile, ios::binary);
        dicin.open(dicfile);
        string decomp = path + "decomp.dat";
        deout.open(decomp);

        return true;
    }

    bool closeFile() {
        dicin.close();
        enin.close();
        deout.close();

        return true;
    }

    void Decode() {
        char temp;
        int tempint;
        int leave;
        dicin >> leave;
        while (1) {
            if (!(dicin >> temp)) {
                break;
            }
            switch (temp) {
            case '\1':
                temp = ' ';break;
            case '\2':
                temp = '\t';break;
            case '\3':
                temp = '\n';break;
            }
            if (!(dicin >> tempint)) {
                break;
            }
            charList.push_back(new HuffTree<E>(temp, tempint));
        }

        HuffTree<char>* IntlTree = buildHuff(recode.getTreeArray(charList), charList.size());
        recode.setHuffCode(IntlTree->root());

        string de;
        string last;

        while (!enin.eof()) {
            enin.read(&temp, 1);
            bitset<8> b((unsigned long)temp);
            de += b.to_string();
            last = b.to_string();
        }

        de.replace(de.size()-16, 16, last.substr(8-leave, leave));
        string deoutstr;
        char cha;
        for (int i=0, j=1; i<de.size();) {
            if ((cha = charFind(IntlTree->root(), de.substr(i,j))) == '\0') {
                j++;
            } else {
                deoutstr += cha;
                findchar = '\0';
                i += j;
                j = 0;
            }
        }
        
        cout << deoutstr;
        deout << deoutstr;
    }

    char charFind(HuffNode<E>* root, string str) {
        if (!root->isLeaf()) {
            IntlNode<E>* node = (IntlNode<E>*)root;
            charFind(node->left(), str);
            charFind(node->right(), str);
        } else {
            LeafNode<E>* leaf = (LeafNode<E>*)root;
            if (leaf->getCode() == str) {
                findchar = leaf->val();
            }
        }

        return findchar;
    }
};
#endif // FILEDECODE_H

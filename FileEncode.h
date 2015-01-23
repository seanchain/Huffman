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
class FileEncode {    //FileEncode class.
private:
    ifstream ori_file;    //Original file to get the original content of the file.
    ofstream com_file;    //Output compressed file.
    ofstream dic;    //Dictionary generated.
    char ch;
    string fileHuffCode;
    vector<string> codeVec;
    list< HuffTree<char>* > charList;
    HuffTree<char>** TreeArray;
    HuffTree<char>* Tree;    //Some declaration of the variables.

    void charCount(char ch) { //Count the times appeared of the characters.
        list< HuffTree<char>* >::iterator charIt = charList.begin(); //new a list object to start iteration.
        for (; charIt != charList.end(); charIt++) {
            LeafNode<E>* leaf = (LeafNode<E>*)(*charIt)->root();
            if (leaf->val() == ch) {
                leaf->setWeight(leaf->weight()+1);
                return;
            }
            continue;
        }
        /*
        The Code Above: 
        If there has never appeared this character, it create a new charater and set the count of 
        this character, otherwise, the count of the existed character plus 1.
        */ 
        charList.push_back(new HuffTree<E>(ch, 1)); //add the character and its appeared times to create a new Huffman tree and add the tree to the list charList.
        return;
    }
public:
    FileEncode() {} //Default constructor, but not going to use here.
    FileEncode(string path) {
        if (!openFile(path)) {
            return;    //Cannot find the input path of the file, the program ends.
        }
        buildForest(); //Call the method buildForest().
        Array(charList); //Call the method Array() with a parameter of a list object.
        Tree = buildHuff(TreeArray, getSize());//Create the Huffman tree.
        setHuffCode(Tree->root());//Use the Huffman Tree to get the corresponding characters.
        Encode(Tree->root()); //Use the Huffman Tree to get the compressed string of the content of the input file.
        fileWrite(); //Write the compressed string to a new file.
    }

    ~FileEncode() {
        closeFile();
    } //Destructor, close the handle of every files.

    bool openFile(string path) {  //method to open a file.
        ori_file.open(path.c_str()); //Open a file handle.
        if (!ori_file) {
            cout << "File Error!" << endl;
            return false;
        } //If the file at the given path parameter does not exist, retur a false.
        string substring = path.substr(0, path.size() - 4); //Get the substring of the input string based on the value delivered by the php file.
        cout << substring << endl;  //check the string if it is correct.
        string compressedfile = substring + ".comp";  //output file name.
        string dicfile = substring + ".dic"; //dictionary of the character and the 0,1 string.
        com_file.open(compressedfile); //open compressed file.
        if (!com_file) {
            return false; //file not found, return false.
        }

        dic.open(dicfile); //open dictionary file.
        if (!dic) {
            return false; //file not found, return false.
        }
        dic << 0 << ' '; 

        return true;
    }

    bool closeFile() { //Method to close all the file handles.
        ori_file.close();
        com_file.close();
        dic.close();

        return true;
    }

    bool buildForest() {    //Method to build the forest of the Huffman code.
        if (!ori_file) {
            return false; //check if the original file exists.
        }

        while (ori_file.get(ch)) {
            charCount(ch);  //get the characters of the original file and count the times of appearing of these characters.
        }
        list< HuffTree<char>* >::iterator charIt = charList.begin();  
        for (; charIt != charList.end(); charIt++) {
            LeafNode<E>* leaf = (LeafNode<E>*)(*charIt)->root();
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
        
        /*
        Rethinking of the characters, because characters like '\n', '\t' and ' ' are hard to save into a dictionary, so we replace them with characters like '\1' and '\2' and '\3'
        */

        return true;
    }

    void Array(list< HuffTree<char>* > charList) { //Get the array of each character' own Huffman tree.
        TreeArray = new HuffTree<E>*[charList.size()];
        list< HuffTree<char>* >::iterator charIt = charList.begin();
        for (int i=0; charIt != charList.end(); charIt++, i++) {
            TreeArray[i] = *charIt;
        }
    }

    void setHuffCode(HuffNode<E>* root) { //set up the Huffman Tree of all the characters.
        if (!root->isLeaf()) {
            IntlNode<E>* node = (IntlNode<E>*)root;
            codeVec.push_back("0");
            setHuffCode(node->left());
            codeVec.pop_back();
            codeVec.push_back("1");
            setHuffCode(node->right());
            codeVec.pop_back(); //if the node is an internal node, so add the '0' and '1' character to a vector.
        } else {
            LeafNode<E>* leaf = (LeafNode<E>*)root; //it is a leaf node.

            string HuffCode;
            for (int i=0; i<codeVec.size(); i++) {
                HuffCode += codeVec[i];
            }
            leaf->setCode(HuffCode); //Gather all the '0' and '1' character to a string to represent the huffman code of the character and stored in the character object itself.
        }
    }

    void findHuffCode(HuffNode<E>* root, char ch) { //method to find each characters' Huffman code.
        if (!root->isLeaf()) {
            IntlNode<E>* node = (IntlNode<E>*)root;
            findHuffCode(node->left(), ch);
            findHuffCode(node->right(), ch); //leaf node, use recursion to find the character's Huffman code.
        } else {
            LeafNode<E>* leaf = (LeafNode<E>*)root;
            if (leaf->val() == ch) {
                fileHuffCode += leaf->getCode(); //get the whole original file's Huffman Code.
            }
        }
    }

    void Encode(HuffNode<E>* root) {
        ori_file.clear();
        ori_file.seekg(0, ios::beg); //start the file reading at the beginning of the file.

        while (ori_file.get(ch)) {
            findHuffCode(root, ch);  //Call method findHuffCode to get the file's all characters' corresponding compressed string.
        }
    }

    int getSize() { //Get the count of the character's Huffman tree before putting them together.
        return charList.size();
    }

    void fileWrite() { //Method to write the compressed string to a new file.
        dic.clear();
        dic.seekp(0, ios::beg);   
        //the Huffman code with '0' and '1' string, 
        dic << fileHuffCode.size()%8; //write the file's number % 8 to the dic file to make sure how many left characters.
        for (int i=0; i<fileHuffCode.size(); i+=8) {
            string substring; 
            if (i+8 <= fileHuffCode.size()) {
                substring = fileHuffCode.substr(i, 8); //every 8 character makes up a new string.
            } else {
                substring = fileHuffCode.substr(i, fileHuffCode.size()-i); //left string with less than 8 characters.
            }
            bitset<8> binary(substring); //Use the bitset class to transform the '0' and '1' string to binary string.
            unsigned long temp = binary.to_ulong();
            cout << (char*)&temp; //Output the value for the php server to reply to the iOS client.
            com_file.write((char *)&temp, 1); //write the compressed file with compressed string using binary transformation to the file on the server.
        }
    }
};

#endif // FILEENCODE_H

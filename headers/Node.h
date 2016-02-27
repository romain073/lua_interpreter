#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Node {
public:
    string tag, value;
    list<Node> children;

    Node(string t, string v) : tag(t), value(v) {
    }
    
    Node(string t) : tag(t), value("") {
    }

    Node() {
        tag = "uninitialised";
        value = "uninitialised";
    } // Bison needs this.

    void dump(int depth = 0) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tag << ":" << value << endl;
        list<Node>::iterator i;
        for (i = children.begin(); i != children.end(); i++)
            (*i).dump(depth + 1);
    }
    
    void push_back(Node n1){
        this->children.push_back(n1);
    }
    void push_back(Node n1, Node n2){
        this->children.push_back(n1);
        this->children.push_back(n2);
    }
    bool isUndefined(){
        return this->tag == "uninitialised" && this->value == "uninitialised";
    }

    int dumpToFile(ofstream &f, int &id) {
        int myID = id++;
        f << "id" << myID << " [label=\"" << tag;
        if(value!="")
            f << " -> " << value;
        f << "\"];" << endl;
        list<Node>::iterator i;
        for (i = children.begin(); i != children.end(); i++) {
            int childID = (*i).dumpToFile(f, id);
            f << "id" << myID << " -> " << "id" << childID << ";" << endl;
        }
        return myID;
    }

};

#endif  /* NODE_H */
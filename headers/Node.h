#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "Environment.h"
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
        tag = "null";
        value = "null";
    }

    void dump(int depth = 0) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tag << ":" << value << endl;
        list<Node>::iterator i;
        for (i = children.begin(); i != children.end(); i++)
            (*i).dump(depth + 1);
    }

    Node operator+(Node n)
    {
        this->children.push_back(n);
        return *this;
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
    
    int execute(Environment* e){
        //cout << "executing "<< this->tag << endl;
        /*if(this->tag == "root"){
            for(auto i : children){
                i.execute(e);
            }
        } */
        
        if (this->tag == "Block") {
            children.front().execute(e);
            children.back().execute(e);
        } else if (this->tag == "pass") {
            
        } else if (this->tag == "Statements") {
            for(auto i : children){
                i.execute(e);
            }
        }else if (this->tag == "args") {
            int ret = -1;
            for(auto i : children){
                ret = i.execute(e);
            }
            return ret;
        } else if (this->tag == "affectation") {
            e->add(children.front().children.front().value, children.back().children.front().execute(e));
        } else if (this->tag == "exp") {
            
            return children.front().execute(e);
        } else if (this->tag == "Explist") {
            return children.front().execute(e);
        } else if (this->tag == "+") {
            return children.front().execute(e)+children.back().execute(e);
        } else if (this->tag == "*") {
            return children.front().execute(e)*children.back().execute(e);
        } else if (this->tag == "/") {
            return children.front().execute(e)/children.back().execute(e);
        } else if (this->tag == "-") {
            return children.front().execute(e)-children.back().execute(e);
        } else if (this->tag == "number") {
            return stoi(this->value);
        } else if (this->tag == "Var") {
            return e->get(this->value);
        } else if (this->tag == "functioncall") {
            if(children.front().value == "print"){
                cout << "   Print => " << children.back().execute(e) << endl;
            }else{
                cout << "unknown function"<<endl;
                return -1;
            }
        } else {
            cout << "Not implemented - " << this->tag << endl;
        }
            
        return 1;
    }

};

#endif  /* NODE_H */
#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "Value.h"
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
    
    Value execute(Environment* e){
        /*
        for(auto i : children){
        i.execute(e);
        }
        */
        
        if (this->tag == "Block") {
            children.front().execute(e);
            children.back().execute(e);
        } else if (this->tag == "pass") {
            
        } else if (this->tag == "Statements") {
            for(auto i : children){
                i.execute(e);
            }
        }else if (this->tag == "args") {
            Value ret;
            for(auto i : children){
                ret = i.execute(e);
            }
            return ret;
        } else if (this->tag == "affectation") {
            Value varlist = children.front().execute(e);
            Value explist = children.back().execute(e);
            // todo assert size equals
            for(int i = 0;i<varlist.list_val.size();i++){
                e->add(varlist.list_val[i].string_val, explist.list_val[i]);
            }
        } else if (this->tag == "Explist") {
            vector<Value> list;
            for(auto i : children){
                list.push_back(i.execute(e));
            }
            return Value(list);
        } else if (this->tag == "varlist") {
            vector<Value> list;
            for(auto i : children){
                list.push_back(i.value);
            }
            return Value(list);
        } else if (this->tag == "+") {
            return children.front().execute(e)+children.back().execute(e);
        } else if (this->tag == "*") {
            return children.front().execute(e)*children.back().execute(e);
        } else if (this->tag == "/") {
            return children.front().execute(e)/children.back().execute(e);
        } else if (this->tag == "-") {
            return children.front().execute(e)-children.back().execute(e);
        } else if (this->tag == "number") {
            return Value(stoi(this->value));
        } else if (this->tag == "string") {
            return Value(this->value);
        } else if (this->tag == "Var") {
            return e->get(this->value);
        } else if (this->tag == "functioncall") {
            if(children.front().value == "print"){
                Value args = children.back().execute(e);
                cout << "   Print => " << args.list_val[0] << " "<< args.list_val[0].int_val << " " << args.list_val[0].string_val << endl;
            }else{
                cout << "unknown function"<<endl;
                return -1;
            }
        } else {
            cout << "Not implemented - " << this->tag << endl;
        }
            
        return Value();
    }

};

#endif  /* NODE_H */
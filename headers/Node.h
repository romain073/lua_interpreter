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
        for(auto i : children)
            i.dump(depth + 1);
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
        for(auto i : children) {
            int childID = i.dumpToFile(f, id);
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
        } else if (this->tag == "forequal") {
            int counter = 0;
            string var;
            Value from;
            Value to;
            Value step;
            for(auto i : children){
                switch(counter){
                    case 0:
                        var = i.value;
                        break;
                    case 1:
                        from = i.execute(e);
                        break;
                    case 2:
                        to = i.execute(e);
                        break;
                    case 3:
                        if(i.tag == "pass"){
                            if(from > to){
                                step = Value(-1);
                            }else{
                                step = Value(1);
                            }
                        }else{
                            step = i.execute(e);
                        }
                        break;
                    
                }
                
                counter++;
            }
            for(int i = from.int_val;i<=to.int_val;i+=step.int_val){
                e->add(var, Value(i));
                children.back().execute(e);
            }
        } else if (this->tag == "if") {
            if(children.front().execute(e).isTrue()){
                list<Node>::iterator it = children.begin();
                advance(it, 1);
                it->execute(e);
            }
            return Value(1);
        } else if (this->tag == "args") {
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
        } else if (this->tag == "%") {
            return children.front().execute(e)%children.back().execute(e);
        } else if (this->tag == "==") {
            return children.front().execute(e)==children.back().execute(e);
        } else if (this->tag == "number") {
            
            
            if(this->value.find("0x") == 0){
                return Value(stoi(this->value.erase(0,2), 0,16)); //hex
            }else {
                int expindex = this->value.find("e");
                if(expindex == string::npos){
                    expindex = this->value.find("E");
                }
                if(expindex == string::npos){
                    return Value::FromNumber(this->value);
                }
                int exponent = stoi(this->value.substr(expindex+1));
                
                Value v = Value::FromNumber(this->value.substr(0,expindex));
                if(exponent == 0){
                    return v;
                }
                
                int mult = 1;
                for(int i = 0; i<abs(exponent);i++){
                    mult*=10;
                }
                
                Value m = Value(mult);
                if(exponent<0)
                    return v/m;
                else
                    return v*m;
                
                
            }
        } else if (this->tag == "string") {
            return Value(this->value);
        } else if (this->tag == "str_arg") {
            vector<Value> v;
            v.push_back(children.front().execute(e));
            return Value(v);
        } else if (this->tag == "Var") {
            return e->get(this->value);
        } else if (this->tag == "functioncall") {
            Value args = children.back().execute(e);
            if(children.front().tag == "propretrieve"){
                string name;
                for(auto i : children.front().children){
                    name+=i.value;
                }   
                if(name == "ioread"){
                    string param = args.list_val.front().string_val;
                    if(param == "*number"){
                        string input;
                        cin >> input;
                        return Value::FromNumber(input);
                    } else {
                        cout << "invalid param for io.read";
                        throw;
                    }
                    
                } else if(name == "iowrite"){
                    cout << args.list_val.front().print();
                } else {
                    cout << "unknown function call " << name;
                    throw;
                }
            } else if(children.front().value == "print"){
                for(int i=0;i<args.list_val.size();i++){
                    cout << args.list_val[i].print();
                    if(i != args.list_val.size()-1){
                        cout << "\t";
                    }
                }
                cout << endl;
            }else{
                cout << "unknown function " << children.front().value<<endl;
                throw;
            }
        } else {
            cout << "Not implemented - " << this->tag << endl;
        }
            
        return Value();
    }

};

#endif  /* NODE_H */
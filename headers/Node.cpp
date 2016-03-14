#ifndef NODE_H
#define NODE_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "Value.cpp"
#include "Environment.cpp"
using namespace std;



class Node {
public:
    string tag, value;
    vector<Node*> children;

    Node(string t, string v) : tag(t), value(unescape(v)) {
    }
    
    Node(string t) : tag(t), value("") {
    }

    Node() {
        tag = "null";
        value = "null";
    }
    
    ~Node() {
        for(auto i : children)
            delete i;
    }
    
    string unescape(string s){
        size_t idx = s.find("\\");
        while( idx != string::npos ){
            if( idx + 1 < s.size() ){
                switch(s[idx + 1]){
                    case 'n':
                        s.replace( idx, 2, "\n");
                        break;
                    case 't':
                        s.replace( idx, 2, "\t");
                        break;
                    case '\\':
                        s.replace( idx, 2, "\\");
                        break;
                    case '\'':
                        s.replace( idx, 2, "'");
                        break;
                    case '"':
                        s.replace( idx, 2, "\"");
                        break;
                    default:
                        cout << "unexpected escape sequence"<<endl;
                        exit(1);
                }
            }
            idx = s.find("\\", idx + 1);
        }
        return s;
    }
    
    string escape(string s){
        size_t idx = s.find('"');
        while( idx != string::npos ){
            if(idx == 0 || s[idx-1] !='\\'){
                s.replace(idx, 1, "\\\"");
            }
            idx = s.find('"', idx + 1);
        }
        
        return s;
    }

    void dump(int depth = 0) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tag << ":" << value << endl;
        for(auto i : children)
            i->dump(depth + 1);
    }

    Node* add(Node* n)
    {
        this->children.push_back(n);
        return this;
    }

    int dumpToFile(ofstream &f, int &id) {
        int myID = id++;
        f << "id" << myID << " [label=\"" << tag;
        if(value!="")
            f << " -> " << escape(value);
        f << "\"];" << endl;
        for(auto i : children) {
            int childID = i->dumpToFile(f, id);
            f << "id" << myID << " -> " << "id" << childID << ";" << endl;
        }
        return myID;
    }
    
    Value execute(Environment* e){
        if (this->tag == "Block") {
            Value v = children.front()->execute(e);
            if(v.isReturn() || v.isBreak()){
                return v; // propagate
            }
            return children.back()->execute(e);
        } else if (this->tag == "tableconstructor") {
            return children.front()->execute(e);
        } else if (this->tag == "fieldlist") {
            Value v = Value::ARRAY();
            for(auto i : children){
                if(i->tag == "simple"){
                    v.array_val.push_back(i->children.front()->execute(e).int_val);
                }
            }
            return v;
        } else if (this->tag == "pass") {
            
        } else if (this->tag == "and") {
            return Value(children.front()->execute(e).isTrue() && children.back()->execute(e).isTrue());
        } else if (this->tag == "or") {
            return Value(children.front()->execute(e).isTrue() || children.back()->execute(e).isTrue());
        } else if (this->tag == "return") {
            Value ret = children.front()->execute(e);
            
            if(ret.isSingleElementList()){
                return ret.list_val[0].setReturn();
            }
            return ret.setReturn();
        } else if (this->tag == "break") {
            return Value::BREAK();
        } else if (this->tag == "value") {
            if(this->value == "true"){
                return Value(true);
            } else if(this->value == "false"){
                return Value(false);
            } else if(this->value == "nil"){
                return Value::NIL();
            } else {
                cout << "Unsupported value " << this->value;
                exit(1);
            }
        } else if (this->tag == "Statements") {
            for(auto i : children){
                Value v = i->execute(e);
                if(v.isReturn() || v.isBreak()){
                    return v;// propagate
                }
            }
        } else if (this->tag == "repeat") {
            do{
                Value v = children.front()->execute(e);
                if(v.isBreak()){
                    return Value();
                }
                if(v.isReturn()){
                    return v;
                }
            }while(!children.back()->execute(e).isTrue());
        } else if (this->tag == "forequal") {

            string var = children[0]->value;
            Value from = children[1]->execute(e);
            Value to = children[2]->execute(e);
            Value step(1);
            if(children[3]->tag != "pass"){
                step = children[3]->execute(e);
            }

            for(int i = from.int_val;i<=to.int_val;i+=step.int_val){
                e->add(var, Value(i));
                Value v = children.back()->execute(e);
                if(v.isBreak()){
                    return Value();
                }
                if(v.isReturn()){
                    return v;
                }
            }
        } else if (this->tag == "if") {
            if(children.front()->execute(e).isTrue()){
                return children[1]->execute(e);
            }
            Node* elseif = children[2];
            for(unsigned i = 0;i<elseif->children.size();i+=2){
                if(elseif->children[i]->execute(e).isTrue()){
                    return elseif->children[i+1]->execute(e);
                }
            }
            return children[3]->children.front()->execute(e);
        } else if (this->tag == "args") {
            Value ret;
            for(auto i : children){
                ret = i->execute(e);
            }
            return ret;
        } else if (this->tag == "affectation") {
            vector<pair<string, int>> list;
            for(auto i : children.front()->children){
                if(i->tag == "Var"){
                    list.push_back(pair<string, int>(i->value,-1));
                }
                else if(i->tag == "tableretrieve"){
                    string var = i->children.front()->value;
                    int index = i->children.back()->execute(e).int_val;
                    list.push_back(pair<string, int>(var,index));
                    
                }
            }

            Value explist = children.back()->execute(e);
            int i = 0;
            // todo assert size equals
            for(auto var : list){
                if(var.second == -1){
                    e->add(var.first, explist.list_val[i]);
                } else {
                    Value v = e->get(var.first);
                    v.array_val[var.second-1] = explist.list_val[i].int_val; // TODO check type
                    e->add(var.first, v);
                }
                i++;
            }
        } else if (this->tag == "Explist") {
            vector<Value> list;
            for(auto i : children){
                list.push_back(i->execute(e));
            }
            return Value(list);
        } else if (this->tag == "+") {
            return children.front()->execute(e)+children.back()->execute(e);
        } else if (this->tag == "#") {
            Value v = children.front()->execute(e);
            if(!v.isArray()){
                cout << "Can't use operator # on non-array";
                exit(1);
            }
            return Value(v.arrayLength());
        }else if (this->tag == "tableretrieve") {
            Value array = children.front()->execute(e);
            Value index = children.back()->execute(e);
            return Value(array.array_val[index.int_val-1]);// TODO Check types
            
        } else if (this->tag == "*") {
            return children.front()->execute(e)*children.back()->execute(e);
        } else if (this->tag == "/") {
            return children.front()->execute(e)/children.back()->execute(e);
        } else if (this->tag == "-") {
            if(children.size()==1){
                return children.front()->execute(e).negate();
            }
            return children.front()->execute(e)-children.back()->execute(e);
        } else if (this->tag == "%") {
            return children.front()->execute(e)%children.back()->execute(e);
        } else if (this->tag == "==") {
            return children.front()->execute(e)==children.back()->execute(e);
        } else if (this->tag == "~=") {
            return children.front()->execute(e)!=children.back()->execute(e);
        } else if (this->tag == "<") {
            return children.front()->execute(e)<children.back()->execute(e);
        } else if (this->tag == ">") {
            return children.front()->execute(e)>children.back()->execute(e);
        } else if (this->tag == "number") {
            
            
            if(this->value.find("0x") == 0){
                return Value(stoi(this->value.erase(0,2), 0,16)); //hex
            }else {
                size_t expindex = this->value.find("e");
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
            v.push_back(children.front()->execute(e));
            return Value(v);
        } else if (this->tag == "Var") {
            return e->get(this->value);
        } else if (this->tag == "parlist") {
            return children.front()->execute(e);
        } else if (this->tag == "namelist") {
            vector<Value> names;
            for(auto i : children){
                names.push_back(i->value);
            }
            return Value(names);
        } else if (this->tag == "funcname") {
            return Value(children.front()->value); // todo - implement colon syntax
        } else if (this->tag == "functiondef") {
            string name = children.front()->execute(e).string_val;
            e->add(name, Value(children.back()));
        } else if (this->tag == "functioncall") {
            Value args = children.back()->execute(e);
            if(children.front()->tag == "propretrieve"){
                string name;
                for(auto i : children.front()->children){
                    name+=i->value;
                }   
                if(name == "ioread"){
                    string param = args.list_val.front().string_val;
                    if(param == "*number"){
                        string input;
                        cin >> input;
                        return Value::FromNumber(input);
                    } else {
                        cout << "invalid param for io.read";
                        exit(1);
                    }
                    
                } else if(name == "iowrite"){
                    for(auto i : args.list_val){
                        cout << i.print();
                    }
                } else {
                    cout << "unknown function call " << name;
                    exit(1);
                }
            } else if(children.front()->value == "print"){
                for(unsigned i=0;i<args.list_val.size();i++){
                    cout << args.list_val[i].print();
                    if(i != args.list_val.size()-1){
                        cout << "\t";
                    }
                }
                cout << endl;
            }else{
                string name = children.front()->value;
                Value retrieved = e->get(name);
                if(!retrieved.isFunction()) {
                    cout << "unknown function " << name <<endl;
                    exit(1);
                }
                Node* fn = retrieved.function_val;
                Value names = fn->children.front()->execute(e);
                Environment e_child = e->clone();
                if(args.list_val.size() != names.list_val.size()){
                    cout << "args" << args << " - names " << names << endl;
                    cout << "Params number does not match...";
                    exit(1);
                }
                for(unsigned i = 0; i<args.list_val.size();i++){
                    e_child.add(names.list_val[i].string_val, args.list_val[i]);
                }
                Value v = fn->children.back()->execute(&e_child);
                if(v.isReturn()){
                    return v.setReturn(false);
                }
            }
        } else {
            cout << "Not implemented - " << this->tag << endl;
            exit(1);
        }
            
        return Value();
    }

};

#endif  /* NODE_H */
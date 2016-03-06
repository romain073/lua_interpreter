#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <iostream>
#include <map>
#include "Value.h"
using namespace std;



class Environment {
public:
    map<string, Value> name_space;
    
    Environment() {
    }
    
    void add(string s, Value value){
        //cout << "add" << s << " => " << value << endl;
        name_space[s] = value;
    }
    Value get(string s){
        return name_space[s];
    }
};

#endif  /* ENVIRONMENT_H */
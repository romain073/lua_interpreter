#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <iostream>
#include <map>
#include "Value.cpp"
using namespace std;



class Environment {
public:
    map<string, Value> name_space;
    
    Environment() {
    }
    
    void add(string s, Value value){
        name_space[s] = value;
    }
    Value get(string s){
        return name_space[s];
    }
    
    Environment clone(){
        Environment e;
        e.name_space = name_space;
        return e;
    }
};

#endif  /* ENVIRONMENT_H */
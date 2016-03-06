#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <iostream>
#include <map>
using namespace std;



class Environment {
public:
    map<string, int> name_space;
    
    Environment() {
    }
    
    void add(string s, int value){
        name_space[s] = value;
    }
    int get(string s){
        return name_space[s];
    }

    void dump() {
        std::cout << "OK!" << std::endl;
    }
};

#endif  /* ENVIRONMENT_H */
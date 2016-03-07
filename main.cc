#include<iostream>
#include "interpreter.tab.hh"
#include "headers/Node.h"
#include "headers/Environment.h"
#include <fstream>
#include <string.h>
extern Node root;
extern FILE *yyin;
void yy::parser::error(string const&err)
{
  cout << "Parse error... " << err << endl;
}

int main(int argc, char **argv)
{
  yy::parser parser;
  if(argc != 2){
    cout << "Usage: ./int program.lua" << endl;
    return 1;
  }
  yyin = fopen(argv[1], "r");
  if(errno != 0){
    cout << argv[1] << ": " << strerror(errno) << endl;
    return 1;
  }
  
  if(!parser.parse()){
    cout << "Parse complete." << endl;
    root.dump();
    ofstream myfile;
    myfile.open("tree.dot");
    int id = 0;
    myfile << "digraph {" << endl;
    root.dumpToFile(myfile, id);
    myfile << "}" << endl;
    myfile.close(); 
    Environment* e = new Environment();
    root.execute(e);
    return 0;
  }
  return 1;
}
#include<iostream>
#include "interpreter.tab.hh"
#include "headers/Node.h"
#include "headers/Environment.h"
#include <fstream>
extern Node root;
void yy::parser::error(std::string const&err)
{
  std::cout << "Parse error... " << err << std::endl;
}

int main(int argc, char **argv)
{
  yy::parser parser;
  if(!parser.parse()){
    std::cout << "Parse complete." << std::endl;
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
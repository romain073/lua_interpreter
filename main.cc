#include<iostream>
#include "interpreter.tab.hh"
#include "headers/Node.h"
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
    myfile.open("output.dot");
    int id = 0;
    myfile << "digraph {" << endl;
    root.dumpToFile(myfile, id);
    myfile << "}" << endl;
    myfile.close(); 
  }
  
  return 0;
}
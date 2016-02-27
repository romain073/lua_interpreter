#include<iostream>
#include "interpreter.tab.hh"

void yy::parser::error(std::string const&err)
{
  std::cout << "Parse error... " << err << std::endl;
}

int main(int argc, char **argv)
{
  yy::parser parser;
  if(!parser.parse())
    std::cout << "Parse complete." << std::endl;
  return 0;
}
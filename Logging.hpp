#ifndef LOGGING_CUSTOM_HPP
#define LOGGING_CUSTOM_HPP

#include <iostream>
#include "CommandLine.hpp"
class BufferNulo: public std::streambuf{
public:
  int overflow(int c){
    return c;
  }
};

std::ostream &debugOutput(std::ostream &os);

std::ostream &infoOutput(std::ostream &os);

std::ostream &debugOutputNoPrefix(std::ostream &os);

std::ostream &infoOutputNoPrefix(std::ostream &os);

#define debugOut std::cout<<debugOutput
#define infoOut std::cout<<infoOutput
#define debugOutN std::cout<<debugOutputNoPrefix
#define infoOutN std::cout<<infoOutputNoPrefix

#endif

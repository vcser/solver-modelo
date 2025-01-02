
#include <iostream>
#include "CommandLine.hpp"
#include "Logging.hpp"

std::ostream &debugOutput(std::ostream &os){
  if(Commandline::get_verbose()>=2){
    return os<<"\u001b[33mDEBUG: \u001b[0m";
  }else{
    static BufferNulo nulo;
    static std::ostream outputNulo(&nulo);
    return outputNulo;
  }
}

std::ostream &infoOutput(std::ostream &os){
  if(Commandline::get_verbose()>=1){
    return os<<"\u001b[32mINFO: \u001b[0m";
  }else{
    static BufferNulo nulo;
    static std::ostream outputNulo(&nulo);
    return outputNulo;
  }
}

std::ostream &debugOutputNoPrefix(std::ostream &os){
  if(Commandline::get_verbose()>=2){
    return os;
  }else{
    static BufferNulo nulo;
    static std::ostream outputNulo(&nulo);
    return outputNulo;
  }
}

std::ostream &infoOutputNoPrefix(std::ostream &os){
  if(Commandline::get_verbose()>=1){
    return os;;
  }else{
    static BufferNulo nulo;
    static std::ostream outputNulo(&nulo);
    return outputNulo;
  }
}

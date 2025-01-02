#ifndef PREDICTORHOLDER_HPP
#define PREDICTORHOLDER_HPP

#include "Predictor.hpp"
#include "Kitral.hpp"
#include "RedNeuronal.hpp"


class HolderPredictor{
public:

  static Predictor &getPredictor(){ return *_ptr;}
  static void setAsKitral(){ _ptr = (Predictor*)new Kitral();}
  static void setAsNN(){ _ptr = (Predictor*)new RedNeuronal();}
  static Predictor *_ptr;
};

Predictor *HolderPredictor::_ptr = NULL;

#endif
/*
Debemos definir como puramente virtuales las funciones del predictor
*/

#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include "Estructuras.hpp"

class Predictor{
public:
  virtual Perimetro consultarPerimetro(const Incendio &i, double tiempo) = 0;
  virtual Area consultarArea(const Incendio &i, double tiempo) = 0;
  virtual double consultarCosto(const Incendio &i, double tiempo) = 0;
};

#endif

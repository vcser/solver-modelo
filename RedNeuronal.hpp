#ifndef RED_NEURONAL_HPP
#define RED_NEURONAL_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include "Estructuras.hpp"
#include "keras_model.h"
#include "Kitral.hpp"

using namespace keras;

class RedNeuronal : public Kitral{
private:
  static const std::string _PerimetroDumped;
  static const std::string _AreaDumped;

  std::unordered_map<const Incendio*,std::pair<Perimetro,Area> > _incendiosProcesados;
  
  KerasModel * PeriModel;
  KerasModel * AreaModel;

  void _procesaNuevoIncendio(const Incendio &i);

public:
  RedNeuronal(){
    // Inicialización de los modelos en Keras
    PeriModel = new KerasModel(_PerimetroDumped, false); 
    AreaModel = new KerasModel(_AreaDumped, false);
  };

  ~RedNeuronal() {
    _incendiosProcesados.clear();
    delete PeriModel;
    delete AreaModel;
  }
};

#endif

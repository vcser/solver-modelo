#include <iostream>
#include "Incompatibilidades.hpp"
#include "Estructuras.hpp"
#include "Logging.hpp"

bool Incompatibilidades::compatibleRI(const Recurso &r, const Incendio &i) const{
  if(_matrizIncendioRecurso.empty()){
    std::cerr<< "Matriz incompatibilidad Recurso Incendio no esta inicializada!" << std::endl;
  }
  int idRecurso = r.getId();
  int idIncendio = i.getId();
  return _matrizIncendioRecurso[idIncendio][idRecurso];
}

bool Incompatibilidades::compatibleRR(const Recurso &r1, const Recurso &r2) const{
  if(_matrizRecursoRecurso.empty()){
    std::cerr<< "Matriz incompatibilidad Recurso Recurso no esta inicializada!" << std::endl;
  }
  int tipoRecurso1 = r1.getTipo();
  int tipoRecurso2 = r2.getTipo();
  debugOut<<"compatibilidad "<<tipoRecurso1 <<" "<< tipoRecurso2<<" ids "<<
    r1.getId()<<" "<<r2.getId()<<" es " << _matrizRecursoRecurso[tipoRecurso1][tipoRecurso2] <<std::endl;
  return _matrizRecursoRecurso[tipoRecurso1][tipoRecurso2];
}

void Incompatibilidades::setMatrizRecursoRecurso(const std::vector<std::vector<bool> > &m){
  _matrizRecursoRecurso=m;
}

void Incompatibilidades::setMatrizRecursoIncendio(const std::vector<std::vector<bool> > &m){
  _matrizIncendioRecurso=m;
}

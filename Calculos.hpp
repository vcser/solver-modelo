#ifndef CALCULOS_HPP
#define CALCULOS_HPP
#define _USE_MATH_DEFINES

#include <map>
#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <math.h>

#include "Managers.hpp"
#include "Defines.hpp"
#include "Estructuras.hpp"
#include "Predictor.hpp"
//#include "HolderPredictor.hpp"

//considerar rendimiento decreciente
double rendimiento(const Recurso &recurso, const Incendio &incendio);

inline double distLatLon2Met(double lat1, double lon1, double lat2, double lon2);

int calcularDistancia(const Recurso &recurso, const Incendio &incendio);

// double determinarTiempoLlegada(const Recurso &recurso, const Incendio &incendio);

inline double horasDiff(time_t end, time_t begin);


class tablaRendimientos{

public:

  static tablaRendimientos& get(){
    static tablaRendimientos instance;
    return instance;
  }

  tablaRendimientos(tablaRendimientos const &) = delete;
  void operator=(tablaRendimientos const&) = delete;

  double getRendimiento(const Recurso &r, const Incendio &i);
  void setTabla(const std::map<std::pair<int,int> , double> &tabla){_tabla=tabla;}
private:
  tablaRendimientos(){}
  // cambiar a par de enum? matriz?
  std::map<std::pair<int,int> , double> _tabla;
};

class tablaCostosRecursos{

public:

  static tablaCostosRecursos& get(){
    static tablaCostosRecursos instance;
    return instance;
  }

  tablaCostosRecursos(tablaCostosRecursos const &) = delete;
  void operator=(tablaCostosRecursos const&) = delete;

  double getCostoTransporte(const Recurso &r);
  double getCostoUso(const Recurso &r);

  double getCostoTransporte(int tipoRecurso);
  double getCostoUso(int tipoRecurso);

  void setTabla(const std::map<int, std::pair<double,double> > &tabla){_tabla=tabla;}
private:
  tablaCostosRecursos(){}

  // tipo de recurso => costo Transporte, costo Uso del recurso
  std::map<int, std::pair<double,double> >_tabla;
};

class matrizETA{

public:

  static matrizETA& get(){
    static matrizETA instance;
    return instance;
  }

  matrizETA(matrizETA const &) = delete;
  void operator=(matrizETA const&) = delete;

  time_t getETA(const Recurso &r, const Incendio &i);
  void setETA(const Recurso &r, const Incendio &i, time_t eta);

  class Hasher{
  public:
    size_t operator()(const std::pair<int,int> &p) const{
      return std::hash<int>()(p.first)^std::hash<int>()(p.second);
    }
  };

private:


  // Uso de una función hash simple
  matrizETA() {};
  // asociar id de recurso con Incendio al ETA
  std::unordered_map<std::pair<int,int>, time_t, Hasher > _ETAs;
};




#endif

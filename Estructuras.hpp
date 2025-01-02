/*
Le agregue getters a Area y Perimetro... tmb constructores c/arg y sin
Incendio debe tener los getters como const?
Le faltaba a Incendio el init de la temperatura

Agregar pendiente y factorVPL del modelo de combustible hubicado en el foco??
-> No, estos quedan como parámetros de entrada (DN)
*/
#ifndef ESTRUCTURAS_HPP
#define ESTRUCTURAS_HPP

#include <ctime>
#include <vector>
#include "Defines.hpp"


class Recurso{
public:
  /*
  https://docs.google.com/spreadsheets/d/149_ahP2W8upXXqwXTZ6MNCVnnEyKNmQ6uyj5Fugf2K8/edit?pli=1#gid=0

  Estados:

  no operativo
  operativo	disponible listo
	en combate o desplegado
	en actividad
	despachado en preparación
	despachado en camino
	despachado en cierre de actividad
	en descanso
  */
  // Cuantos tipos de recursos distintos pueden existir?
  // enum TipoRecurso {helitransportadas=1, terrestre=2, cisternas=4, mecanizadas=8, interfaz=16, chinook=33};
  // enum EstadoRecurso {inactivo, combatiendo, movilizándose, reposo};

  Recurso(int id, int tipo, double horas, double latitud, double longitud,
    int estado, bool agrupado, int preAsignadoId):  _id(id), _tipo(tipo), _horasDeTrabajo(horas),
    _latitud(latitud), _longitud(longitud), _estado(estado),
    _estaAgrupado(agrupado), _preAsignadoId(preAsignadoId) {};

  //getters
  int getId() const{ return _id;};
  int getTipo() const{ return _tipo;};
  double getHorasDeTrabajo() const{ return _horasDeTrabajo;};
  double getLatitud() const{ return _latitud;}
  double getLongitud() const{ return _longitud;};
  int getEstado() const{ return _estado;};
  bool getEstaAgrupado() const{ return _estaAgrupado;};
  int getPreAsignadoId() const { return _preAsignadoId;} ;

  //usados para ver factibilidad

  // Ya no los necesitamos (tenemos la matriz)
  // inline bool esAereo(){
  //   //TODO: Implementar estilo si es de tipo aereo -> es aereo, aunque esto debería ser
  //   // una función de recurso
  //
  //   // No se a que se refiere con el comentario anterior... pero me imagino que podemos
  //   // guardar los posibles tipos en un enum que sean de esta clase.
  //   return _tipo==aereo;
  // };
  // inline bool esChinook(){
  //   //TODO: Implementar estilo si es de tipo aereo -> es aereo, aunque esto debería ser
  //   // una función de recurso
  //   return false;
  // };

private:

  int _id;
  int _tipo;
  double _horasDeTrabajo;
  double _latitud, _longitud;
  int _estado;
  bool _estaAgrupado;
  int _preAsignadoId;

};

class Incendio{
public:
  Incendio(double latitud, double longitud, double humedad, double rapidez,
    double direccion, double temperatura, double pendiente, double factorVPL,
    time_t timestampInicio, double valorRodalXHectarea, double metrosDeLineaArmadas, int modeloCombustible, int id):
    _latitud(latitud), _longitud(longitud), _humedad(humedad), _rapidezViento(rapidez),
    _direccionViento(direccion), _temperatura(temperatura), _pendiente(pendiente),
    _factorVPL(factorVPL), _timestampInicio(timestampInicio), _valorRodalXHectarea(valorRodalXHectarea),
    _metrosDeLineaArmadas(metrosDeLineaArmadas), _modeloCombustible(modeloCombustible), _id(id){};


  //getters
  double getLatitud() const { return _latitud;}
  int getId() const { return _id;}
  double getLongitud() const {return _longitud;}
  double getTemperatura() const { return _temperatura;}
  double getHumedad() const {return _humedad;}
  double getRapidezViento() const {return _rapidezViento;}
  double getDireccionViento() const {return _direccionViento;}
  double getPendiente() const {return _pendiente;}
  double getFactorVPL() const {return _factorVPL;}
  time_t getTimestampInicio() const {return _timestampInicio;}
  double getValorRodalXHectarea() const {return _valorRodalXHectarea;}
  int getModeloCombustible() const { return _modeloCombustible; }
  double getMetrosDeLineaArmadas() const { return _metrosDeLineaArmadas; }

  //setters
  void setMetrosDeLineaArmadas(double metrosDeLineaArmadas) { _metrosDeLineaArmadas = metrosDeLineaArmadas; }


private:
  int _id;
  double _latitud;
  double _longitud;
  double _temperatura;
  double _humedad;
  double _rapidezViento;
  double _direccionViento;
  double _pendiente;
  double _factorVPL;
  time_t _timestampInicio;
  double _valorRodalXHectarea;
  int _modeloCombustible;
  double _metrosDeLineaArmadas;
};

class Perimetro{
private:
  double _perimetro;

public:
  Perimetro(double p) : _perimetro(p) {};
  Perimetro() : _perimetro(0) {};
  double getPerimetro() {return _perimetro;}

};

class Area{
private:
  double _area;

public:
  Area(double a) : _area(a) {};
  Area() : _area(0) {};
  double getArea() {return _area;}
};


class comparadorPar{
public:
  bool operator() (const std::pair<int, int> &pi1, const std::pair<int, int> &pi2){
    return pi1.first < pi2.first;
  }

};
#endif

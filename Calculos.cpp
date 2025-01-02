#include "Calculos.hpp"
#include "Estructuras.hpp"
#include <iostream>
// #include <cmath>


int calcularDistancia(const Recurso &recurso, const Incendio &incendio){
    // asumimos que contamos en segundos? puede cambiar
    return distLatLon2Met(recurso.getLatitud(),recurso.getLongitud(),
        incendio.getLatitud(), incendio.getLongitud());
}


double rendimiento(const Recurso &recurso, const Incendio &incendio){
  double rendimientoBase = tablaRendimientos::get().getRendimiento(recurso, incendio);
  //TODO: Modificar segun decrecimiento
  return rendimientoBase;
}


inline double horasDiff(time_t end, time_t begin) {
  return std::difftime(end,begin)/3600;
}



inline double distLatLon2Met(double lat1, double lon1, double lat2, double lon2){
  // sacado de https://www.geeksforgeeks.org/program-distance-two-points-earth/

  // Transformar a radianes
  lat1 = lat1*M_PI / 180.0;
  lon1 = lon1*M_PI / 180.0;
  lat2 = lat2*M_PI / 180.0;
  lon2 = lon2*M_PI / 180.0;

  // Formula de Harvesine
  double dlat = lat2-lat1;
  double dlon = lon2-lon1;

  double harvesine = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlon / 2), 2);
  double ans = 2.0 * asin(sqrt(harvesine));

  // multiplicar por el radio de la tierra en metros.
  return ans*6371000.0;
}

double tablaRendimientos::getRendimiento(const Recurso &r, const Incendio &i){
  return _tabla[std::make_pair(r.getTipo(), i.getModeloCombustible())];
}



double tablaCostosRecursos::getCostoTransporte(const Recurso &r) {
 return getCostoTransporte(r.getTipo());
  }

double tablaCostosRecursos::getCostoUso(const Recurso &r) {
  return getCostoUso(r.getTipo());
  }

double tablaCostosRecursos::getCostoTransporte(int tipoRecurso) {
  return _tabla[tipoRecurso].first;
}

double tablaCostosRecursos::getCostoUso(int tipoRecurso) {
  return _tabla[tipoRecurso].second;
}




time_t matrizETA::getETA(const Recurso &r, const Incendio &i){
  return _ETAs[std::make_pair(r.getId(), i.getId())];
}

void matrizETA::setETA(const Recurso &r, const Incendio &i, time_t eta) {
  // std::cerr << " ETA de recurso " << r.getId() <<" a incendio "<< i.getId() <<
  //   " se setea a "<<eta <<std::endl;
  _ETAs[std::make_pair(r.getId(),i.getId())] = eta;
}

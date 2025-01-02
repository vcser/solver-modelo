#ifndef MANAGERS_HPP
#define MANAGERS_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include "Estructuras.hpp"
#include <string>

class HolderRecursos{
public:
  Recurso &getRecurso(int id){ return _recursos[id];};
  int getNumRecursos(){ return _recursos.size();}
  void agregarRecurso(Recurso i){ _recursos.push_back(i);}
  inline void agregarIdentificadorInv(int i, const std::string & s){_identicadorInv.insert(std::make_pair(s,i));}
  inline void agregarIdentificador(int i, const std::string & s){_identicador.insert(std::make_pair(i,s));agregarIdentificadorInv(i, s);}
  inline const std::string & buscarIdentificador(int i){return _identicador[i];}
  inline int buscarIdentificadorInv(const std::string &s){return _identicadorInv[s];}
private:
  std::vector<Recurso> _recursos;
  std::unordered_map<int,std::string> _identicador;
  std::unordered_map<std::string,int> _identicadorInv;
};


class HolderIncendios{
public:
  Incendio &getIncendio(int id){ return _incendios[id];};
  int getNumIncendios(){ return _incendios.size();}
  void agregarIncendio(Incendio i){ _incendios.push_back(i);}
private:
  std::vector<Incendio> _incendios;

};

// Suponemos que siempre viene el par incendio / ciudad mas cercana
class HolderCiudadesCercanas{
public:
  double getCiudadMasCercana(const Incendio &i){ return _ciudadMasCercana[i.getId()]; };
  int getNumIncendios(){ return _ciudadMasCercana.size();}
  void agregarIncendioYCiudad(Incendio &i, double distanciaCiudad){ _ciudadMasCercana[i.getId()] = distanciaCiudad;}
private:
  std::unordered_map<int,double> _ciudadMasCercana;

};

class ManagerEstructuras{
  //Singleton para manejar los holders
public:
  static ManagerEstructuras& get(){
    static ManagerEstructuras instance;
    return instance;
  }

  ManagerEstructuras(ManagerEstructuras const &) = delete;
  void operator=(ManagerEstructuras const&) = delete;

  HolderRecursos &getHolderRecursos(){ return _holderRecursos;}
  HolderIncendios &getHolderIncendios(){ return _holderIncendios;}
  HolderCiudadesCercanas &getHolderCiudadesCercanas() { return _holderCiudades; }

private:
  ManagerEstructuras(){}
  HolderRecursos _holderRecursos;
  HolderIncendios _holderIncendios;
  HolderCiudadesCercanas _holderCiudades;

};

class Solucion{
private:
  std::vector<int> _asignaciones;
public:
  Solucion(){
    // Obtenemos los managers pertinentes
    ManagerEstructuras &manager = ManagerEstructuras::get();
    HolderRecursos &hRecursos = manager.getHolderRecursos();
    std::size_t numRecursos = hRecursos.getNumRecursos();

    // Inicialmente, todos los recursos están sin asignar
    _asignaciones.assign(numRecursos,-1);
  }

  Solucion (const Solucion &s){
    //copy constructor
    _asignaciones=s._asignaciones;
  }

  int asignadoAIncendio(int idRecurso) const {
    return _asignaciones.at(idRecurso);
  }

  void asignarAIncendio(int idRecurso, int idIncendio){
    _asignaciones[idRecurso] = idIncendio;
  }

  void copy(const Solucion &s) {
    _asignaciones=s._asignaciones;
  }
};



#endif

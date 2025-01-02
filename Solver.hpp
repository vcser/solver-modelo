#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Defines.hpp"
#include "Estructuras.hpp"
#include "Managers.hpp"
#include "Predictor.hpp"
#include "Calculos.hpp"
#include <stdexcept>
#include <list>
#include <algorithm>
#include "Parametros.hpp"
#include <set>
#include "Kitral.hpp"
#include "RedNeuronal.hpp"

class Solver{
private:
  void _initPredictor() {
    switch(Parametros::get().getPredictorId()) {
        case 0:
          predictor = new Kitral();
          break;
        case 1:
          predictor = new RedNeuronal();
          break;
        default:
          throw std::runtime_error("Predictor seleccionado invalido.");
      }
  }

protected:
  Solucion * sol;
  HolderIncendios * hIncendios;
  HolderRecursos * hRecursos;
  HolderCiudadesCercanas * hCiudades;
  Predictor * predictor;

  double fitness;
  double calcularFitness();
  Solucion revisarVecindad(Solucion *);
  bool testearSatisfabilidad(std::set<int> &inapagables);
  bool testearSatisfabilidadIncendioIds(std::list<int>&,Incendio&);
  Solver(){};

public:
  double calcularFitness(Solucion *);
  time_t apagaIncendio(const std::vector<int> &idsAsignados, const Incendio &incendio);
  double metrosDeLinea(const Recurso &recurso, const Incendio &incendio, time_t tiempo);
  // double determinarTiempoLlegada(const Recurso &recurso, const Incendio &incendio);
  Solver(HolderIncendios * hIncendios, HolderCiudadesCercanas * hCiudades,
         HolderRecursos * hRecursos, Solucion * sol):hIncendios(hIncendios),
         hCiudades(hCiudades),hRecursos(hRecursos),sol(sol){
      fitness = -1.0;

      _initPredictor();
    };
  virtual bool pasoBusquedaLocal();
  virtual void solve() = 0;

  virtual double getFitness();

  Area consultarArea(const Incendio &i, double tiempo){
    return predictor->consultarArea(i, tiempo);
  }

  Perimetro consultarPerimetro(const Incendio &i, double tiempo){
      return predictor->consultarPerimetro(i, tiempo);
  }

  double consultarCosto(const Incendio &i, double tiempo){
    return predictor->consultarCosto(i, tiempo);
  }

  const Predictor* getPredictor() { return predictor; }
};

#endif

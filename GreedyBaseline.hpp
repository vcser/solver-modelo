#ifndef GREEDY_BASELINE_HPP
#define GREEDY_BASELINE_HPP

#define _USE_MATH_DEFINES

#include <queue>
#include <list>
#include <climits>
#include "Solver.hpp"
#include <random>
#include "Parametros.hpp"
#include <queue>
#include <algorithm>
#include <list>
#include <numeric>
#include <iostream>
#include <cmath>
#include "Incompatibilidades.hpp"

class GreedyBaseline : public Solver{
protected:
  std::list<int> idsRecursos;

  time_t determinarTiempoInicial(const Incendio &);
  void perturbarOrden();
  void inicializarColaIncendios(
    std::priority_queue< std::pair<int,int>, std::vector<std::pair<int,int> >, comparadorPar > &colaIncendios,
    double maxPerturbacion);
  int calcularPrioridad(const Incendio &incendio);
  void perturbarOrden(std::list<int> &l);
  virtual int calcularAfinidad(const Recurso &r, const Incendio &i);
public:
  /*GreedyBaseline(HolderIncendios * hIncendios , HolderRecursos * hRecursos, Solucion * sol):
    Solver(*hIncendios,*hRecursos,*sol){};*/
  using Solver::Solver;
  void solve() override;
  ~GreedyBaseline() {};
};

#endif

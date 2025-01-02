#ifndef SOLVERHOLDER_HPP
#define SOLVERHOLDER_HPP

#include "Solver.hpp"
#include "GreedyBaseline.hpp"
#include "RandomizedGreedy.hpp"
#include "GreedyPorAfinidad.hpp"
#include "BruteForce.hpp"
class HolderSolver{
public:

  static Solver &getSolver(){ return *_ptr;}
  static void setAsGreedyBaseline(HolderIncendios * hIncendios , HolderCiudadesCercanas * hCiudades, HolderRecursos * hRecursos, Solucion * sol){
    _ptr = (Solver*)new GreedyBaseline(hIncendios,hCiudades,hRecursos,sol);
  }
  static void setAsRandomizedGreedy(HolderIncendios * hIncendios , HolderCiudadesCercanas * hCiudades, HolderRecursos * hRecursos, Solucion * sol){
    _ptr = (Solver*)new RandomizedGreedy(hIncendios,hCiudades,hRecursos,sol);
  }
  static void setAsGreedyPorAfinidad(HolderIncendios * hIncendios , HolderCiudadesCercanas * hCiudades, HolderRecursos * hRecursos, Solucion * sol){
    _ptr = (Solver*)new GreedyPorAfinidad(hIncendios,hCiudades,hRecursos,sol);
  }
  static void setAsBruteForce(HolderIncendios * hIncendios , HolderCiudadesCercanas * hCiudades, HolderRecursos * hRecursos, Solucion * sol){
    _ptr = (Solver*)new BruteForce(hIncendios,hCiudades,hRecursos,sol);
  }
  static Solver *_ptr; 
};

Solver *HolderSolver::_ptr = NULL;

#endif

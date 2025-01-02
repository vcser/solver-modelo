#ifndef RANDOMIZEDGREEDY_HPP
#define RANDOMIZEDGREEDY_HPP

#include <random>

#include "GreedyBaseline.hpp"

class RandomizedGreedy : public GreedyBaseline{
private:
  void _ocuparRecurso(std::vector<std::pair<int,int> > &vectorRecursoAfinidad, int idRecurso);
  int _seleccionarRecursoRandom(std::vector<std::pair<int,int> > &vectorRecursoAfinidad);
  const Incendio & _seleccionarIncendioRandom(std::vector<std::pair<int,Incendio> > &vectorIncendios);
  std::vector<std::pair<int,int> > _getAfinidadesDisponibles(const Incendio &in, std::list<int> &idsRecursos);
  void _inicializarVectorIncendios(std::vector<std::pair<int,Incendio> > &vectorIncendios,
    double maxPerturbacion);

  int calcularAfinidad(const Recurso &recurso, const Incendio &incendio) override;

public:
  /*RandomizedGreedy(HolderIncendios * hIncendios, HolderRecursos * hRecursos, Solucion * sol):
    GreedyBaseline(hIncendios,hRecursos,sol){};*/
  using GreedyBaseline::GreedyBaseline;
  void solve() override;
  ~RandomizedGreedy(){};
};

#endif

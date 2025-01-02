#ifndef GREEDY_POR_AFINIDAD
#define GREEDY_POR_AFINIDAD

#include "GreedyBaseline.hpp"

class GreedyPorAfinidad : public GreedyBaseline{
protected:
  int calcularAfinidad(const Recurso&, const Incendio&) override;
public:
  GreedyPorAfinidad(HolderIncendios * hIncendios ,
    HolderCiudadesCercanas * hCiudades, HolderRecursos * hRecursos, Solucion * sol):
    GreedyBaseline(hIncendios, hCiudades, hRecursos, sol){};
};

#endif

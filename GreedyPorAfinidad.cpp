#include "GreedyPorAfinidad.hpp"
#include "Calculos.hpp"
#include "Defines.hpp"
#include "Logging.hpp"
int GreedyPorAfinidad::calcularAfinidad(const Recurso &r, const Incendio &i){
  int afinidad = metrosDeLinea(r, i, Parametros::get().getTimeStamp()+3600
    *Parametros::get().getHorasCalculoAfinidad())/ tablaCostosRecursos::get().getCostoUso(r);

  infoOut << "La afinidad del recurso "<<r.getId() <<" en el incendio " << i.getId()
    << " es de "<<afinidad << std::endl;

  return afinidad;
}

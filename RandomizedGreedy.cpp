#include <iostream>
#include <algorithm>
#include "RandomizedGreedy.hpp"
#include "Incompatibilidades.hpp"
#include "Logging.hpp"
int RandomizedGreedy::calcularAfinidad(const Recurso &recurso, const Incendio &incendio)
{
  // asumimos que contamos en segundos? puede cambiar
  int afinidad = metrosDeLinea(recurso, incendio, Parametros::get().getTimeStamp() + 3600 * Parametros::get().getHorasCalculoAfinidad()) / tablaCostosRecursos::get().getCostoUso(recurso);

  infoOut << "La afinidad del recurso " << recurso.getId() << " en el incendio " << incendio.getId()
          << " es de " << afinidad << std::endl;

  return afinidad;
}

void RandomizedGreedy::_inicializarVectorIncendios(std::vector<std::pair<int, Incendio>> &vectorIncendios,
                                                   double maxPerturbacion = 0.0)
{
  vectorIncendios.clear();

  for (int i = 0; i < hIncendios->getNumIncendios(); i++)
  {
    int randNumber = rand() % 1000;
    double perturbacion = 1.0 - maxPerturbacion + (2.0 * randNumber * maxPerturbacion) / 1000.0;
    Incendio in = hIncendios->getIncendio(i);
    int prioridad = calcularPrioridad(in) * perturbacion;

    vectorIncendios.emplace_back(prioridad, in);
  }
}

std::vector<std::pair<int, int>> RandomizedGreedy::_getAfinidadesDisponibles(const Incendio &in, std::list<int> &idsRecursos)
{
  // TODO: precalcular aquí recursos disponibles con el incendio

  std::vector<std::pair<int, int>> res;
  for (auto it = idsRecursos.begin(); it != idsRecursos.end(); it++)
  {

    int idRecurso = *it;
    const Recurso &rec = hRecursos->getRecurso(idRecurso);
    int afinidad = calcularAfinidad(rec, in);
    res.push_back(std::make_pair(idRecurso, afinidad));
  }
  return res;
}

const Incendio &RandomizedGreedy::_seleccionarIncendioRandom(std::vector<std::pair<int, Incendio>> &vectorIncendios)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dist(0.0, 1.0);
  int prioridadTotal = 0;
  for (int i = 0; i < vectorIncendios.size(); i++)
  {
    prioridadTotal += vectorIncendios[i].first;
    debugOut << "Prioridad incendio " << vectorIncendios[i].second.getId() << " es " << vectorIncendios[i].first << std::endl;
  }

  int elegido = prioridadTotal * dist(gen);
  int prioridadAcumulada = 0;
  for (int i = 0; i < vectorIncendios.size(); i++)
  {
    prioridadAcumulada += vectorIncendios[i].first;
    if (prioridadAcumulada > elegido)
    {
      return vectorIncendios[i].second;
    }
  }
  // caso borde
  return vectorIncendios.back().second;
}

int RandomizedGreedy::_seleccionarRecursoRandom(std::vector<std::pair<int, int>> &vectorRecursoAfinidad)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dist(0.0, 1.0);
  int afinidadTotal = 0;
  for (int i = 0; i < vectorRecursoAfinidad.size(); i++)
  {
    afinidadTotal += vectorRecursoAfinidad[i].second;
  }

  int elegido = afinidadTotal * dist(gen);
  int prioridadAcumulada = 0;
  for (int i = 0; i < vectorRecursoAfinidad.size(); i++)
  {
    prioridadAcumulada += vectorRecursoAfinidad[i].second;
    if (prioridadAcumulada > elegido)
    {
      return vectorRecursoAfinidad[i].first;
    }
  }
  // caso borde
  return vectorRecursoAfinidad.back().first;
}

void RandomizedGreedy::_ocuparRecurso(std::vector<std::pair<int, int>> &vectorRecursoAfinidad,
                                      int idRecurso)
{
  for (int i = 0; i < vectorRecursoAfinidad.size(); i++)
  {
    if (vectorRecursoAfinidad[i].first == idRecurso)
    {
      vectorRecursoAfinidad.erase(vectorRecursoAfinidad.begin() + i);
      return;
    }
  }
}

void RandomizedGreedy::solve()
{
  std::set<int> inapagables;
  if (!testearSatisfabilidad(inapagables))
  {
    debugOut << "Alerta-> Al menos un foco no se puede apagar" << std::endl;
  }

  Incompatibilidades &tablaIncompatibilidad = Incompatibilidades::get();
  idsRecursos.resize(hRecursos->getNumRecursos());

  // TODO: revisar si dejamos ids de recursos desde 0?
  std::iota(idsRecursos.begin(), idsRecursos.end(), 0);

  std::vector<std::pair<int, Incendio>> vectorIncendios;

  _inicializarVectorIncendios(vectorIncendios);

  double perturbacionAfinidad = 0.0;

  bool repitoMismoIncendio = false;
  while (!vectorIncendios.empty())
  {

    Incendio actual = _seleccionarIncendioRandom(vectorIncendios);

    if (inapagables.count(actual.getId()))
    {
      infoOut << "El incendio " << actual.getId() << " es inapagable! " << std::endl;
      for (int i = 0; i < vectorIncendios.size(); i++)
      {
        if (vectorIncendios[i].second.getId() == actual.getId())
        {
          vectorIncendios.erase(vectorIncendios.begin() + i);
        }
      }
      continue;
    }

    debugOut << "Asignamos recursos al foco " << actual.getId() << std::endl;

    if (!testearSatisfabilidadIncendioIds(idsRecursos, actual))
    {
      debugOut << "No se puede satisfacer con los recursos disponibles\n"
               << "Reintentando con orden perturbado " << std::endl;
      // TODO: Setear este incremento como un parametro

      _inicializarVectorIncendios(vectorIncendios);
      // Reiniciar los recursos disponibles
      *sol = Solucion();
      idsRecursos.resize(hRecursos->getNumRecursos());
      std::iota(idsRecursos.begin(), idsRecursos.end(), 0);
      // Reintentamos desde un principio, en orden alterado
      continue;
    }

    auto it = idsRecursos.begin();

    if (it == idsRecursos.end())
    {
      // comenzar de nuevo?
      // variar algo en la cola?
      perturbacionAfinidad += 0.05f;
      // no usar afinidad? probar bien despues
      _inicializarVectorIncendios(vectorIncendios);
      continue;
    }
    std::vector<std::pair<int, int>> vectorRecursoAfinidad =
        _getAfinidadesDisponibles(actual, idsRecursos);
    // Posible optimización -> sort parcial?

    // Ordenar recursos restantes x afinidad
    // solo si es que no estamos re-iterando sobre el mismo incendio
    // if(!repitoMismoIncendio){
    //   idsRecursos.sort(
    //     [actual](const Recurso &l, const Recurso &r) -> bool{
    //       return calcularAfinidad(l, actual) > calcularAfinidad(r,actual);
    //     }
    //   );
    // }else{
    //   perturbarOrden(idsRecursos);
    // }

    // El tiempo de inicio más 1 hora
    double tObjetivo = determinarTiempoInicial(actual) + 21600;
    // dejar en define
    time_t apagado = std::numeric_limits<time_t>::max();
    // Mientras no se apaga el incendio, asignamos recursos según afinidad Y random
    std::vector<int> idsAsignados;
    // si no se apaga, intentar de nuevo en otro orden

    // hasta que nos quedemos sin cosas que asignar?
    while (!vectorRecursoAfinidad.empty())
    {

      // cuidado con cuando está vacio
      int idRecursoAsignado = _seleccionarRecursoRandom(vectorRecursoAfinidad);
      infoOut << "Intendo asignar recurso " << idRecursoAsignado << " a incendio " << actual.getId() << std::endl;
      Recurso &recursoActual = hRecursos->getRecurso(idRecursoAsignado);

      // hard codear restricciones?
      // si son relativamente pocas, puede salir mas facil
      // verificar factibilidad en seleccion del recurso
      // ej -> incompatibilidad de recursos

      // DONE: cambiar todo esto por usar matrices de compatibilidad
      //  bool recursoAereo = esAereo(recursoActual);
      //  bool chinook = esChinook(recursoActual);
      //  Revisar si se puede usar (estado)

      // Si es aereo, verificamos no estar usando Chinook
      //  factibleDeUsar = factibleDeUsar && !(recursoAereo && usamosChinook );
      // Si es Chinook, verificamos no estar usando aereos
      //  factibleDeUsar = factibleDeUsar && !(chinook && usamosAereos );

      bool factibleDeUsar = tablaIncompatibilidad.compatibleRI(recursoActual, actual);
      if (!factibleDeUsar)
      {
        debugOut << " El recurso " << recursoActual.getId() << " no es compatible con foco " << actual.getId() << std::endl;
      }
      // Solo iteramos si sigue siendo factible usarlo
      for (int k = 0; factibleDeUsar && k < idsAsignados.size(); k++)
      {
        Recurso &anteriormenteAsignado = hRecursos->getRecurso(idsAsignados[k]);
        // Si no es compatible con alguno anteriormente asignado...
        if (!tablaIncompatibilidad.compatibleRR(anteriormenteAsignado, recursoActual))
        {
          factibleDeUsar = false;
        }
      }

      if (factibleDeUsar)
      {
        idsAsignados.push_back(idRecursoAsignado);
        _ocuparRecurso(vectorRecursoAfinidad, idRecursoAsignado);
      }
      else
      {

        continue;
      }

      // s.asignarAIncendio(idRecursoAsignado, actual.getId());

      apagado = apagaIncendio(idsAsignados, actual);
      if (apagado <= tObjetivo)
      {
        // ta bien, sacamos los recursos de la lista de disponibles
        // infoOut << "Esta asignacion apaga el incendio " << actual.getId() << std::endl;

        std::sort(idsAsignados.begin(), idsAsignados.end());
        debugOut << "idsAsignados: " << std::endl;
        for (int i = 0; i < idsAsignados.size(); i++)
        {
          debugOutN << idsAsignados[i] << " ";
        }
        debugOutN << std::endl;
        debugOut << "idsDisponibles: " << std::endl;
        for (auto i = idsRecursos.begin(); i != idsRecursos.end(); i++)
        {
          debugOutN << *i << " ";
        }
        debugOutN << std::endl;

        auto i = idsRecursos.begin();
        auto j = idsAsignados.begin();
        while (j != idsAsignados.end())
        {
          if (i == idsRecursos.end())
          {
            debugOut << "Error: Los recursos asignados no son subconjunto de los disponibles" << std::endl;
          }
          if (*i == *j)
          {
            sol->asignarAIncendio(*i, actual.getId());
            i = idsRecursos.erase(i);
            j++;
          }
          else
          {
            i++;
          }
        }
        break;
      }
    }

    if (apagado > tObjetivo)
    {
      // ta mal
      infoOut << "No se pudo apagar el incendio " << actual.getId() << " reintentando con otra asignacion " << std::endl;
      repitoMismoIncendio = true;
    }
    else
    {
      // borrar incendio actual del vector
      // infoOut << "El incendio " << actual.getId() << " se pudo apagar con la asignacion con t=" << apagado << std::endl;
      // infoOut << apagado - actual.getTimestampInicio() << " segundos desde el inicio" << std::endl;
      for (int i = 0; i < vectorIncendios.size(); i++)
      {
        if (vectorIncendios[i].second.getId() == actual.getId())
        {
          vectorIncendios.erase(vectorIncendios.begin() + i);
        }
      }
      repitoMismoIncendio = false;
    }
  }
}

#include "GreedyBaseline.hpp"

#include <limits>
#include "Logging.hpp"
void GreedyBaseline::inicializarColaIncendios(std::priority_queue<std::pair<int,int>,
  std::vector<std::pair<int,int> >, comparadorPar >  &colaIncendios,
  double maxPerturbacion = 0){

  // Vaciar la cola de prioridad
  colaIncendios = std::priority_queue<std::pair<int,int>,
    std::vector<std::pair<int,int> >, comparadorPar >();
  //TODO: Perturbar orden de asignacion a incendios???
  for(int i=0; i < hIncendios->getNumIncendios(); i++){
    int randNumber = rand()%1000;
    double perturbacion = 1.0-maxPerturbacion + (2.0*randNumber*maxPerturbacion)/1000.0;
    Incendio &in = hIncendios->getIncendio(i);
    int prioridad = calcularPrioridad(in)*perturbacion;
    // std::cerr << "Incendio "<<in.getId() << " tiene una prioridad de "<<prioridad<<std::endl;
    colaIncendios.push(std::make_pair(prioridad, i));
  }
}
// si un incendio no se puede apagar con todos los recursos, alertar al usuario

int GreedyBaseline::calcularAfinidad(const Recurso &r, const Incendio &i){
  // En negativo, porque mayor distancia implica menor afinidad
  return -calcularDistancia(r, i);
}

void GreedyBaseline::solve(){
  std::set<int> inapagables;
  if(!testearSatisfabilidad(inapagables)){
    debugOut<<"Alerta-> Al menos un foco no se puede apagar"<<std::endl;
  }

  idsRecursos.resize(hRecursos->getNumRecursos());

  //TO DO: revisar si dejamos ids de recursos desde 0?
  std::iota(idsRecursos.begin(), idsRecursos.end(), 0);

  std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int> >, comparadorPar > colaIncendios;

  Incompatibilidades &tablaIncompatibilidad = Incompatibilidades::get();

  inicializarColaIncendios(colaIncendios);

  double perturbacionAfinidad = 0.0;
  double perturbacionPrioridad = 0.0;

  bool repitoMismoIncendio = false;
  while( !colaIncendios.empty() ){

      int idIncendioActual =  colaIncendios.top().second;

      if(inapagables.count(idIncendioActual)){
        infoOut << "El incendio " << idIncendioActual << " es inapagable! "<<std::endl;
        colaIncendios.pop();
        continue;
      }

      Incendio &actual = hIncendios->getIncendio(idIncendioActual);
      infoOut << "Asigno recursos hacia el foco "<< actual.getId() << std::endl;

      if(!testearSatisfabilidadIncendioIds(idsRecursos, actual)){
        debugOut << "No se puede satisfacer con los recursos disponibles\n"<<
        "Reintentando con orden perturbado "<<std::endl;
        //TODO: Setear este incremento como un parametro
        perturbacionPrioridad+=0.05;
        inicializarColaIncendios(colaIncendios, perturbacionPrioridad);
        //Reiniciar los recursos disponibles
        idsRecursos.resize(hRecursos->getNumRecursos() );
        std::iota(idsRecursos.begin(), idsRecursos.end(), 0);
        *sol = Solucion();
        //Reintentamos desde un principio, en orden alterado
        continue;
      }


      auto it = idsRecursos.begin();

      if(it == idsRecursos.end()){
        // comenzar de nuevo?
        //variar algo en la cola?
        perturbacionAfinidad += 0.05f;
        inicializarColaIncendios(colaIncendios, perturbacionAfinidad);
        continue;
      }

      //Posible optimización -> sort parcial?

      // Ordenar recursos restantes x afinidad
      // solo si es que no estamos re-iterando sobre el mismo incendio
      if(!repitoMismoIncendio){
        infoOut <<" ordeno idsRecursos" <<std::endl;
        idsRecursos.sort(
          // Arreglado
          [this, actual](int l, int r) -> bool{
            return this->calcularAfinidad(this->hRecursos->getRecurso(l), actual) > this->calcularAfinidad(this->hRecursos->getRecurso(r),actual);
          }

        );

      }else{
        perturbarOrden(idsRecursos);
      }
      infoOut << "Las ids de recursos disponibles ordenados son:";
      for( auto &x : idsRecursos){
        infoOutN << x << " ";
      }
      infoOutN << std::endl;
      it = idsRecursos.begin();
      // TODO: cambiar 21600 por un parametro?
      time_t tObjetivo = determinarTiempoInicial(actual) + 21600;
      time_t apagado = std::numeric_limits<time_t>::max();
      // Mientras no se apaga el incendio, asignamos recursos según afinidad
      std::vector<int> idsAsignados;
      //si no se apaga, intentar de nuevo en otro orden
      //Comentados por que no los usamos
      //bool usamosAereos = false;
      //bool usamosChinook = false;

      while(it != idsRecursos.end()){

        //cuidado con cuando está vacio
        int idRecursoAsignado = *it;
        infoOut << "Intento asignar recurso con id "<< idRecursoAsignado<<std::endl;
        Recurso &recursoActual = hRecursos->getRecurso(idRecursoAsignado);

        bool factibleDeUsar = tablaIncompatibilidad.compatibleRI(recursoActual, actual);
        if(!factibleDeUsar){
          debugOut << " El recurso "<<recursoActual.getId()<<" no es compatible con foco "<<
              actual.getId()<<std::endl;
        }
        //Solo iteramos si sigue siendo factible usarlo
        for(int k=0; factibleDeUsar && k<idsAsignados.size(); k++){
          Recurso &anteriormenteAsignado = hRecursos->getRecurso(idsAsignados[k]);
          // Si no es compatible con alguno anteriormente asignado...
          if ( ! tablaIncompatibilidad.compatibleRR(anteriormenteAsignado, recursoActual) ){
            factibleDeUsar=false;
          }
        }


        // Cambio de estilo en revisar restricciones -> usar tablas
        if( factibleDeUsar){
          std :: cerr << "Es factible de usar "<<std::endl;
          idsAsignados.push_back(idRecursoAsignado);
        }else{
          std :: cerr << "No es factible de usar "<<std::endl;
          it++;
          continue;
        }

        // s.asignarAIncendio(idRecursoAsignado, actual.getId());
        //Probamos si es que sirve de algo, sino lo devolvemos
        apagado = apagaIncendio(idsAsignados, actual);

        if(apagado <= tObjetivo){
          std :: cerr << "Esta asignacion apaga el incendio "<<actual.getId()<<std::endl;
          // ta bien, sacamos los recursos de la lista de disponibles
          auto i = idsRecursos.begin();
          auto j = idsAsignados.begin();
          while(j!=idsAsignados.end()){
            if(i==idsRecursos.end()){
              std::cerr << "Los recursos asignados no son subconjunto de los disponibles" << std::endl;
            }
            if(*i == *j){
              sol->asignarAIncendio(*i, actual.getId());
              i = idsRecursos.erase(i);
              j++;
            }else{
              i++;
            }
          }
          break;
        }
        it++;

      }

      if(apagado > tObjetivo){
        //ta mal
        std :: cerr << "No se pudo apagar el incendio "<<actual.getId()<<
            " reintentando con otra asignacion "<<std::endl;
        repitoMismoIncendio = true;
      }else{
        std :: cerr << "El incendio "<<actual.getId()<<
            " se pudo apagar con la asignacion con t="<<apagado<<std::endl;
        std::cerr << apagado-actual.getTimestampInicio() << " segundos desde el inicio" <<
          std::endl;
        colaIncendios.pop();
        repitoMismoIncendio=false;
      }

  }
}

time_t GreedyBaseline::determinarTiempoInicial(const Incendio &i){


  return i.getTimestampInicio();
}

void GreedyBaseline::perturbarOrden(){
  std::random_device rd;
  std::mt19937 seed(rd()); // seed the generator
  std::uniform_int_distribution<> rec(0, hRecursos->getNumRecursos());
  std::uniform_int_distribution<> inc(-1, hIncendios->getNumIncendios());

  int nPerturbaciones = Parametros::get().getNPerturbaciones();

  for (int i=0; i<nPerturbaciones; i++) {
    int idRecurso = rec(seed);
    int asignacion = inc(seed);

    sol->asignarAIncendio(idRecurso, asignacion);
  }
}

void GreedyBaseline::perturbarOrden(std::list<int> &l){

  debugOut << "Antes de perturbar el orden, este es: "<<std::endl;

  for(auto &x : l){
    debugOutN <<x<<" ";
  }
  debugOutN << std::endl;

  std::random_device rd;
  std::mt19937 seed(rd()); // seed the generator
  std::uniform_int_distribution<> rec(0, l.size()-1);
  std::uniform_int_distribution<> rec2(0, l.size()-1);

  int nPerturbaciones = Parametros::get().getNPerturbaciones();

  for (int i=0; i<nPerturbaciones; i++) {
    int idRecurso1 = rec(seed);
    int idRecurso2 = rec2(seed);
    debugOut << "intercambio posiciones "<<idRecurso1 <<" "<<idRecurso2<<std::endl;
    auto it1 = l.begin();
    auto it2 = l.begin();
    std::advance(it1, idRecurso1);
    std::advance(it2, idRecurso2);
    std::swap(*it1, *it2);
  }

  debugOut << "Despues de perturbar el orden, este es: "<<std::endl;

  for(auto &x : l){
    debugOutN <<x<<" ";
  }
  debugOutN << std::endl;

}

int GreedyBaseline::calcularPrioridad(const Incendio &incendio){
  //TODO: Implementar
  // Cercania a poblado a partir de una burda aproximacion de distancia radial, utilizando el perimetro
  // (es mas rapido que el area) aproximamos un radio circunferencial.
  auto perimetro = predictor->consultarPerimetro(incendio, CONST6HORAS/3600).getPerimetro();
  auto radioDeIncendio = Parametros::get().getMargenDeSeguridadCiudad() * perimetro / (2*M_PI);

  auto distanciaCiudadMasCercana = hCiudades->getCiudadMasCercana(incendio);

  debugOut << "Distancia a ciudad mas cercana incendio "<<incendio.getId()<< " " <<
      distanciaCiudadMasCercana <<std::endl;
  int costo;
  debugOut << "Prioridad de incendio "<<incendio.getId()<<std::endl;
  if (distanciaCiudadMasCercana <= radioDeIncendio){
    debugOut << "Infinita porque distancia Ciudad mas cercana es "<<distanciaCiudadMasCercana<<
      " y radio de incendio es "<<radioDeIncendio<<std::endl;
    costo = INT_MAX;
  }else {
    auto costo6Horas = predictor->consultarCosto(incendio, 6.0);
    costo = costo6Horas;// + alguna ponderaccion dada a la distancia con la ciudad...
    debugOut << "Costo: "<< costo6Horas << " "<<costo <<std::endl;
  }

  return costo;
}

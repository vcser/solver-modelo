/*
Cambiar el timestamp de retorno cuando no se puede apagar un incendio...
El max() genera un overflow cuando se hace la conversion a segundos.
*/

#include "Solver.hpp"
#include <limits>
#include <algorithm>
#include <numeric>
#include "Logging.hpp"
#include "AnytimeRecorder.hpp"


double Solver::getFitness(){
    calcularFitness();
    return fitness;
  };

bool Solver::pasoBusquedaLocal(){
  double currentFitness = calcularFitness();
  Solucion siguiente = revisarVecindad(sol);
  double fitnessVecino = calcularFitness(&siguiente);
  // Si la nueva solucion es mejor
  if( fitnessVecino > currentFitness ){
    *sol = siguiente;
    fitness = fitnessVecino;
    AnytimeRecorder::get().agregarMedicion(clock(), fitness);
    return true;
  }else{
    return false;
  }
}

bool Solver::testearSatisfabilidad(std::set<int> &inapagables){
  int numRecursos = hRecursos->getNumRecursos();
  int numIncendios = hIncendios->getNumIncendios();
  std::vector<int> idsTodosLosRecursos(numRecursos);
  std::iota(idsTodosLosRecursos.begin(), idsTodosLosRecursos.end(), 0);
  bool can = true;
  for(int i=0; i< numIncendios; i++){
    Incendio &in = hIncendios->getIncendio(i);
    time_t tiempoApagado = apagaIncendio(idsTodosLosRecursos, in);
    if(tiempoApagado > in.getTimestampInicio()+21600){
      inapagables.insert(in.getId());
      can=false;
      infoOut<<"No se puede apagar el incendio "<<i<<" ni siquiera con todos los recursos!"<<std::endl;
    }
  }
  return can;
}

bool Solver::testearSatisfabilidadIncendioIds(std::list<int> &l,Incendio &i){
  std::vector<int> vectorDeLista(l.begin(),l.end());
  time_t tiempoApagado = apagaIncendio(vectorDeLista, i);
  if(tiempoApagado > i.getTimestampInicio()+121600){
    infoOut<<"No se puede apagar el incendio "<<i.getId()<<" con los recursos disponibles!"<<std::endl;
    return false;
  }
  return true;
}

Solucion Solver::revisarVecindad(Solucion *s){
  ManagerEstructuras &manager = ManagerEstructuras::get();
  HolderRecursos &hRecursos = manager.getHolderRecursos();
  HolderIncendios &hIncendios = manager.getHolderIncendios();
  std::size_t numRecursos = hRecursos.getNumRecursos();
  int numIncendios = hIncendios.getNumIncendios();
  Solucion ans = *s;
  std::vector<int> idsRecursosOciosos;
  // ver recursos ociosos
  for(int i=0; i<numRecursos; i++){
    if(s->asignadoAIncendio(i)==-1){
      idsRecursosOciosos.push_back(i);
    }
  }

  // Inicialmente, no movemos nada
  int mejorRecursoAsignado = -1;
  int mejorAsignacionIncendio = -1;
  double mejorFitness = calcularFitness(s);
  for(int i=0; i<idsRecursosOciosos.size(); i++){
    //Probar asignar este recurso a cada incendio
    for(int j=0; j<numIncendios; j++){
      //Asignamos este recurso con id i al incendio con id j
      int idRecursoAsignado = idsRecursosOciosos[i];
      ans.asignarAIncendio(idRecursoAsignado, j);
      double fitnessAsignacion = calcularFitness(&ans);
      if( fitnessAsignacion>mejorFitness ){
        mejorRecursoAsignado=idRecursoAsignado;
        mejorAsignacionIncendio=j;
      }
      //Revertimos asignación
      ans.asignarAIncendio(idRecursoAsignado,-1);
    }
  }

  if( mejorRecursoAsignado!=-1){
    // Encontramos una asignación mejor, aplicar y retornar
    ans.asignarAIncendio(mejorRecursoAsignado,mejorAsignacionIncendio);
  }

  return ans;

}


double Solver::calcularFitness(Solucion *s){
  double costoSinControl = 0;
  double localFitness = 0;
  for( int i=0; i< hIncendios->getNumIncendios(); i++){
    Incendio &in = hIncendios->getIncendio(i);
    // Definir como dejamos 6 horas ( 6 * 3600.0, 6? )
    costoSinControl += predictor->consultarCosto(in, 6);
  }
  std::vector<std::vector<int> > recursosPorIncendio(hIncendios->getNumIncendios());
  for(int recId = 0; recId < hRecursos->getNumRecursos(); recId++){
    int inId = s->asignadoAIncendio(recId);
    if( inId!=-1){
      recursosPorIncendio[inId].push_back(recId);
    }
  }

  double costoQuemadoSolucion = 0;

  for( int i=0; i< hIncendios->getNumIncendios(); i++){
    Incendio &in = hIncendios->getIncendio(i);
    time_t tiempoApagado = apagaIncendio(recursosPorIncendio[i], in);
    int segundosPropagacion = tiempoApagado-in.getTimestampInicio();
    double horasPropagacion = (double)segundosPropagacion/3600.0;

    // Parche por overflow de (transformacion tiempo maximo - timestamp) a segundos
    if (horasPropagacion < 0 || horasPropagacion > 6.0) horasPropagacion = 6;
    costoQuemadoSolucion += predictor->consultarCosto(in, horasPropagacion);

  }

  // Costo de los recursos, junto con los transportes
  double costoTransporteRecursos = 0;
  double costoUsoRecursos = 0;
  tablaCostosRecursos &costosRecursos = tablaCostosRecursos::get();
  time_t actual = Parametros::get().getTimeStamp();
  matrizETA &etas = matrizETA::get();

  for( int i=0; i< hIncendios->getNumIncendios(); i++){
    Incendio &in = hIncendios->getIncendio(i);
    time_t tiempoApagado = apagaIncendio(recursosPorIncendio[i], in);
    int segundosPropagacion = tiempoApagado-actual;
    double horasPropagacion = (double)segundosPropagacion/3600.0;
    // std::cout << horasPropagacion << std::endl;

    // Parche por overflow de (transformacion tiempo maximo - timestamp) a segundos
    if (horasPropagacion < 0 || horasPropagacion > 6.0) horasPropagacion = 6;

    for (int &idRecurso : recursosPorIncendio[i]) {
      Recurso &recurso = hRecursos->getRecurso(idRecurso);
      time_t recursoETA = etas.getETA(recurso, in);
      int segundosETA =  (actual < recursoETA) ? recursoETA-actual : 0;
      double horasETA = segundosETA/3600.0;
      costoTransporteRecursos += horasETA * costosRecursos.getCostoTransporte(recurso.getTipo());
      costoUsoRecursos += (horasPropagacion-horasETA) * costosRecursos.getCostoUso(recurso.getTipo());
    }
  }


  // std::cout << costoSinControl << " " << costoQuemadoSolucion << " " << costoUsoRecursos << " " << costoTransporteRecursos << std::endl;
  localFitness = costoSinControl - costoQuemadoSolucion - costoUsoRecursos - costoTransporteRecursos;

  return localFitness;
}

double Solver::calcularFitness(){
  fitness = calcularFitness(sol);
  return fitness;
}

double Solver::metrosDeLinea(const Recurso &recurso, const Incendio &incendio, time_t tiempo){

  matrizETA &etas = matrizETA::get();

  tablaRendimientos *table = &tablaRendimientos::get();
  // Cuantos metros de linea hace en el tiempo
  // time_t tiempoLlegada = std::max(etas.getETA(recurso, incendio),tiempo);
  unsigned int tiempoDesdeInicio = std::max(etas.getETA(recurso, incendio),Parametros::get().getTimeStamp());
  // std::cerr << recurso.getId() <<" llega a incendio "<< incendio.getId() << " en "<<
  //   tiempoLlegada << "(s) y han pasado " <<tiempoDesdeInicio<<"(s) desde inicio" <<std::endl;
  if(tiempo < tiempoDesdeInicio){
    return 0;
  }
  // int tiempoEnCombate = tiempoDesdeInicio-tiempoLlegada;
  int tiempoEnCombate = tiempo - std::max(etas.getETA(recurso,incendio), Parametros::get().getTimeStamp());
  double horasEnCombate = (double) tiempoEnCombate/3600.0;
  double rendimientoRec = table->getRendimiento(recurso, incendio);
  double metrosDeLineaArmados = horasEnCombate*rendimientoRec;
  return metrosDeLineaArmados;
}

//Devuelve timestamp en que el incendio se apaga
time_t Solver::apagaIncendio(const std::vector<int> &idsAsignados, const Incendio &incendio){
  //bisección para ver tiempo de apagado del incendio

  time_t l = Parametros::get().getTimeStamp();
  time_t r = incendio.getTimestampInicio()+21600;

  //Seteo limite derecho más alto que el usado (12 horas )
  // Fix -> obtener predictor desde un singleton o algo similar

  bool puedeApagar = false;
  while( r>l ){
    int m = l + (r-l)/2;
    int segundosDesdeInicioM = m-incendio.getTimestampInicio();
    double horasDesdeInicioM = (double)segundosDesdeInicioM/3600.0;
    //SOLVED: por alguna razon, me da perimetro distnto cuando consulto antes de la escritura?
    // era por punteros
    double perimetroIncendioEnInstanteM = predictor->consultarPerimetro(incendio, horasDesdeInicioM).getPerimetro()
      +1;
    perimetroIncendioEnInstanteM -= incendio.getMetrosDeLineaArmadas();
    debugOut << "El perimetro del incendio "<<incendio.getId()<<" a las "<<
      horasDesdeInicioM<<" horas es de " << perimetroIncendioEnInstanteM << " metros" << std::endl;
    for(const int &id : idsAsignados){
      Recurso &rec = hRecursos->getRecurso(id);

      double metrosArmados = metrosDeLinea(rec, incendio, m);
      debugOut << "El recurso "<<rec.getId()<<" hace "<<metrosArmados << " de linea en el incendio "<<
      incendio.getId()<< " a las "<<horasDesdeInicioM<<" horas desde inicio"<<std::endl;
      perimetroIncendioEnInstanteM -= metrosArmados;
    }
    if( perimetroIncendioEnInstanteM > 0.0){
      // No lo conseguimos apagar aun
      l = m+1;
    }else{
      // Si se puede apagar durante este tiempo
      puedeApagar = true;
      r = m;
    }

  }
  if( puedeApagar ){
    return r;
  }else{
    return std::numeric_limits<time_t>::max();
  }
}

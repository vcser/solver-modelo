#ifndef LECTURA_HPP
#define LECTURA_HPP

#include "Estructuras.hpp"
#include "Managers.hpp"
#include "Incompatibilidades.hpp"
#include "Defines.hpp"
#include "Calculos.hpp"
#include "Parsing.hpp"
#include "Logging.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <utility>
/*
  TODO:
    -Rectificador de parametros
*/

/*
Lectura de Archivo/Linea de comandas escrito como:
---
TIME_STAMP_MOMENTO_ACTUAL
NUMERO_DE_FOCOS
LATITUD_FOCO_1 LONGITUD_FOCO_1
HUMEDAD_RELATIVA_FOCO_1 VELOCIDAD_VIENTO_FOCO_1 DIRECCION_VIENTO_FOCO_1 TEMPERATURA_AMBIENTE_FOCO_1 PENDIENTE_FOCO_1 FACTOR_VPL_FOCO_1 TIMESTAMP_INICIO_FOCO_1 VALOR_RODAL_USD_POR_HECTAREA_FOCO_1 MODELO_COMBUSTIBLE_1 DISTANCIA_A_CIUDAD_MAS_CERCANA_1 CANTIDAD_METROS_CONSTRUIDA_1
LATITUD_FOCO_2 LONGITUD_FOCO_2
HUMEDAD_RELATIVA_FOCO_2 VELOCIDAD_VIENTO_FOCO_2 DIRECCION_VIENTO_FOCO_2 TEMPERATURA_AMBIENTE_FOCO_2 PENDIENTE_FOCO_2 FACTOR_VPL_FOCO_2 TIMESTAMP_INICIO_FOCO_2 VALOR_RODAL_USD_POR_HECTAREA_FOCO_2 MODELO_COMBUSTIBLE_2 DISTANCIA_A_CIUDAD_MAS_CERCANA_2 CANTIDAD_METROS_CONSTRUIDA_2
...
LATITUD_FOCO_N LONGITUD_FOCO_N
HUMEDAD_RELATIVA_FOCO_N VELOCIDAD_VIENTO_FOCO_N DIRECCION_VIENTO_FOCO_N TEMPERATURA_AMBIENTE_FOCO_N PENDIENTE_FOCO_N FACTOR_VPL_FOCO_N TIMESTAMP_INICIO_FOCO_N VALOR_RODAL_USD_POR_HECTAREA_FOCO_N MODELO_COMBUSTIBLE_M DISTANCIA_A_CIUDAD_MAS_CERCANA_M  CANTIDAD_METROS_CONSTRUIDA_M
NUMERO_DE_RECURSOS
ID_RECURSO_1 TIPO_RECURSO_1 HORAS_DE_TRABAJO_RECURSO_1 LATITUD_RECURSO_1 LONGITUD_RECURSO_1 ID_ESTADO_RECURSO_1 ID_ASIGNADO_INCENDIO BOOL_ESTA_AGRUPADO_RECURSO_1 TIMESTAMP_A_INCENDIO_1 ... TIMESTAMP_A_INCENDIO_M
ID_RECURSO_2 TIPO_RECURSO_2 HORAS_DE_TRABAJO_RECURSO_2 LATITUD_RECURSO_2 LONGITUD_RECURSO_2 ID_ESTADO_RECURSO_2 ID_ASIGNADO_INCENDIO BOOL_ESTA_AGRUPADO_RECURSO_2 TIMESTAMP_A_INCENDIO_1 ... TIMESTAMP_A_INCENDIO_M
...
ID_RECURSO_N TIPO_RECURSO_N HORAS_DE_TRABAJO_RECURSO_N LATITUD_RECURSO_N LONGITUD_RECURSO_N ID_ESTADO_RECURSO_N ID_ASIGNADO_INCENDIO BOOL_ESTA_AGRUPADO_RECURSO_N TIMESTAMP_A_INCENDIO_1 ... TIMESTAMP_A_INCENDIO_M
---
*/

int convertirTipoRecurso(const std::string &s)
{
  if (s == "HELITRANSPORTADA")
    return 0;
  else if (s == "TERRESTRE")
    return 1;
  else if (s == "CISTERNA")
    return 2;
  else if (s == "MECANIZADA")
    return 3;
  else if (s == "Interfaz")
    return 4;
  else if (s == "CHINOOK")
    return 5;
  else if (s == "CISTERNAAEREA")
    return 6;
  else if (s == "SINTIPO")
    return 7;
  else
    return 7;
}

int convertirModeloCombustible(const std::string &s)
{
  if (s == "PCH1")
    return 1;
  else if (s == "PCH2")
    return 2;
  else if (s == "PCH3")
    return 3;
  else if (s == "PCH4")
    return 4;
  else if (s == "PCH5")
    return 5;
  else if (s == "MT01")
    return 6;
  else if (s == "MT02")
    return 7;
  else if (s == "MT03")
    return 8;
  else if (s == "MT04")
    return 9;
  else if (s == "MT05")
    return 10;
  else if (s == "MT06")
    return 11;
  else if (s == "MT07")
    return 12;
  else if (s == "MT08")
    return 13;
  else if (s == "BN01")
    return 14;
  else if (s == "BN02")
    return 15;
  else if (s == "BN03")
    return 16;
  else if (s == "BN04")
    return 17;
  else if (s == "BN05")
    return 18;
  else if (s == "PL01")
    return 19;
  else if (s == "PL02")
    return 20;
  else if (s == "PL03")
    return 21;
  else if (s == "PL04")
    return 22;
  else if (s == "PL05")
    return 23;
  else if (s == "PL06")
    return 24;
  else if (s == "PL07")
    return 25;
  else if (s == "PL08")
    return 26;
  else if (s == "PL09")
    return 27;
  else if (s == "PL10")
    return 28;
  else if (s == "PL11")
    return 29;
  else if (s == "DX01")
    return 30;
  else if (s == "DX02")
    return 31;
  else if (s == "numModelosCombustible")
    return 31;
  else
    return -1;
}

bool lecturaInicial(std::istream &inputStream, HolderRecursos &hRecursos, HolderIncendios &hIncendios,
                    HolderCiudadesCercanas &hCiudades, int verbose)
{
  int numIncendios;
  int numRecursos;
  std::string timestamp;

  infoOut << "Cargando Datos de incendio..." << std::endl;
  matrizETA &etas = matrizETA::get();
  std::vector<Incendio *> refIncendios;
  inputStream >> timestamp;
  inputStream >> numIncendios;

  time_t actual = stringToEpoch(timestamp);
  Parametros::get().setTimeStamp(actual);

  infoOut << "Número de incendios: " << numIncendios << std::endl
          << "latitud  longitud  humedad  rapidezViento  direccionViento  temperatura  pendiente  factorVPL  timestampInicio  valorRodalXHectarea  distanciaCiudad" << std::endl;

  // Lectura de Incendios!
  for (int i = 0; i < numIncendios; i++)
  {
    double latitud, longitud;
    double humedad;
    double rapidezViento;
    double direccionViento;
    double temperatura;
    double pendiente;
    double factorVPL;
    double valorRodalXHectarea;
    double distanciaCiudad;
    double metrosDeLineaArmados;
    time_t timeStampInicio;
    std::string modeloCombustible;
    int modCombustibleId;

    inputStream >> latitud >> longitud >> humedad >> rapidezViento >> direccionViento >> temperatura >> pendiente >> factorVPL >> timestamp >> valorRodalXHectarea >> modeloCombustible >> distanciaCiudad >> metrosDeLineaArmados;

    timeStampInicio = stringToEpoch(timestamp);
    modCombustibleId = convertirModeloCombustible(modeloCombustible);

    Incendio in(latitud, longitud, humedad, rapidezViento, direccionViento, temperatura, pendiente,
                factorVPL, timeStampInicio, valorRodalXHectarea, metrosDeLineaArmados, modCombustibleId, i);

    infoOut << latitud << " " << longitud << " " << humedad << " " << rapidezViento << " " << direccionViento << " " << temperatura << " " << pendiente
            << " " << factorVPL << " " << timestamp << " " << valorRodalXHectarea << " " << modeloCombustible << " " << distanciaCiudad << std::endl;

    hIncendios.agregarIncendio(in);
    // El leseo para vincular el incendio con la distancia, es porque utilizo la direccion de
    // memoria del incendio para hashear... entonces necesito la direccion del incendio guardado en el holder
    refIncendios.push_back(&hIncendios.getIncendio(hIncendios.getNumIncendios() - 1));
    hCiudades.agregarIncendioYCiudad(*refIncendios[refIncendios.size() - 1], distanciaCiudad);
  }

  inputStream >> numRecursos;
  infoOut << "Número de Recursos: " << numRecursos << std::endl;
  infoOut << "id\ttipo\thorasDeTrabajo\tlatitud\tlongitud\testado\tidAsignado\testaAgrupado\tETAs" << std::endl;

  // Lectura de Recursos
  std::vector<std::pair<int, int>> recursosYaCombatiendo;

  // int ignored = 0, i_i = 0;

  for (int i = 0; i < numRecursos; i++)
  {
    std::string id;
    std::string tipo;
    double horasDeTrabajo;
    double latitud, longitud;
    int estado;
    bool estaAgrupado;
    int idAsignado;
    std::string eta;
    int tipoR;

    inputStream >> id >> tipo >> horasDeTrabajo >> latitud >> longitud >>
        estado >> idAsignado >> estaAgrupado;

    // // Si no esta activo el Recurso, no se considera para el solver
    // if (estado==0) {
    //   infoOut<<"\t"<<id<<"\tIGNORADO"<<std::endl;
    //   // ignored++;
    //   numIncendios--;
    //   i--;
    //   for (int j=0; j<numIncendios; j++)
    //     inputStream >> eta;
    //   continue;
    // }

    tipoR = convertirTipoRecurso(tipo);
    hRecursos.agregarIdentificador(i, id);

    Recurso res(i, tipoR, horasDeTrabajo, latitud, longitud, estado, estaAgrupado, idAsignado);

    infoOut << "\t" << id << "\t" << tipoR << "\t" << horasDeTrabajo << "\t" << latitud << "\t" << longitud
            << "\t" << estado << "\t" << idAsignado << "\t" << estaAgrupado << "\t";

    for (int j = 0; j < numIncendios; j++)
    {
      inputStream >> eta;
      infoOutN << eta << " ";
      time_t etaTimestamp = stringToEpoch(eta);
      etas.setETA(res, hIncendios.getIncendio(j), etaTimestamp);
    }

    if (idAsignado != -1 && etas.getETA(res, hIncendios.getIncendio(idAsignado)) < actual)
      recursosYaCombatiendo.emplace_back(i, idAsignado);

    hRecursos.agregarRecurso(res);
    // i_i++;
  }

  // Lectura de tabla de rendimientos
  tablaRendimientos &tabla = tablaRendimientos::get();
  std::map<std::pair<int, int>, double> localMapTablaRendimiento;
  for (int i = 1; i < numModelosCombustible; i++)
  {
    // Enum de tipos de brigada -> numTipos
    for (int j = 0; j < sinTipo; j++)
    {
      double rendimiento;
      inputStream >> rendimiento;
      infoOut << "Lei rendimiento " << rendimiento << std::endl;
      localMapTablaRendimiento[std::make_pair(j, i)] = rendimiento;
    }
  }
  tabla.setTabla(localMapTablaRendimiento);

  // Lectura de gastos de Recursos por tipo
  std::map<int, std::pair<double, double>> localMapTablaCostos;
  for (int i = 0; i < sinTipo; i++)
  {
    double costoTransporte, costoUso;
    inputStream >> costoTransporte >> costoUso;
    infoOut << "Lei costo transporte " << costoTransporte << " y costo de uso " << costoUso << std::endl;
    localMapTablaCostos[i] = std::make_pair(costoTransporte, costoUso);
  }

  tablaCostosRecursos::get().setTabla(localMapTablaCostos);

  // Dejo esta parte fuera del código porque hay que terminar de ver como la recibimos

  // Obtención del singleton de Incompatibilidades
  Incompatibilidades &tablaIncompatibilidad = Incompatibilidades::get();

  // Obtenido del enum
  int numeroTiposRecurso = sinTipo;
  std::vector<std::vector<bool>> compatRR(numeroTiposRecurso + 1,
                                          std::vector<bool>(numeroTiposRecurso + 1, 0));

  for (int i = 0; i < numeroTiposRecurso; i++)
  {
    for (int j = 0; j < numeroTiposRecurso; j++)
    {
      // bool leer;
      // inputStream >> leer;
      compatRR[i][j] = true;
    }
  }

  tablaIncompatibilidad.setMatrizRecursoRecurso(compatRR);

  std::vector<std::vector<bool>> compatRI(numIncendios + 1,
                                          std::vector<bool>(numRecursos + 1, 0));
  infoOut << "Compatibilidades Recurso<->Incendio" << std::endl;
  infoOut << "idIncendio\tidRecurso\t" << std::endl;
  for (int i = 0; i < numIncendios; i++)
  {
    for (int j = 0; j < numRecursos; j++)
    {
      bool leer;
      int idIncendio;
      std::string idRecurso;
      inputStream >> idRecurso >> idIncendio >> leer;
      infoOut << idIncendio - 1 << "\t" << hRecursos.buscarIdentificadorInv(idRecurso) << "\t" << leer << std::endl;
      compatRI[idIncendio - 1][hRecursos.buscarIdentificadorInv(idRecurso)] = true;
    }
  }

  tablaIncompatibilidad.setMatrizRecursoIncendio(compatRI);

  // Calculo de los metros de linea ya construidos
  double subestima = Parametros::get().getSubestimacionLineaConstruida();
  for (auto &p : recursosYaCombatiendo)
  {
    Recurso &res = hRecursos.getRecurso(p.first);
    Incendio &in = hIncendios.getIncendio(p.second);

    double metrosActuales = in.getMetrosDeLineaArmadas();

    double rendimiento = tabla.getRendimiento(res, in);

    int segundosCombate = actual - etas.getETA(res, in);
    double horasCombate = (double)segundosCombate / 3600.0;
    infoOut << "Recurso " << res.getId() << " hace previamente " << rendimiento * horasCombate * subestima << " metros de linea en el incendio " << in.getId() << std::endl;
    in.setMetrosDeLineaArmadas(metrosActuales + rendimiento * horasCombate * subestima);
  }

  return true;
}

#endif

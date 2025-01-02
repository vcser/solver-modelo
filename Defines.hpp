#ifndef DEFINES_HPP
#define DEFINES_HPP

// sinTipo es para saber cuantos tipos hay
enum Brigada { Helitransportada = 0, Terrestre, Cisterna, Mecanizada, Interfaz, Chinook, CisternaAerea, sinTipo};
enum EstadoRecurso {inactivo, combatiendo, movilizandose, reposo};
enum VerboseTypes {none=0,resultados,inputs,iteraciones};
enum modeloCombustible{PCH1=1,PCH2,PCH3,PCH4,PCH5,MT01,MT02,MT03,MT04,MT05,MT06,
  MT07,MT08,BN01,BN02,BN03,BN04,BN05,PL01,PL02,PL03,PL04,PL05,PL06,PL07,PL08,
  PL09,PL10,PL11,DX01,DX02,numModelosCombustible};

#endif

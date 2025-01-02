#include "Kitral.hpp"
#include "Estructuras.hpp"
#include "Calculos.hpp"
#include "Managers.hpp"
#include "Predictor.hpp"

bool ejecutarSuiteTests(){

}

bool testKitral(){
  Kitral kit;

	// (latitud, longitud, humedad, rapidezviento, direccionviento, timestamp)
	Incendio i(0, 0, 30, 30, 30, 30, 0);

	std::vector<int> timeStamps = {1,2,6,12,24};

	for (auto &t : timeStamps) {
		Perimetro p = kit.consultarPerimetro(i,t);
		Area a = kit.consultarArea(i,t);

		std::cout << "t: " << t << "\t\t:" << a.getArea() << "\t\t" << p.getPerimetro() << std::endl;
	}

	std::cout << "funcionando." << std::endl;	
}

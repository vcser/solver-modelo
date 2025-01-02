/*
TODO:

DONE:
	Construir una funcion que transforme de longitud & latitud al id del modelo del combustible ===> Modificacion en Incendio Class
	 //                   ...                             //   a la pendiente presente ===> Modificacion en Incendio class
	Test it ===> Done
*/

#include "Kitral.hpp"

const double Kitral::_ratioElipses[] = {1.5, 1.6, 1.7, 1.9, 2.0};


void Kitral::_procesaNuevoIncendio(const Incendio &i) {
	double pendiente = i.getPendiente();

	double contenidoHumedad = 				// Cuentas!B2
				-2.97374+(0.262*i.getHumedad())-(0.00982*i.getTemperatura())-1;
	double factorContenidoHumedad = 		// Cuentas!B3
				(389.1624-(14.3*contenidoHumedad)+(0.2*contenidoHumedad*contenidoHumedad))/(3.559+(1.6615*contenidoHumedad)+(2.62392*contenidoHumedad*contenidoHumedad));
	double factorViento = 					// Cuentas!B4
				1+(0.51218*i.getRapidezViento())-(0.007151*i.getRapidezViento()*i.getRapidezViento());
	double factorPendiente = 				// Cuentas!B5
				pendiente*pendiente*0.00013585+pendiente*0.023322+1;
	double factorCombustible = 			    // Cuentas!B6
				i.getFactorVPL();

	// Velocidad en km/h
	double velocidadPropagacion = factorContenidoHumedad*factorCombustible*(factorViento+factorPendiente)*60*60/1000;

	double perimetroPorHora = 0;
	double areaPorHora = 0;

	for (int j=0; j<RATIO_N; j++) {
		double ratio = _ratioElipses[j];
		double elipsoide = sqrt(1-(1/(ratio*ratio)));
		double Ea = ((velocidadPropagacion)+velocidadPropagacion*(1-elipsoide)/(1+elipsoide))/2;
		double Eb = Ea/ratio;

		perimetroPorHora += M_PI*((3*(Ea+Eb))-(sqrt(((3*Ea)+Eb)*(Ea+(3*Eb)))))*1000;
		areaPorHora += M_PI*Ea*Eb*100;
	}

	perimetroPorHora /= RATIO_N;
	areaPorHora /= RATIO_N;

	_incendiosProcesados[i.getId()] = std::make_pair(Perimetro(perimetroPorHora),Area(areaPorHora));
}


Kitral::~Kitral() {
	_incendiosProcesados.clear();
}


Perimetro Kitral::consultarPerimetro(const Incendio &i, double tiempo) {
	if (_incendiosProcesados.find(i.getId()) == _incendiosProcesados.end())
		_procesaNuevoIncendio(i);

	return Perimetro(_incendiosProcesados[i.getId()].first.getPerimetro()*tiempo);
}


Area Kitral::consultarArea(const Incendio &i, double tiempo) {
	if (_incendiosProcesados.find(i.getId()) == _incendiosProcesados.end())
		_procesaNuevoIncendio(i);

	return Area(_incendiosProcesados[i.getId()].second.getArea()*tiempo);
}


double Kitral::consultarCosto(const Incendio &i, double tiempo) {
	if (_incendiosProcesados.find(i.getId()) == _incendiosProcesados.end())
		_procesaNuevoIncendio(i);

	return _incendiosProcesados[i.getId()].second.getArea()*tiempo*i.getValorRodalXHectarea();
}

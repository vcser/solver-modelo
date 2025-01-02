#ifndef KITRAL_HPP
#define KITRAL_HPP

#define _USE_MATH_DEFINES

#include <string>
#include <unordered_map>
#include <utility>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Estructuras.hpp"
#include "Predictor.hpp"

#define RATIO_N 5

class Kitral : public Predictor {
private:
	static const double _ratioElipses[RATIO_N];

protected:
	std::unordered_map<int,std::pair<Perimetro,Area> > _incendiosProcesados;
	void _procesaNuevoIncendio(const Incendio &i);

public:
	Kitral(){};
	~Kitral();

	Perimetro consultarPerimetro(const Incendio &i, double tiempo);
	Area consultarArea(const Incendio &i, double tiempo);
	double consultarCosto(const Incendio &i, double tiempo);
};

#endif

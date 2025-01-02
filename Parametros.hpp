#ifndef PARAMETROS_HPP
#define PARAMETROS_HPP

#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <istream>
#include <fstream>
#include <stdexcept>
#include "Parsing.hpp"

#include "Managers.hpp"

// Lista de numeros magicos
#define SUB_LINEA_CONST 0.5

// Defines para GreedyBaseline
#define MARGEN_DE_SEGURIDAD 2.0
#define N_PERTURBACIONES 2

// Defines de CommandLine
#define DEFAULT_NAME "default.txt"

// Default predictor (0=Kitral, 1=RedNeuronal)
#define PREDICTOR_ID 0


#define SOLVER_INF 1E12
#define CONST6HORAS 21600	// No le veo sentido a poner este en la clase Parametros...

#define TIME_STAMP_FORMAT "%Y-%m-%d,%H:%M"

#define TIME_LIMIT 1.0

#define GREEDY_ID 1

#define CONFIG_FILE "ParametrosDefault"

#define HORAS_CALCULO_AFINIDAD 1.0
/*
En kitral hay un parametro que tiene que ver con los elipses, habria que considerar quizas la posiblidad de
modificar la lista de elipses y la cantidad de elipses que se utilizan para calcular las propagaciones de fuego.
*/


class Parametros {
public:
	static Parametros & get() {
		static Parametros instance;
		return instance;
	}

	Parametros (const Parametros   &) = delete;
	void operator=(const Parametros &) = delete;

	// getters
	double getSubestimacionLineaConstruida() { return _subestimacionLineaConstruida; }
	double getMargenDeSeguridadCiudad() { return _margenDeSeguridadCiudad; }
	int getNPerturbaciones() { return _nPerturbaciones; }
	std::string getDefaultFileName() { return _defaultFileName; }
	double getSolverInf() { return _solverInf; }
	int getPredictorId() { return _predictorId; }
	time_t getTimeStamp() { return _timeStamp; }
	std::string getTimeStampFormat() { return _timeStampFormat; }
	int getGreedyId() { return _greedyId; }
	double getTimeLimit() { return _timeLimit; }
	double getHorasCalculoAfinidad() { return _horasCalculoAfinidad; }

	// setters
	void setSubestimacionLineaConstruida(double subestima) { _subestimacionLineaConstruida = subestima; }
	void setMargenDeSeguridadCiudad(double margen) { _margenDeSeguridadCiudad = margen; }
	void setNPerturbaciones(int n) { _nPerturbaciones = n; }
	// No hay setter para el nombre default, ya que para eso mejor le pones el nombre del archivo por consola...
	void setSolverInf(double inf) { _solverInf = inf; }
	void setPredictorId(int id) { _predictorId = id; }
	void setTimeStamp(time_t timestamp) { _timeStamp = timestamp; }
	void setTimeStamp(const std::string &timestamp_formated);
	void setTimeStampFormat(const std::string &format) { _timeStampFormat = format; }
	void setGreedyId(int id) { _greedyId = id; }
	void setTimeLimit(double t) { _timeLimit = t; }
	void setHorasCalculoAfinidad(double h) { _horasCalculoAfinidad = h; }

	void loadConfigurationFromFile(const std::string &filePath);
	void loadConfigurationFromStream(std::istream &input);

	bool loadParameterFromKey(const std::string &key, const std::string &value);

private:
	std::map<std::string,int> mapper = {
		std::make_pair("-subest", 0),
		std::make_pair("-margenSeguridad", 1),
		std::make_pair("-nPerturb", 2),
		std::make_pair("-defFile", 3),
		std::make_pair("-solInf", 4),
		std::make_pair("-predictor", 5),
		std::make_pair("-timeStamp", 6),
		std::make_pair("-greedyId", 7),
		std::make_pair("-timeLimit", 8),
		std::make_pair("-timeFormat", 9),
		std::make_pair("-horasAfinidad", 10),
};

	Parametros();

	double _subestimacionLineaConstruida = SUB_LINEA_CONST;
	double _margenDeSeguridadCiudad = MARGEN_DE_SEGURIDAD;
	int _nPerturbaciones = N_PERTURBACIONES;
	std::string _defaultFileName = DEFAULT_NAME;
	double _solverInf = SOLVER_INF;
	int _predictorId = PREDICTOR_ID;
	time_t _timeStamp = time(NULL);
	std::string _timeStampFormat = TIME_STAMP_FORMAT;
	int _greedyId = GREEDY_ID;
	double _timeLimit = TIME_LIMIT;
	double _horasCalculoAfinidad = HORAS_CALCULO_AFINIDAD;
};


#endif

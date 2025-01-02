#include "Parametros.hpp"

Parametros::Parametros(){
	loadConfigurationFromFile(std::string(CONFIG_FILE));
};


void Parametros::setTimeStamp(const std::string &timestamp_formated) {
	std::tm t = {};
	std::istringstream ss(timestamp_formated);
	ss >> std::get_time(&t, _timeStampFormat.c_str());

	if (ss.fail())
		throw std::runtime_error("TimeStamp parse failed.\n");

	_timeStamp = std::mktime(&t);
}

void Parametros::loadConfigurationFromFile(const std::string &filePath) {
	std::ifstream file;
	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error al leer desde archivo.\n");
	loadConfigurationFromStream(file);
	file.close();
}


void Parametros::loadConfigurationFromStream(std::istream &input) {
	std::string key, value;

	while (input >> key >> value)
		if (!loadParameterFromKey(key, value))
			throw std::runtime_error("Keyword no encontrada, no se pudo inicializar una variable de Parametros.hpp.\n");
}


bool Parametros::loadParameterFromKey(const std::string &key, const std::string &value) {
	auto found = mapper.find(key);

	if (found == mapper.end())
		return false;

	int k = found->second;

	switch (k) {
		case 0:
		_subestimacionLineaConstruida = std::stod(value);
		break;

		case 1:
		_margenDeSeguridadCiudad = std::stod(value);
		break;

		case 2:
		_nPerturbaciones = std::stoi(value);
		break;

		case 3:
		_defaultFileName = value;
		break;

		case 4:
		_solverInf = std::stod(value);
		break;

		case 5:
		_predictorId = std::stoi(value);
		break;

		// case 6
		case 6:
		_timeStamp = formatedStringToTime_tWithFormat(value,_timeStampFormat);
		break;

		case 7:
		_greedyId = std::stoi(value);
		break;

		case 8:
		_timeLimit = std::stod(value);
		break;

		case 9:
		_timeStampFormat = value;
		break;
		case 10:
		_horasCalculoAfinidad = std::stod(value);
		break;

		// default:
		// throw std::runtime_error("Llave para parametro no encontrada");
		// break;
	}

	return true;
}

#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

#include "Parametros.hpp"

// Convertir un timestamp en ISO 8601 a epoch
time_t stringToEpoch(const std::string &timestamp);

time_t formatedStringToTime_t(const std::string &timestamp_formated);
time_t formatedStringToTime_tWithFormat(const std::string &timestamp_formated, const std::string &format);

// Convertir un epoch a timestamp en ISO 8601
std::string epochToString(const time_t epoch);



#endif

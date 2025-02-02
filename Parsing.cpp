#include "Parsing.hpp"
#include <iostream>

time_t stringToEpoch(const std::string &timestamp)
{
  struct tm t = {0};
  int y, M, d, h, m;
  float s{0.0};
  sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);

  // Redondear los segundos
  t.tm_year = y - 1900;
  t.tm_mon = M - 1;
  t.tm_mday = d;
  t.tm_hour = h;
  t.tm_min = m;
  t.tm_sec = static_cast<int>(s); // Redondear o truncar la parte decimal

  printf("%d-%d-%dT%d:%d:%fZ\n", y, M, d, h, m, s);

  // Asegurarse de que la conversión a epoch sea válida
  time_t epoch = timegm(&t);

  if (epoch == -1)
  {
    std::cerr << "Error: timegm() failed to convert time." << std::endl;
    return -1; // Retornar un valor de error si la conversión falla
  }

  std::cout << "Conversion: " << timestamp << " -> " << epoch << std::endl;

  return epoch;
}

time_t formatedStringToTime_t(const std::string &timestamp_formated)
{
  std::tm t = {};
  std::istringstream ss(timestamp_formated);
  ss >> std::get_time(&t, Parametros::get().getTimeStampFormat().c_str());

  if (ss.fail())
    throw std::runtime_error("TimeStamp parse failed.\n");

  return std::mktime(&t);
}

time_t formatedStringToTime_tWithFormat(const std::string &timestamp_formated, const std::string &format)
{
  std::tm t = {};
  std::istringstream ss(timestamp_formated);
  ss >> std::get_time(&t, format.c_str());

  if (ss.fail())
    throw std::runtime_error("TimeStamp parse failed.\n");

  return std::mktime(&t);
}

std::string epochToString(const time_t epoch)
{
  char buffer[70];
  time_t t = epoch;
  struct tm convTime = {0};

  // gmtime() devuelve un puntero, por lo que debes dereferenciarlo al asignar
  struct tm *tmpTime = gmtime(&t);
  if (tmpTime == nullptr)
  {
    // Obtén información adicional sobre el error
    std::cerr << "Error: gmtime() failed to convert epoch time." << std::endl;
    std::cerr << "Epoch value: " << epoch << std::endl;
    return "Invalid date"; // O el mensaje que desees para un error
  }

  convTime = *tmpTime; // Asigna el valor de la estructura tm a convTime

  // Usa snprintf para evitar desbordamientos de buffer
  snprintf(buffer, sizeof(buffer), "%d-%d-%dT%d:%d:%dZ",
           convTime.tm_year + 1900,
           convTime.tm_mon + 1,
           convTime.tm_mday,
           convTime.tm_hour,
           convTime.tm_min,
           convTime.tm_sec);

  return std::string(buffer);
}

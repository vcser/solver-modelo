#include "Parsing.hpp"

time_t stringToEpoch(const std::string &timestamp){
  struct tm t={0};
  int y,M,d,h,m;
    float s{0.0};
  sscanf(timestamp.c_str(), "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);
  t.tm_year = y-1900;
  t.tm_mon=M-1;
  t.tm_mday=d;
  t.tm_hour=h;
  t.tm_min=m;
  t.tm_sec=(int)s;
  printf("%d-%d-%dT%d:%d:%fZ\n", y, M, d, h, m, s);
  time_t epoch = timegm(&t);
  return epoch;
}

time_t formatedStringToTime_t(const std::string &timestamp_formated) {
  std::tm t = {};
  std::istringstream ss(timestamp_formated);
  ss >> std::get_time(&t, Parametros::get().getTimeStampFormat().c_str());

  if (ss.fail())
    throw std::runtime_error("TimeStamp parse failed.\n");

  return std::mktime(&t);
}


time_t formatedStringToTime_tWithFormat(const std::string &timestamp_formated, const std::string &format) {
  std::tm t = {};
  std::istringstream ss(timestamp_formated);
  ss >> std::get_time(&t, format.c_str());

  if (ss.fail())
    throw std::runtime_error("TimeStamp parse failed.\n");

  return std::mktime(&t);
}


std::string epochToString(const time_t epoch){
  char buffer[70];
  time_t t = epoch;
  struct tm convTime={0};

  convTime = *gmtime(&t);
  sprintf(buffer, "%d-%d-%dT%d:%d:%dZ" ,convTime.tm_year+1900, convTime.tm_mon+1,
      convTime.tm_mday, convTime.tm_hour, convTime.tm_min, convTime.tm_sec);

  return std::string(buffer);
}

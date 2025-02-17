#ifndef ESCRITURA_HPP
#define ESCRITURA_HPP

#include <vector>
#include <ostream>
#include <nlohmann/json.hpp>
#include "Managers.hpp"
#include "Solver.hpp"

using json = nlohmann::json;

bool escrituraFinal(std::ostream &outputStream, Solver &solver, Solucion *sol);
void escrituraAsignaciones(json &output, Solver &solver, Solucion *solucion);

#endif

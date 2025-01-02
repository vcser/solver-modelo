#ifndef ESCRITURA_HPP
#define ESCRITURA_HPP

#include <vector>
#include <ostream>
#include "Managers.hpp"
#include "Solver.hpp"

bool escrituraFinal(const std::string &outputFile, Solver &solver, Solucion *sol);
void escrituraAsignaciones(std::ostream &outputStream, Solver &solver, Solucion *solucion);

#endif

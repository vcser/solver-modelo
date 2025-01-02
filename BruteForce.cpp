#include "BruteForce.hpp"
#include "AnytimeRecorder.hpp"

bool BruteForce::next_assignation() {
	Incompatibilidades &tablaIncompatibilidad = Incompatibilidades::get();
	int nRecursos = hRecursos->getNumRecursos();
	int nIncendios = hIncendios->getNumIncendios();

	bool factible = false;

	while (!factible) {
		std::string asignacion = "Asignacion: ";
		int i;
		for (i=0; i<nRecursos; i++) {
			// Se cambia asignacion de un recurso y se actualiza la solucion
			_permutacionAsignaciones[i] =((_permutacionAsignaciones[i]+2)%(nIncendios+1))-1;;
			sol->asignarAIncendio(i,_permutacionAsignaciones[i]);

			if (_permutacionAsignaciones[i] != -1)
				break;
		}

		for (auto &incendio : _permutacionAsignaciones)
			asignacion += std::to_string(incendio) + " ";
		infoOut << asignacion << std::endl;

		// En caso de pasar por todas las permutaciones
		if (i==nRecursos) return false;

		// Se revisan las incompatibilidades
		Recurso &recursoI = hRecursos->getRecurso(i);
		Incendio &incendio =  hIncendios->getIncendio(_permutacionAsignaciones[i]);

		factible = tablaIncompatibilidad.compatibleRI(recursoI, incendio);

		if (factible)
			for(int k=0; factible && k<nRecursos; k++){
				if (_permutacionAsignaciones[i]==_permutacionAsignaciones[k]) {
					Recurso &recursoK = hRecursos->getRecurso(_permutacionAsignaciones[k]);
					// Si no es compatible con alguno anteriormente asignado...
					factible = tablaIncompatibilidad.compatibleRR(recursoK, recursoI);
				}
			}
	}

	return true;
}

void BruteForce::solve() {
	int nRecursos = hRecursos->getNumRecursos();

	// Primera permutacion es sin asignar
	_permutacionAsignaciones = std::vector<int> (nRecursos, -1);
	std::vector<int> mejorAsignacion(nRecursos);

	double bestFitness = -Parametros::get().getSolverInf();
	double currentFitness;

	do {
		currentFitness = calcularFitness(sol);
		infoOut << "ahorro actual: " << currentFitness << std::endl;
		infoOut << "mejor ahorro: " <<bestFitness << std::endl;

		if (currentFitness > bestFitness) {
			bestFitness = currentFitness;
      AnytimeRecorder::get().agregarMedicion(clock(), bestFitness);
			std::memcpy(mejorAsignacion.data(),_permutacionAsignaciones.data(), nRecursos*sizeof(int));
			infoOut << "Mejor solucion encontrada, ahorro: " << currentFitness << std::endl; 

			std::string info = "Asignaciones: ";
			for (int &incendio : _permutacionAsignaciones)
				info +=  std::to_string(incendio) + " ";
			infoOut << info << std::endl;
		}

	} while (next_assignation());

	// Copiamos la mejor asignacion a la solucion del solver
	for (int i=0; i<nRecursos; i++)
		sol->asignarAIncendio(i,mejorAsignacion[i]);

	std::memcpy(_permutacionAsignaciones.data(),mejorAsignacion.data(), nRecursos*sizeof(int));

	fitness = bestFitness;
}


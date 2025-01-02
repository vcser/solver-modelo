
#include <vector>
#include <ostream>
#include "Managers.hpp"
#include "Solver.hpp"
#include "Escritura.hpp"
#include "Logging.hpp"
#include "Parsing.hpp"

bool escrituraFinal(const std::string &outputFile, Solver &solver, Solucion *sol) {
	std::ofstream file;
	file.open(outputFile.c_str());
	if (!file.is_open())
		return false;

	escrituraAsignaciones(file, solver, sol);

	file.close();

	return true;
}

void escrituraAsignaciones(std::ostream &outputStream, Solver &solver, Solucion *solucion) {
	ManagerEstructuras &manager = ManagerEstructuras::get();
	HolderRecursos &hRecursos = manager.getHolderRecursos();
	HolderIncendios &hIncendios = manager.getHolderIncendios();

	tablaCostosRecursos &costosRecursos = tablaCostosRecursos::get();
	matrizETA &etas = matrizETA::get();

	int nRecursos = hRecursos.getNumRecursos();
	int nIncendios = hIncendios.getNumIncendios();

	// // Iterar sobre los recursos: id recurso -> incendio asignado;
	// for (int recurso=0; i<nRecursos; recurso++) {
	// 	int incendio = solucion.asignadoAIncendio(i);

	// 	outputStream << recurso << " -> " << incendio << "\n";
	// }

	// Iteracion sobre la asignacion a los incendios:
	// Incendio x:
	//	recursos ...
	time_t actual = Parametros::get().getTimeStamp();
	for (int idIncendio=0; idIncendio<nIncendios; idIncendio++) {
		outputStream << "Incendio " << idIncendio << "\n";
		Incendio &incendio = hIncendios.getIncendio(idIncendio);
		infoOut <<"Incendio "<<idIncendio<< " timestamp inicio \t"<< epochToString(incendio.getTimestampInicio()) << std::endl;
		std::vector<int> idRecursosAsignados;

		for (int idRecurso=0; idRecurso<nRecursos; idRecurso++) {
			if (solucion->asignadoAIncendio(idRecurso) == idIncendio)
				idRecursosAsignados.push_back(idRecurso);
		}
		//TODO: Que pasa aca cuando no se apaga el incendio?
		time_t tiempoApagado = solver.apagaIncendio(idRecursosAsignados, incendio);
		infoOut <<"Incendio " << idIncendio<< " timestamp apagado \t"<< epochToString(tiempoApagado) << std::endl;
		int segundosCombate = tiempoApagado-actual;
		double horasDeCombate = (double)segundosCombate/3600.0;
		outputStream << "Tiempo Apagado (h) " << horasDeCombate << "\n";
		// Ver bien como sacar una referencia al predictor para recibir estos datos
		// const Predictor *predictor = solver.getPredictor();
		// De momento lo pasé a tener métodos en el solver.
		outputStream << "AreaQuemada  " << solver.consultarArea(incendio, horasDeCombate).getArea() << "\n";
		outputStream << "Perimetro  " << solver.consultarPerimetro(incendio, horasDeCombate).getPerimetro() << "\n";
		outputStream << "PatrimonioUSD  " << std::fixed << solver.consultarCosto(incendio, horasDeCombate) << "\n";
		outputStream << "PatrimonioUSD Salvado " << std::fixed << solver.consultarCosto(incendio, 6-horasDeCombate) << "\n";

		// Lo querran ordenado de mayor a menor?
		outputStream << "idRecurso\tmetroLinea\tcostoRecurso\n";
		for (auto &idRecurso:idRecursosAsignados) {
			Recurso &recurso = hRecursos.getRecurso(idRecurso);
			double metroLinea = solver.metrosDeLinea(recurso, incendio,tiempoApagado);
			time_t recursoETA = etas.getETA(recurso, incendio);
			int segundosETA =  (actual < recursoETA) ? recursoETA-actual : 0;
			double horasETA = segundosETA/3600.0;
			double costoRecurso = horasETA*costosRecursos.getCostoTransporte(recurso.getTipo()) + (horasDeCombate-horasETA) 
				* costosRecursos.getCostoUso(recurso.getTipo());
			outputStream << "\t" << hRecursos.buscarIdentificador(idRecurso) << "  " << metroLinea << " " << costoRecurso << "\n";
		}
		outputStream << "\n";
	}

	outputStream << "No se utiliza Recurso Id:\n";
	for (int recurso=0; recurso<nRecursos; recurso++) {
		if (solucion->asignadoAIncendio(recurso)==-1)
			outputStream << "\t" << hRecursos.buscarIdentificador(recurso) << "\n";
	}
}

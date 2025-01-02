#include <iostream>
#include <fstream>
#include <utility>
#include <cstdlib>
#include "CommandLine.hpp"
#include "Lectura.hpp"
#include "Logging.hpp"
#include "Escritura.hpp"
#include "Managers.hpp"
#include "HolderSolver.hpp"
#include "AnytimeRecorder.hpp"

int main(int argc, char **argv)
{
	srand(1);
	try
	{
		Commandline c(argc, argv);
		infoOut << "Iniciando solver " << std::endl;
		if (!c.is_valid())
			throw std::string("No se ha podido leer la línea de comandos, Uso: ./solver -i input_path [-o output_path] [-v verbose] [-s solver]");

		infoOut << "El programa ha comenzado..." << std::endl;
		ManagerEstructuras &manager = ManagerEstructuras::get();
		Solucion *sol;
		HolderRecursos &hRecursos = manager.getHolderRecursos();
		HolderIncendios &hIncendios = manager.getHolderIncendios();
		HolderCiudadesCercanas &hCiudades = manager.getHolderCiudadesCercanas();
		if (c.get_lecturaPorLineaComando())
		{
			if (!lecturaInicial(std::cin, hRecursos, hIncendios, hCiudades, c.get_verbose()))
				throw std::string("Error al leer desde terminal");
		}
		else
		{
			std::ifstream file;
			file.open(c.get_inputPath().c_str());
			if (!file.is_open())
				throw std::string("Error al leer desde archivo");
			lecturaInicial(file, hRecursos, hIncendios, hCiudades, c.get_verbose());
			file.close();
		}
		sol = new Solucion();
		// AnytimeRecorder::get().agregarMedicion(clock(), 0);
		switch (Parametros::get().getGreedyId())
		{
		case 0:
			infoOut << "Utilizamos heuristica greedy por distancia." << std::endl;
			HolderSolver::setAsGreedyBaseline(&hIncendios, &hCiudades, &hRecursos, sol);
			break;

		case 1:
			infoOut << "Utilizamos heuristica GRASP" << std::endl;
			HolderSolver::setAsRandomizedGreedy(&hIncendios, &hCiudades, &hRecursos, sol);
			break;

		case 2:
			infoOut << "Utilizamos heuristica greedy por afinidad." << std::endl;
			HolderSolver::setAsGreedyPorAfinidad(&hIncendios, &hCiudades, &hRecursos, sol);
			break;

		case 3:
			infoOut << "Utilizamos fuerza bruta." << std::endl;
			HolderSolver::setAsBruteForce(&hIncendios, &hCiudades, &hRecursos, sol);
			break;

		default:
			throw std::runtime_error("Greedy no identificado (0-2).");
		}

		Solver &solver = HolderSolver::getSolver();
		solver.solve();
		// GRASP
		if (Parametros::get().getGreedyId() == 1)
		{
			Solucion *best = new Solucion(*sol);
			double bestFitness = solver.calcularFitness(sol), fitness;
			// Medicion 1
			AnytimeRecorder::get().agregarMedicion(clock(), solver.getFitness());
			clock_t inicio = clock();

			do
			{
				std::cout << bestFitness << std::endl;
				solver.pasoBusquedaLocal();
				fitness = solver.calcularFitness(sol);

				if (fitness > bestFitness)
				{
					best->copy(*sol);
					bestFitness = fitness;
				}
				solver.solve();
			} while ((double)(clock() - inicio) / CLOCKS_PER_SEC < Parametros::get().getTimeLimit());

			sol->copy(*best);
		}
		else if (Parametros::get().getGreedyId() <= 2)
		{
			infoOut << " El fitness de la solucion inicial es " << solver.getFitness() << std::endl;

			// Medicion 1
			AnytimeRecorder::get().agregarMedicion(clock(), solver.getFitness());

			clock_t inicio = clock();
			int numIteraciones = 0;
			escrituraAsignaciones(std::cerr, solver, sol);

			// while( (double)(clock()-inicio)/CLOCKS_PER_SEC<Parametros::get().getTimeLimit() ){
			// 	if( !solver.pasoBusquedaLocal() ){
			// 		//Si no se consigue mejorar, nos detenemos
			// 		break;
			// 	}
			// 	numIteraciones++;
			// }

			debugOut << " Despues de " << numIteraciones << " iteraciones, la solucion tiene " << "un fitness de " << solver.getFitness() << std::endl;
			escrituraAsignaciones(std::cerr, solver, sol);
		}

		// output
		if (!escrituraFinal(c.get_outputPath().c_str(), solver, sol))
			throw std::string("Error al crear archivo de salida.");

		delete sol;
		infoOut << "The program has finished" << std::endl;
		std::ofstream anytimeSteps("salidaAnytime.txt");
		AnytimeRecorder::get().imprimirEnStream(anytimeSteps);
		anytimeSteps.close();
	}
	catch (const std::string &e)
	{
		infoOut << e << std::endl;
	}
	return 0;
}

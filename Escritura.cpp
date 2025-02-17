#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Managers.hpp"
#include "Solver.hpp"
#include "Escritura.hpp"
#include "Logging.hpp"
#include "Parsing.hpp"

using json = nlohmann::json;

bool escrituraFinal(std::ostream &outputStream, Solver &solver, Solucion *sol) {
    json outputData;

    // std::ofstream file(outputFile.c_str());
    // if (!file) {
    //     std::cerr << "Error creando el archivo de salida" << std::endl;
    //     return false;
    // }

    // Verificar si los objetos de entrada son válidos antes de escribir
    if (sol == nullptr) {
        std::cerr << "Datos no válidos para la escritura." << std::endl;
        return false;
    }

    escrituraAsignaciones(outputData, solver, sol);

    // Volcar los datos en el archivo JSON
    outputStream << outputData.dump(2);

    return true;
}

void escrituraAsignaciones(json &output, Solver &solver, Solucion *solucion) {
    ManagerEstructuras &manager = ManagerEstructuras::get();
    HolderRecursos &hRecursos = manager.getHolderRecursos();
    HolderIncendios &hIncendios = manager.getHolderIncendios();

    tablaCostosRecursos &costosRecursos = tablaCostosRecursos::get();
    matrizETA &etas = matrizETA::get();

    int nRecursos = hRecursos.getNumRecursos();
    int nIncendios = hIncendios.getNumIncendios();

    time_t actual = Parametros::get().getTimeStamp();

    output["fires"] = json::array();

    for (int idIncendio = 0; idIncendio < nIncendios; idIncendio++) {
        json incendio_json;
        incendio_json["id"] = idIncendio;
        Incendio &incendio = hIncendios.getIncendio(idIncendio);
        infoOut << "Incendio " << idIncendio << " timestamp inicio \t" << epochToString(incendio.getTimestampInicio()) << std::endl;

        std::vector<int> idRecursosAsignados;
        for (int idRecurso = 0; idRecurso < nRecursos; idRecurso++) {
            if (solucion->asignadoAIncendio(idRecurso) == idIncendio)
                idRecursosAsignados.push_back(idRecurso);
        }

        time_t tiempoApagado = solver.apagaIncendio(idRecursosAsignados, incendio);

        double horasDeCombate = 0.0;
        if (tiempoApagado == std::numeric_limits<time_t>::max()) {
            infoOut << "Incendio " << idIncendio << " no pudo ser apagado.\n";
            incendio_json["metrics"] = {
                {"extinguishedTime", 0},
                {"area", 0},
                {"perimeter", 0},
                {"damage", 0},
                {"savedDamage", 0}
            };
        } else {
            infoOut << "Incendio " << idIncendio << " timestamp apagado \t" << epochToString(tiempoApagado) << std::endl;

            int segundosCombate = tiempoApagado - actual;
            horasDeCombate = static_cast<double>(segundosCombate) / 3600.0;
            incendio_json["metrics"] = {
                {"extinguishedTime", horasDeCombate},
                {"area", solver.consultarArea(incendio, horasDeCombate).getArea()},
                {"perimeter", solver.consultarPerimetro(incendio, horasDeCombate).getPerimetro()},
                {"damage", solver.consultarCosto(incendio, horasDeCombate)},
                {"savedDamage", solver.consultarCosto(incendio, 6 - horasDeCombate)}
            };
        }

        // Recursos asignados al incendio
        incendio_json["resources"] = json::array();
        for (auto &idRecurso : idRecursosAsignados) {
            Recurso &recurso = hRecursos.getRecurso(idRecurso);
            double metroLinea = solver.metrosDeLinea(recurso, incendio, tiempoApagado);
            time_t recursoETA = etas.getETA(recurso, incendio);
            int segundosETA = (actual < recursoETA) ? recursoETA - actual : 0;
            double horasETA = segundosETA / 3600.0;
            double costoRecurso = horasETA * costosRecursos.getCostoTransporte(recurso.getTipo()) + (horasETA > 0 ? (horasETA - horasDeCombate) : 0) * costosRecursos.getCostoUso(recurso.getTipo());

            json recurso_json;
            recurso_json["id"] = hRecursos.buscarIdentificador(idRecurso);
            recurso_json["line"] = metroLinea;
            recurso_json["cost"] = costoRecurso;
            incendio_json["resources"].push_back(recurso_json);
        }

        output["fires"].push_back(incendio_json);
    }

    // Recursos no utilizados
    output["notUsed"] = json::array();
    for (int recurso = 0; recurso < nRecursos; recurso++) {
        if (solucion->asignadoAIncendio(recurso) == -1)
            output["notUsed"].push_back(hRecursos.buscarIdentificador(recurso));
    }
}

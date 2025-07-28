package web

import (
	"encoding/json"
	"html/template"
	"log"
	"net/http"
	"os"
	"recomendador-despacho/solver"
	"strconv"
	"time"
)

func cargarRecursos() solver.Recursos {
	file, err := os.Open("data/recursos.json")
	if err != nil {
		log.Fatalf("Error leyendo recursos: %v\n", err)
	}
	defer file.Close()

	var content []solver.Recurso
	decoder := json.NewDecoder(file)
	decoder.Decode(&content)

	return content
}

func cargarRendimientos() solver.Rendimientos {
	file, err := os.Open("data/rendimientos.json")
	if err != nil {
		log.Fatalf("Error leyendo recursos: %v\n", err)
	}
	defer file.Close()

	var content solver.Rendimientos
	decoder := json.NewDecoder(file)
	decoder.Decode(&content)

	return content
}

var recursos = cargarRecursos()
var rendimientos = cargarRendimientos()

func IndexHandler(w http.ResponseWriter, r *http.Request) {
	tmpl, err := template.ParseFiles("web/templates/index.html")
	if err != nil {
		http.Error(w, "Error al cargar la plantilla", http.StatusInternalServerError)
		log.Println("Error al parsear index.html", err)
		return
	}
	tmpl.Execute(w, nil)
}

func SolveHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Metodo http no permitido", http.StatusMethodNotAllowed)
		return
	}

	latString := r.FormValue("lat")
	lonString := r.FormValue("lon")

	log.Println("Latitud:", latString, "Longitud:", lonString)

	lat, err := strconv.ParseFloat(latString, 64)
	if err != nil {
		http.Error(w, "Latitud invalida", http.StatusBadRequest)
		log.Println("Error al parsear latitud", lat)
		return
	}

	lon, err := strconv.ParseFloat(lonString, 64)
	if err != nil {
		http.Error(w, "Longitud invalida", http.StatusBadRequest)
		log.Println("Error al parsear longitud", lon)
		return
	}

	// Una vez parseadas correctamente las entradas se puede generar una recomendacion
	timestamp := time.Now().Add(-15 * time.Minute)
	incendio := solver.NewIncendio(lat, lon, timestamp)

	s := solver.Solver{Timestamp: time.Now()}
	recursos.CalcularETAs(incendio)
	recursos.CalcularRendimientos(rendimientos, incendio)

	sol := s.Solve(incendio, recursos)
	log.Println("Solucion: ", sol)
	// fmt.Println("Total recursos: ", len(sol.Asignacion))
	// fmt.Println("Fitness: ", s.CalcularFitness(incendio, sol))

	tmpl, err := template.ParseFiles("web/templates/results.html")
	if err != nil {
		http.Error(w, "Error al cargar la plantilla", http.StatusInternalServerError)
		log.Println("Error al parsear results.html", err)
		return
	}

	tmpl.Execute(w, sol.Asignacion)
}

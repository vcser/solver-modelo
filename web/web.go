package web

import (
	"html/template"
	"log"
	"net/http"
	"recomendador-despacho/solver"
	"strconv"
	"time"
)

var templates = template.Must(template.ParseGlob("web/templates/*.html"))

func IndexHandler(w http.ResponseWriter, r *http.Request) {
	templates.ExecuteTemplate(w, "index.html", nil)
}

func SolveHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
		return
	}

	latStr := r.FormValue("lat")
	lonStr := r.FormValue("lon")

	log.Printf("Received coordinates: lat=%s, lon=%s\n", latStr, lonStr)

	// convertir latStr y lonStr a float64
	lat, err := strconv.ParseFloat(latStr, 64)
	if err != nil {
		http.Error(w, "Invalid latitude", http.StatusBadRequest)
		return
	}
	lon, err := strconv.ParseFloat(lonStr, 64)
	if err != nil {
		http.Error(w, "Invalid longitude", http.StatusBadRequest)
		return
	}

	log.Printf("Received coordinates: lat=%f, lon=%f\n", lat, lon)

	s := solver.Solver{Timestamp: time.Now()}
	incendio := solver.NewIncendio(lat, lon, s.Timestamp.Add(-15*time.Minute))
	recursos := solver.InitializeRecursos(incendio)

	solucion := s.Solve(incendio, recursos)
	log.Println("Solucion:", solucion)

	templates.ExecuteTemplate(w, "results.html", solucion.Asignacion)

	// timestamp := time.Now().Add(-15 * time.Minute)
	// recursos := cargarRecursos()
	// rendimientos := cargarRendimientos()
	// incendio := solver.Incendio{
	// 	Lat:                 -37.418420,
	// 	Lon:                 -72.394186,
	// 	Temperatura:         15,
	// 	Humedad:             40,
	// 	VelocidadViento:     22,
	// 	Pendiente:           1,
	// 	FactorVPL:           0.02,
	// 	ModeloCombustible:   "PCH1",
	// 	ValorRodalXHectarea: 1000,
	// 	TiempoInicio:        timestamp,
	// }

	// // incendio.CalcularMeteorologia()
	// log.Println("Incendio updated", incendio)

	// s := solver.Solver{Timestamp: time.Now()}
	// recursos.CalcularETAs(incendio)
	// recursos.CalcularRendimientos(rendimientos, incendio)

	// sol := s.Solve(incendio, recursos)
	// fmt.Println("Solucion: ", sol)
	// fmt.Println("Total recursos: ", len(sol.Asignacion))
	// fmt.Println("Fitness: ", s.CalcularFitness(incendio, sol))
}

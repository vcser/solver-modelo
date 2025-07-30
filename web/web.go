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
var config = solver.Config{
	SolutionTimeout: 5 * time.Second,
}

func IndexHandler(w http.ResponseWriter, r *http.Request) {
	templates.ExecuteTemplate(w, "index.html", config)
}

// Considerar pasar la configuracion junto con la request de /solve
func ConfigHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
		return
	}

	timeoutStr := r.FormValue("solutionTimeout")
	timeout, err := strconv.Atoi(timeoutStr)
	if err != nil || timeout <= 0 {
		http.Error(w, "Invalid timeout value", http.StatusBadRequest)
		return
	}

	config.SolutionTimeout = time.Duration(timeout) * time.Second
	log.Printf("Configuration updated: SolutionTimeout = %v\n", config.SolutionTimeout)

	http.Redirect(w, r, "/", http.StatusSeeOther)
}

func SolveHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
		return
	}

	latStr := r.FormValue("lat")
	lonStr := r.FormValue("lon")

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

	s := solver.Solver{Timestamp: time.Now(), Configuration: config}
	incendio := solver.NewIncendio(lat, lon, s.Timestamp.Add(-15*time.Minute))
	recursos := solver.InitializeRecursos(incendio)

	solucion := s.Solve(incendio, recursos)
	log.Println("Solucion:", solucion)

	templates.ExecuteTemplate(w, "results.html", solucion.Asignacion)
}

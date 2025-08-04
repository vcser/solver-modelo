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
	recursos := solver.GetRecursos()
	templates.ExecuteTemplate(w, "index.html", recursos)
}

func SolveHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Redirect(w, r, "/", http.StatusSeeOther)
		return
	}

	latStr := r.FormValue("lat")
	lonStr := r.FormValue("lon")
	timeoutStr := r.FormValue("solutionTimeout")

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
	timeout, err := strconv.Atoi(timeoutStr)
	if err != nil || timeout <= 0 {
		http.Error(w, "Invalid timeout value", http.StatusBadRequest)
		return
	}
	log.Printf("Received coordinates: lat=%f, lon=%f\n", lat, lon)

	configuration := solver.Config{
		SolutionTimeout: time.Duration(timeout) * time.Second,
	}
	log.Println("Configuration:", configuration)

	s := solver.Solver{Timestamp: time.Now(), Configuration: configuration}
	incendio := solver.NewIncendio(lat, lon, s.Timestamp.Add(-15*time.Minute))
	recursos := solver.InitializeRecursos(incendio)

	solucion := s.Solve(incendio, recursos)
	log.Println("Solucion:", solucion)

	data := struct {
		Incendio solver.Incendio
		Solucion solver.Recursos
	}{Incendio: incendio, Solucion: solucion.Asignacion}

	log.Println("Enviando:", data)

	templates.ExecuteTemplate(w, "results.html", data)
}

package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"recomendador-despacho/solver"
	"time"
)

type JSONInput struct {
	Timestamp time.Time        `json:"timestamp"`
	Fire      solver.Incendio  `json:"fire"`
	Recursos  []solver.Recurso `json:"recursos"`
}

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

func main() {
	timestamp := time.Now().Add(-15 * time.Minute)
	recursos := cargarRecursos()
	rendimientos := cargarRendimientos()
	incendio := solver.Incendio{
		Lat:                 -37.418420,
		Lon:                 -72.394186,
		Temperatura:         15,
		Humedad:             40,
		VelocidadViento:     22,
		Pendiente:           1,
		FactorVPL:           0.02,
		ModeloCombustible:   "PCH1",
		ValorRodalXHectarea: 1000,
		TiempoInicio:        timestamp,
	}

	// incendio.CalcularMeteorologia()
	log.Println("Incendio updated", incendio)

	s := solver.Solver{Timestamp: time.Now()}
	recursos.CalcularETAs(incendio)
	recursos.CalcularRendimientos(rendimientos, incendio)

	sol := s.Solve(incendio, recursos)
	fmt.Println("Solucion: ", sol)
	fmt.Println("Total recursos: ", len(sol.Asignacion))
	fmt.Println("Fitness: ", s.CalcularFitness(incendio, sol))
}

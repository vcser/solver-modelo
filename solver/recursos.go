package solver

import (
	"encoding/json"
	"log"
	"math/rand"
	"os"
	"time"
)

type Recurso struct {
	Lat               float64   `json:"lat"`
	Lon               float64   `json:"lon"`
	Tipo              string    `json:"tipo"`
	ID                string    `json:"id"`
	ETA               time.Time `json:"ETA"`
	Rendimiento       float64   `json:"rendimiento"`
	CostoTransporte   float64   `json:"costoTransporte"`
	CostoUso          float64   `json:"costoUso"` // eliminar
	VelocidadPromedio float64   `json:"velocidadPromedio"`
}

type Recursos []Recurso

func InitializeRecursos(i Incendio) Recursos {
	file, err := os.Open("data/recursos.json")
	if err != nil {
		log.Fatalf("Error leyendo recursos: %v\n", err)
	}
	defer file.Close()

	var content Recursos
	decoder := json.NewDecoder(file)
	decoder.Decode(&content)

	content.CalcularETAs(i)
	content.CalcularRendimientos(cargarRendimientos(), i)

	return content
}

func GetRecursos() Recursos {
	file, err := os.Open("data/recursos.json")
	if err != nil {
		log.Fatalf("Error leyendo recursos: %v\n", err)
	}
	defer file.Close()
	var content Recursos
	decoder := json.NewDecoder(file)
	decoder.Decode(&content)
	return content
}

func (r Recursos) GetRandom(afinidades map[Recurso]float64) Recurso {
	afinidadTotal := 0.0
	for _, value := range afinidades {
		afinidadTotal += value
	}
	elegido := afinidadTotal * rand.Float64()
	prioridadAcumulada := 0.0
	for rec, value := range afinidades {
		prioridadAcumulada += value
		if prioridadAcumulada >= elegido {
			return rec
		}
	}
	return Recurso{}
}

func (r Recursos) CalcularETAs(i Incendio) {
	initialTime := time.Now() // TODO: Revisar bien esto
	for index, rec := range r {
		distance := HaversineDistance(rec.Lat, rec.Lon, i.Lat, i.Lon)
		timeHours := distance / rec.VelocidadPromedio
		ETADuration := time.Duration(timeHours * float64(time.Hour))
		r[index].ETA = initialTime.Add(ETADuration)
	}
}

func (r Recursos) CalcularRendimientos(rendimientos Rendimientos, i Incendio) {
	modCom := i.ModeloCombustible
	for index, rec := range r {
		rendimiento := rendimientos[ModCom(modCom)][TipoRecurso(rec.Tipo)]
		r[index].Rendimiento = rendimiento
	}
}

type ModCom string
type TipoRecurso string

type Rendimientos map[ModCom]map[TipoRecurso]float64

func cargarRendimientos() Rendimientos {
	file, err := os.Open("data/rendimientos.json")
	if err != nil {
		log.Fatalf("Error leyendo recursos: %v\n", err)
	}
	defer file.Close()

	var content Rendimientos
	decoder := json.NewDecoder(file)
	decoder.Decode(&content)

	return content
}

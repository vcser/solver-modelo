package solver

import (
	"math/rand"
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
	CostoUso          float64   `json:"costoUso"`
	VelocidadPromedio float64   `json:"velocidadPromedio"`
}

type Recursos []Recurso

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

// Estructura muy fija, no permite modificar tipos de recursos facilmente
// type DataTipo struct {
// 	Helitransportada float64
// 	Tipo1 float64
// 	Tipo2 float64
// 	Mecanizada float64
// 	AvionCisterna float64
// 	HelicopteroMediano float64
// 	HelicopteroGrande float64
// }

type ModCom string
type TipoRecurso string

type Rendimientos map[ModCom]map[TipoRecurso]float64

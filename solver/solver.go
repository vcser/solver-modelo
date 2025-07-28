package solver

import (
	"slices"
	"time"
)

type Solucion struct {
	// asignacion de recursos
	// metricas del incendio, costos
	Asignacion []Recurso
}

type Solver struct {
	Timestamp time.Time
	Predictor Kitral
}

func (s *Solver) CalcularAfinidad(r Recurso, i Incendio) float64 {
	tTotal := s.Timestamp.Add(6 * time.Hour)
	metros := s.MetrosDeLinea(r, i, tTotal)
	return metros / r.CostoUso
}

func (s *Solver) GetAfinidades(i Incendio, r Recursos) map[Recurso]float64 {
	afinidades := make(map[Recurso]float64)
	for _, rec := range r {
		afinidad := s.CalcularAfinidad(rec, i)
		afinidades[rec] = afinidad
	}
	return afinidades
}

func (s *Solver) GetNoUsados(r Recursos, sol Solucion) []Recurso {
	result := make([]Recurso, 0)
	for _, rec := range r {
		if !slices.Contains(sol.Asignacion, rec) {
			result = append(result, rec)
		}
	}
	return result
}

func (s *Solver) PasoBusquedaLocal(i Incendio, r Recursos, sol *Solucion) bool {
	currentFitness := s.CalcularFitness(i, *sol)
	vecino := s.RevisarVecindad(i, r, *sol)
	fitnessVecino := s.CalcularFitness(i, vecino)

	if fitnessVecino > currentFitness {
		*sol = vecino
		return true
	}
	return false
}

func (s *Solver) RevisarVecindad(i Incendio, r Recursos, sol Solucion) Solucion {
	recursosOciosos := s.GetNoUsados(r, sol)
	mejorFitness := s.CalcularFitness(i, sol)
	ans := sol

	for _, rec := range recursosOciosos {
		ans.Asignacion = append(ans.Asignacion, rec) // se esta agregando pero no quitando a la solucion original
		fitnessAsignacion := s.CalcularFitness(i, ans)
		if fitnessAsignacion > mejorFitness {
			return ans
		}
		ans.Asignacion = ans.Asignacion[:len(ans.Asignacion)-1]
	}

	return ans
}

func (s *Solver) CalcularFitness(i Incendio, sol Solucion) float64 {
	costoSinControl := s.Predictor.Costo(i, 6*time.Hour)
	localFitness, costoQuemadoSolucion := 0.0, 0.0

	tiempoApagado, _ := s.ApagaIncendio(sol.Asignacion, i)
	horasPropagacion := tiempoApagado.Sub(i.TiempoInicio)
	costoQuemadoSolucion = s.Predictor.Costo(i, horasPropagacion)

	costoUsoRecursos, costoTransporteRecursos := 0.0, 0.0
	for _, rec := range sol.Asignacion {
		var tiempoHastaLlegada time.Duration
		if rec.ETA.After(s.Timestamp) {
			tiempoHastaLlegada = rec.ETA.Sub(s.Timestamp)
		} else {
			tiempoHastaLlegada = time.Duration(0)
		}
		costoTransporteRecursos += tiempoHastaLlegada.Hours() * rec.CostoTransporte
		costoUsoRecursos += (horasPropagacion.Hours() - tiempoHastaLlegada.Hours()) * rec.CostoUso
	}

	localFitness = costoSinControl - costoQuemadoSolucion - costoUsoRecursos - costoTransporteRecursos
	return localFitness
}

func (s *Solver) GenerateSolution(fire Incendio, r Recursos) Solucion {
	afinidades := s.GetAfinidades(fire, r)
	tObjetivo := fire.TiempoInicio.Add(time.Hour) // t inicio + 1 hora
	asignados := make([]Recurso, 0)

	for len(afinidades) > 0 {
		recurso := r.GetRandom(afinidades)
		asignados = append(asignados, recurso)
		delete(afinidades, recurso)

		tApagado, puedeApagar := s.ApagaIncendio(asignados, fire)
		if !puedeApagar {
			// no se puede apagar con la asignacion generada
			continue
		}
		if tApagado.Before(tObjetivo) || tApagado.Equal(tObjetivo) {
			// log.Println("Se apaga el incendio en el tiempo: ", tApagado)
			// la asignacion apaga el incendio
			break
		}
	}

	sol := Solucion{Asignacion: asignados}

	return sol
}

// idea: como solamente nos interesa dar la recomendacion ideal para cada incendio, considerar que solo existe un incendio
func (s *Solver) Solve(fire Incendio, r Recursos) Solucion {
	sol := s.GenerateSolution(fire, r)
	bestSolution := sol
	bestFitness := s.CalcularFitness(fire, sol)

	inicio := time.Now()
	for time.Now().Before(inicio.Add(5000 * time.Millisecond)) {
		s.PasoBusquedaLocal(fire, r, &sol)
		fitness := s.CalcularFitness(fire, sol)

		if fitness > bestFitness {
			bestSolution = sol
			bestFitness = fitness
		}
		sol = s.GenerateSolution(fire, r)
	}

	return bestSolution
}

func (s *Solver) MetrosDeLinea(r Recurso, i Incendio, t time.Time) float64 {
	timepoLlegada := r.ETA
	if s.Timestamp.After(r.ETA) {
		timepoLlegada = s.Timestamp
	}
	if t.Before(timepoLlegada) {
		return 0
	}
	tiempoEnCombate := t.Sub(timepoLlegada)
	horasEnCombate := tiempoEnCombate.Hours()
	return horasEnCombate * r.Rendimiento
}

func (s *Solver) ApagaIncendio(asignacion []Recurso, i Incendio) (time.Time, bool) {
	// l := s.Timestamp
	l := i.TiempoInicio
	r := l.Add(time.Hour)

	puedeApagar := false
	for l.Before(r) {
		m := l.Add(r.Sub(l) / 2)
		tDesdeInicioM := m.Sub(i.TiempoInicio)
		perimetroIncendioM := s.Predictor.PerimetroFrontal(i, tDesdeInicioM)
		for _, rec := range asignacion {
			metrosArmados := s.MetrosDeLinea(rec, i, m)
			perimetroIncendioM -= metrosArmados
		}
		if perimetroIncendioM > 0.0 {
			l = m.Add(time.Second)
		} else {
			puedeApagar = true
			r = m
		}
	}
	if puedeApagar {
		return r, true
	}
	// asumimos que un incendio que no se puede controlar en 1h se apagara en 6h
	return i.TiempoInicio.Add(6 * time.Hour), false
}

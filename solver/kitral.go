package solver

import (
	"log"
	"math"
	"time"
)

type calculo struct {
	Area      float64
	Perimetro float64
	Frente    float64
}

type Kitral struct {
	IncendiosProcesados map[Incendio]calculo
}

func calcularRatioElipse(velocidadViento, pendiente float64) float64 {
	a, b := 0.05, 0.01
	r := 1.0 + a*velocidadViento + b*pendiente

	if r > 3.0 {
		r = 3.0
	}
	if r < 1.0 {
		r = 1.0
	}
	return r
}

func (k *Kitral) ProcesarIncendio(i Incendio) {
	IHCFM := -2.97374 + 0.262*i.Humedad - 0.00982*i.Temperatura
	factorHumedad := (389.1624 - 14.3*IHCFM + 0.2*IHCFM*IHCFM) / (3.559 + 1.6615*IHCFM + 2.62392*IHCFM*IHCFM)
	factorViento := 1 + 0.51218*i.VelocidadViento - 0.007151*i.VelocidadViento*i.VelocidadViento
	factorPendiente := i.Pendiente*i.Pendiente*0.00013585 + i.Pendiente*0.023322 + 1
	factorCombustible := i.FactorVPL

	velocidadPropagacion := factorHumedad * factorCombustible * (factorViento + factorPendiente) * 60 * 60 / 1000

	ratio := calcularRatioElipse(i.VelocidadViento, i.Pendiente)

	elipsoide := math.Sqrt(1 - 1/(ratio*ratio))
	Ea := (velocidadPropagacion + velocidadPropagacion*(1-elipsoide)/(1+elipsoide)) / 2
	Eb := Ea / ratio

	perimetroPorHora := math.Pi * (3*(Ea+Eb) - math.Sqrt((3*Ea+Eb)*(Ea+3*Eb))) * 1000
	areaPorHora := math.Pi * Ea * Eb * 1000
	velocidadFrente := 2 * Ea * 1000

	log.Println("Incendio procesado, perimetro/h =", perimetroPorHora, ", Perimetro frontal/h =", velocidadFrente)

	k.IncendiosProcesados[i] = calculo{Perimetro: perimetroPorHora, Area: areaPorHora, Frente: velocidadFrente}
}

func (k *Kitral) Perimetro(i Incendio, t time.Duration) float64 {
	if k.IncendiosProcesados == nil {
		k.IncendiosProcesados = make(map[Incendio]calculo)
	}
	incendio, exists := k.IncendiosProcesados[i]
	if !exists {
		k.ProcesarIncendio(i)
	}
	// log.Println("Perimetro del incendio = ", incendio.Perimetro*float64(t), " a las ", t.Hours(), " horas.")
	return incendio.Perimetro * t.Hours()
}

func (k *Kitral) PerimetroFrontal(i Incendio, t time.Duration) float64 {
	if k.IncendiosProcesados == nil {
		k.IncendiosProcesados = make(map[Incendio]calculo)
	}
	incendio, exists := k.IncendiosProcesados[i]
	if !exists {
		k.ProcesarIncendio(i)
		incendio = k.IncendiosProcesados[i]
	}

	perimetroFrontal := incendio.Frente * t.Hours()

	// fmt.Println("Perimetro frontal a las", t.Hours(), "horas =", perimetroFrontal) // DEBUG
	return perimetroFrontal
}

func (k *Kitral) Area(i Incendio, t time.Duration) float64 {
	if k.IncendiosProcesados == nil {
		k.IncendiosProcesados = make(map[Incendio]calculo)
	}
	incendio, exists := k.IncendiosProcesados[i]
	if !exists {
		k.ProcesarIncendio(i)
	}
	return incendio.Area * t.Hours()
}

func (k *Kitral) Costo(i Incendio, t time.Duration) float64 {
	if k.IncendiosProcesados == nil {
		k.IncendiosProcesados = make(map[Incendio]calculo)
	}
	incendio, exists := k.IncendiosProcesados[i]
	if !exists {
		k.ProcesarIncendio(i)
	}
	return incendio.Area * t.Hours() * i.ValorRodalXHectarea
}

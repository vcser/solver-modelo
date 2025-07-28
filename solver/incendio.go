package solver

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"time"
)

type Incendio struct {
	Pendiente           float64   `json:"pendiente"`
	Temperatura         float64   `json:"temperatura"`
	Humedad             float64   `json:"humedad"`
	VelocidadViento     float64   `json:"velocidadViento"`
	FactorVPL           float64   `json:"factorVPL"`
	ValorRodalXHectarea float64   `json:"valorRodalXHectarea"`
	TiempoInicio        time.Time `json:"timestamp"`
	Lat                 float64   `json:"lat"`
	Lon                 float64   `json:"lon"`
	ModeloCombustible   string    `json:"modeloCombustible"`
}

type meteoResponse struct {
	Current struct {
		Temperature float64 `json:"temperature_2m"`
		Humidity    float64 `json:"relative_humidity_2m"`
		WindSpeed   float64 `json:"wind_speed_10m"`
	} `json:"current"`
}

func (i *Incendio) CalcularMeteorologia() {
	url := fmt.Sprintf("https://api.open-meteo.com/v1/forecast?latitude=%v&longitude=%v&current=temperature_2m,relative_humidity_2m,wind_speed_10m&timezone=auto", i.Lat, i.Lon)

	response, err := http.Get(url)
	if err != nil {
		log.Fatalf("Error al hacer la request GET: %v", err)
	}
	defer response.Body.Close()

	if response.StatusCode != http.StatusOK {
		log.Fatalf("La request falló con código de estado: %d %s", response.StatusCode, response.Status)
	}

	// Leer el cuerpo de la respuesta
	body, err := io.ReadAll(response.Body)
	if err != nil {
		log.Fatalf("Error al leer el cuerpo de la respuesta: %v", err)
	}

	var meteo meteoResponse
	err = json.Unmarshal(body, &meteo)
	if err != nil {
		log.Fatalf("Error al decodificar el JSON: %v", err)
	}

	i.Temperatura = meteo.Current.Temperature
	i.Humedad = meteo.Current.Humidity
	i.VelocidadViento = meteo.Current.WindSpeed
}

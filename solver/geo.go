package solver

import "math"

const EarthRadiusKm = 6371

// degToRad convierte grados a radianes.
// Las funciones trigonométricas en Go (y en Haversine) operan con radianes.
func degToRad(deg float64) float64 {
	return deg * math.Pi / 180
}

// HaversineDistance calcula la distancia en kilómetros entre dos puntos
// en la superficie de la Tierra, dadas sus latitudes y longitudes en grados decimales.
//
// Parámetros:
//   lat1, lon1: Latitud y longitud del primer punto.
//   lat2, lon2: Latitud y longitud del segundo punto.
//
// Retorna:
//   La distancia geodésica entre los dos puntos en kilómetros.
func HaversineDistance(lat1, lon1, lat2, lon2 float64) float64 {
	// Convertir las latitudes y longitudes de grados a radianes
	lat1Rad := degToRad(lat1)
	lon1Rad := degToRad(lon1)
	lat2Rad := degToRad(lat2)
	lon2Rad := degToRad(lon2)

	// Calcular la diferencia de latitud y longitud
	deltaLat := lat2Rad - lat1Rad
	deltaLon := lon2Rad - lon1Rad

	// Aplicar la fórmula de Haversine
	a := math.Sin(deltaLat/2)*math.Sin(deltaLat/2) +
		math.Cos(lat1Rad)*math.Cos(lat2Rad)*
			math.Sin(deltaLon/2)*math.Sin(deltaLon/2)

	c := 2 * math.Atan2(math.Sqrt(a), math.Sqrt(1-a))

	// Calcular la distancia final
	distance := EarthRadiusKm * c
	return distance
}

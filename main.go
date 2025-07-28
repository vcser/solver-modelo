package main

import (
	"log"
	"net/http"
	"recomendador-despacho/web"
)

func main() {
	http.HandleFunc("/", web.IndexHandler)
	http.HandleFunc("/solve", web.SolveHandler)

	log.Println("Servidor iniciado en http://localhost:8080")
	log.Fatal(http.ListenAndServe(":8080", nil))
}

package main

import (
	"fmt"
	"net/http"
	"recomendador-despacho/web"
)

func main() {
	http.HandleFunc("/", web.IndexHandler)
	http.HandleFunc("/solve", web.SolveHandler)
	http.HandleFunc("/config", web.ConfigHandler)

	fmt.Println("Starting server at http://localhost:8080")
	http.ListenAndServe(":8080", nil)
}

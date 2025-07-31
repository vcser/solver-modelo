# --- Etapa de construcción ---
# Utiliza una imagen base de Go para compilar la aplicación.
# Esta etapa contiene todas las herramientas necesarias para la compilación.
FROM golang:1.24-alpine AS builder

# Establece el directorio de trabajo dentro del contenedor.
WORKDIR /app

# Copia los archivos go.mod y go.sum para descargar las dependencias.
# Esto permite que Docker cachee las dependencias si no cambian.
COPY go.mod go.sum ./

# Descarga las dependencias del módulo Go.
RUN go mod download

# Copia todo el código fuente de la aplicación al directorio de trabajo.
COPY . .

# Compila la aplicación Go.
# CGO_ENABLED=0: Deshabilita CGO para crear un binario estáticamente enlazado,
# lo que es bueno para imágenes más pequeñas y sin dependencias de glibc.
# GOOS=linux: Asegura que el binario se compile para Linux.
# GOARCH=amd64: Asegura que el binario se compile para la arquitectura amd64.
# -o main: Define el nombre del binario de salida como 'main'.
# ./cmd/your-app-name: Reemplaza con la ruta a tu paquete principal (main.go).
# Por ejemplo, si tu main.go está en la raíz, usa `./`.
# Si está en `./cmd/server/main.go`, usa `./cmd/server`.
RUN CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -ldflags="-s -w" -o main ./cmd/your-app-name

# --- Etapa de producción ---
# Utiliza una imagen base mínima para la aplicación final.
# alpine es muy pequeña y no contiene herramientas de desarrollo innecesarias.
FROM alpine:latest

# Establece el directorio de trabajo para la aplicación final.
WORKDIR /app

# Copia el binario compilado desde la etapa 'builder'.
# 'main' es el nombre del binario que definimos en la etapa de construcción.
COPY --from=builder /app/main .

# Copia la carpeta 'data' y su contenido al directorio de trabajo del contenedor.
# Asegúrate de que la ruta 'data/' sea correcta desde la raíz de tu proyecto.
COPY data/ ./data/

# Copia la carpeta 'web/templates' y su contenido al directorio de trabajo del contenedor.
# Asegúrate de que la ruta 'web/templates/' sea correcta desde la raíz de tu proyecto.
COPY web/templates/ ./web/templates/

# Expone el puerto en el que escucha tu aplicación Go.
# Asegúrate de que este puerto coincida con el puerto que tu aplicación usa.
EXPOSE 8080

# Define el comando por defecto para ejecutar la aplicación cuando el contenedor se inicie.
ENTRYPOINT ["./main"]

# Comando que se ejecuta si no se especifica otro comando al iniciar el contenedor.
CMD []

FROM go1.24-alpine
WORKDIR /app
COPY . .
# RUN go mod download
RUN go build -o solver-modelo
EXPOSE 8080
CMD ["./solver-modelo"]
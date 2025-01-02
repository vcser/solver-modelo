mkdir "resultados/Toy Datasets"
for metodo in 0 1 2 3
  do
  for dataset in Toy\ Datasets/*
  do
    echo $dataset
    timeout 300 ./solver -i "$dataset" -s $metodo
    mv salidaAnytime.txt "resultados/$dataset$metodo"
  done
done

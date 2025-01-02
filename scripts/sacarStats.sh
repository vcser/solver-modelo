
echo $#
echo $1
if [ $# -eq 0 ]
  then
    echo "Falta path a dataset"
    exit
fi
dataset=$(echo "$1" | rev | cut -d "/" -f 1 | rev | cut -d "." -f 1)
echo $dataset
# rm ~/GitHub/solver-incendios/Resultados/$dataset/resumen.csv
echo "" > ~/GitHub/solver-incendios/Resultados/$dataset/resumen.csv


echo "dataset media mediana std" >> ~/GitHub/solver-incendios/Resultados/$dataset/resumen.csv
for metodo in 0 1 2
do
  echo -n $metodo" " >> ~/GitHub/solver-incendios/Resultados/$dataset/resumen.csv
  tail -q -n 1 ~/GitHub/solver-incendios/Resultados/$dataset/$metodo"ejecucion"*".txt" \
   | cut -d " " -f 2 | datamash mean 1 median 1 sstdev 1 >> ~/GitHub/solver-incendios/Resultados/$dataset/resumen.csv
done

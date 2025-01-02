
echo $#
echo $1
if [ $# -eq 0 ]
  then
    echo "Falta path a dataset"
    exit
fi
dataset=$(echo "$1" | rev | cut -d "/" -f 1 | rev | cut -d "." -f 1)
echo $dataset

mkdir ~/GitHub/solver-incendios/Resultados/$dataset
rm ~/GitHub/solver-incendios/Resultados/$dataset/*
cd ..
for metodo in 0 1 2
do
  for ejecucion in $(seq 10)
  do
    # cat "$1"
    echo $ejecucion
    rm ~/GitHub/solver-incendios/salidaAnytime.txt
    rm ~/GitHub/solver-incendios/salidaAsignacion.txt
    timeout 3 ./solver -i "$1" -s $metodo  -v 0 -o salidaAsignacion.txt
    cp ~/GitHub/solver-incendios/salidaAnytime.txt ~/GitHub/solver-incendios/Resultados/$dataset/$metodo"ejecucion"$ejecucion".txt"
    cp ~/GitHub/solver-incendios/salidaAsignacion.txt ~/GitHub/solver-incendios/Resultados/$dataset/$metodo"Asignacion"$ejecucion".txt"
  done
done


echo $#
echo $1
if [ $# -eq 0 ]
  then
    echo "Falta path a dataset"
    exit
fi
dataset=$(echo "$1" | rev | cut -d "/" -f 1 | rev | cut -d "." -f 1)
echo $dataset

python graficar.py ~/GitHub/solver-incendios/Resultados/$dataset/

exit
# cd ..
# for metodo in 0 1 2
# do
#   for ejecucion in $(seq 10)
#   do
#     # cat "$1"
#     echo $ejecucion
#     rm ~/GitHub/solver-incendios/salidaAnytime.txt
#     ./solver -i "$1" -s $metodo -horasAfinidad 3 -v 0
#     cp ~/GitHub/solver-incendios/salidaAnytime.txt ~/GitHub/solver-incendios/Resultados/$dataset/$metodo"ejecucion"$ejecucion".txt"
#   done
# done

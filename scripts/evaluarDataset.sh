echo $#
echo $1
if [ $# -eq 0 ]
  then
    echo "Falta path a dataset"
    exit
fi


sh sacarResultados.sh "$1"
# sleep 5
sh sacarStats.sh "$1"
# sleep 5
sh graficar.sh "$1"
# sleep 5

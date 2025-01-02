#cambiar de acuerdo a localizacion de los datasets
for dataset in ~/GitHub/solver-incendios/Realistas\ Datasets/Dataset\ para\ informe/Realista_5*
do
  echo $dataset
  sh evaluarDataset.sh "$dataset"
done

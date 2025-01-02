import sys
import matplotlib.pyplot as plt
import pandas as pd

labels = {
0:"Greedy por distancia",
1:"GRASP",
2:"Greedy por afinidad",
3:"Fuerza bruta",
}

path = sys.argv[1]
print(path)

for metodo in range(3):
    file = path + str(metodo) + "ejecucion1.txt"
    data = pd.read_csv(file,sep=" ",names=["time","fitness"])
    data.loc[len(data)]= [1000,max(data.fitness)]
    plt.plot(data["time"],data["fitness"], label=labels[metodo],drawstyle="steps-post")
plt.legend()
plt.xlim(-0.1,1)
plt.xlabel("Tiempo [s]")
plt.ylabel("Beneficio [USD]")
plt.savefig(path+"grafico.pdf")
plt.savefig(path+"grafico.png")

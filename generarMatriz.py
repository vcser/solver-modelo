import random

for i in range(32):
    for j in range(6):
        print( round((0.5+random.random())*1000,2) ,end=" ")
    print()

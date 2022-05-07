import os
import time
import matplotlib.pyplot as plt

tests = []
n = 100

for i in range(1, 9):
    
    cmd = f"./fec Binary_doc_test -f tests.txt -n {i}"
    total = 0
    for _ in range(n):
        start = time.time()
        os.system(cmd)
        total += time.time() - start
    tests.append(total / n)

plt.title("Temps d'exécution du programme en fonction du nombre N de threads")
plt.xlabel("Nombre N + 2 threads")
plt.ylabel("Temps en ms mis par le programme python pour une exécution")
plt.plot(range(1, len(tests)+1), tests)
plt.show()
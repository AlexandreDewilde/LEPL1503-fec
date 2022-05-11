import os
import time
import matplotlib.pyplot as plt
import seaborn as sns
import tempfile
import shutil
import numpy as np
import pandas as pd


sns.set_theme()


n = 500

with open("samples/africa.bin", "rb") as f:
    content = f.read()

tests = []
for nb in range(20, 100, 20):
    tests = []
    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    for i in range(1, 5):
        
        cmd = f"./fec {dirpath} -f tests.txt -n {i}"
        os.system(f"sh ./test_time.sh '{cmd}' out 10")
        df = pd.read_csv("out.csv")
        df["runtime"] /= 1_000_000_000
        tests.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

    plt.plot(range(1, len(tests)+1), tests, label=f"Avec {nb} fichiers")

os.remove("tests.txt")

plt.title(f"Temps d'exécution du programme en fonction du nombre N de threads de calcul")
plt.xlabel("Nombre N de threads de calcul")
plt.ylabel("Temps en secondes mis pour une exécution")
plt.xticks(range(1, len(tests)+1))
plt.legend(loc='best')
plt.show()
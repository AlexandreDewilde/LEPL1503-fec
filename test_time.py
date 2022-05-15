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

    plt.plot(range(1, len(tests)+1), tests, label=f"Avec {nb} fichiers africa.bin")

os.remove("tests.txt")

plt.title(f"Temps d'exécution du programme en fonction du nombre N de threads de calcul")
plt.xlabel("Nombre N de threads de calcul")
plt.ylabel("Temps en secondes mis pour une exécution")
plt.xticks(range(1, len(tests)+1))
plt.legend(loc='best')
plt.savefig("ExecutionTimeByNumberOfThreads.png")
plt.close()


tests_c_mono = []
tests_c = []

for nb in range(20, 100, 20):
    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    cmd = f"./fec {dirpath} -f tests.txt -n 1"
    os.system(f"sh ./test_time.sh '{cmd}' out 20")
    df = pd.read_csv("out.csv")
    df["runtime"] /= 1_000_000_000
    tests_c_mono.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    cmd = f"./fec {dirpath} -f tests.txt"
    os.system(f"sh ./test_time.sh '{cmd}' out 10")
    df = pd.read_csv("out.csv")
    df["runtime"] /= 1_000_000_000
    tests_c.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

width = 0.3
X = np.arange(len(tests_c))
bar1 = plt.bar(X+0.1, tests_c_mono, width, color="r")
bar2 = plt.bar(X+width+0.1, tests_c, width, color="b")
plt.legend((bar1, bar2), ("1 thread de calcul", "4 threads de calcul"), loc="best")
plt.xticks(X+width, range(20, 100, 20))
plt.title("Comparaison du programme C: 1 vs 4 threads")
plt.xlabel("Nombre de fichiers africa.bin")
plt.ylabel("Temps d'exécution en secondes")
plt.savefig("ExecutionTimeByProgram.png")
plt.close()

tests_c_mono = []
tests_c = []
tests_py = []

for nb in range(20, 60, 20):
    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    cmd = f"./fec {dirpath} -f tests.txt -n 1"
    os.system(f"sh ./test_time.sh '{cmd}' out 20")
    df = pd.read_csv("out.csv")
    df["runtime"] /= 1_000_000_000
    tests_c_mono.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    cmd = f"./fec {dirpath} -f tests.txt"
    os.system(f"sh ./test_time.sh '{cmd}' out 10")
    df = pd.read_csv("out.csv")
    df["runtime"] /= 1_000_000_000
    tests_c.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    cmd = f"python3 lepl1503-2022-pyfec/main.py {dirpath} -f tests.txt"
    os.system(f"sh ./test_time.sh '{cmd}' out 1")
    df = pd.read_csv("out.csv")
    df["runtime"] /= 1_000_000_000
    tests_py.append(sum(df["runtime"]) / len(df["runtime"]))
    shutil.rmtree(dirpath)

print("Python execution time vs C with one thread vs C with 4 threads :")
for i, nb in enumerate(range(20, 60, 20)):
    print(f"\t with {nb} files africa.bin : {tests_py[i]}s - {tests_c_mono[i]}s - {tests_c[i]}s")
ratio = sum(tests_py) / sum(tests_c)
print(f"Ratio between Python program and multi threaded C : {ratio}")


os.remove("tests.txt")
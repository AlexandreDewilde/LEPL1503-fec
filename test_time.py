import os
import time
import matplotlib.pyplot as plt
import seaborn as sns
import tempfile
import shutil

sns.set_theme()


n = 10

with open("Binary_doc_test/africa.bin", "rb") as f:
    content = f.read()

for nb in range(0, 100, 10):
    tests = []
    dirpath = tempfile.mkdtemp()
    for i in range(nb):
        with open(f"{dirpath}/{i}", 'wb') as f:
            f.write(content)
    for i in range(1, 9):
        
        cmd = f"./fec {dirpath} -f tests.txt -n {i}"
        total = 0
        for _ in range(n):
            start = time.time()
            os.system(cmd)
            total += time.time() - start
        tests.append(total / n)
    shutil.rmtree(dirpath)

    plt.plot(range(1, len(tests)+1), tests, label=f"Avec {nb} fichiers")

os.remove("tests.txt")
plt.title(f"Temps d'exécution du programme en fonction du nombre N de threads")
plt.xlabel("Nombre N + 2 threads")
plt.ylabel("Temps en ms mis par le programme python pour une exécution")
plt.legend(loc='best')
plt.show()
import os
import time
import matplotlib.pyplot as plt

tests = []

for i in range(1, 5):
    start = time.time()
    cmd = f"./fec Binary_doc_test -f tests.txt -n {i}"
    for _ in range(1000):
        os.system(cmd)
    end = time.time()
    tests.append((end - start) / 100)

#TODO ajouter légende
plt.plot(tests)
plt.show()
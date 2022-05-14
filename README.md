# Implementation du code correcteurs des données - LEPL1503 Projet3 

## 1. Introduction

Durant la première moitié du quadrimestre, nous avons appris à programmer dans le langage
de programmation C. Ce langage nous offre un grand contrôle sur la gestion de la mémoire du
programme et sur les instructions réellement exécutées par l’ordinateur, contrairement à Python 2.
Ensuite, durant la deuxième moitié, nous avons été demandé de produire une version d’un programe
en C, à partir d’un programme Python, qui résout le problème de l’utilisation de codes correcteurs
d’erreurs.
Notre programme C doit significativement être plus rapide que ce de Python en utilisant
plusieurs threads pour paralléliser l’exécution sur les multiples coeurs du processeur de la machine.
Enfin, nous allons aussi être côté sur la consommation énergétique de notre implémentation dans le cas
où il est éxecuté sur un Raspberry-Pi ou autre système embarqué fonctionnant sur batterie.

Notre programme est séquentiel. Nous avons pas su terminer la version multithreadé en raisons de manque de temps pour le debugger.

## 2. Execution du programme
Pour executer le code, Il fuat exécuter les instructions suivants dans l'ordre:
1.  ```make fec``
2. ```./fec``` avec le répetoire du dossier contenant les fichiers txt
Vous pouvez ajouter le ```-v``` pour exécuter le programme en mode verbosePour exécuer les fichiers tests déjà implementés, exécutez les instructions suivantes:```make tests```
## 3. Bugs potentiels
Les bugs peuvent surgir dans les variables globales que nous utilisons partout dans le programme. Nous avons essayé de les debug mais, par manque de temps, nous avons pas fini cette tâche.

## 4. Version multithreadé
Cette version n'est pas disponible pour le moment car nous avons pas fini de faire tous les debugs. Cependant, les codes sont présent dans ce programme soumis pour les reviews.
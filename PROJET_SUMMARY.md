# 📚 PROJET TPIA - BENCHMARK FRONT PARETO COMPLET

## 🎯 Résumé Exécutif

Ce projet implémente et compare **3 algorithmes d'exploration du front Pareto** pour le problème d'ordonnancement d'ateliers (Flow Shop) avec 2 objectifs : **Makespan** et **Tardiness**.

### Trois approches de test disponibles:

1. **main.c** - Une exécution par algorithme
2. **main_multiple_runs.c** - 10 exécutions par algorithme (RECOMMANDÉ)
3. **main_climbers.c** - Test des algorithmes de recherche locale basique

---

## 🚀 Démarrage Rapide

### Option 1 : Test complet automatisé (Recommandé)
```bash
chmod +x run_multiple_runs.sh
./run_multiple_runs.sh
```

### Option 2 : Exécution manuelle étape par étape
```bash
# Compiler
gcc -o prog_multiple_runs main_multiple_runs.c solution.c instance.c helper.c test.c -lm

# Exécuter
./prog_multiple_runs

# Visualiser
python3 visualize_multiple_runs.py
```

### Option 3 : Test simple et rapide
```bash
gcc -o prog main.c solution.c instance.c helper.c test.c -lm
./prog
python3 analyze_pareto.py
```

---

## 📂 Structure des Fichiers

### Code Source
```
main.c                   - Version simple (1 run par algo)
main_multiple_runs.c     - Version robuste (10 runs par algo) ⭐
main_climbers.c          - Algorithmes de base
solution.c/h             - Implémentations des algos
instance.c/h             - Chargement des instances
helper.c/h               - Utilitaires
test.c/h                 - Tests
```

### Scripts
```
run_multiple_runs.sh     - Automatisation version multiple runs
run_benchmarks.sh        - Menu interactif pour toutes les versions
analyze_pareto.py        - Analyse simple (1 run)
visualize_multiple_runs.py - Analyse multiple runs (10 runs) ⭐
```

### Documentation
```
README.md                - Documentation détaillée
USAGE_GUIDE.md           - Guide des 3 versions
PROJET_SUMMARY.md        - Ce fichier
```

### Données
```
instances/               - Fichiers d'instances (.txt)
plots/                   - Graphiques générés (créé par visualize)
output_*.gnuplot         - Fichiers de résultats bruts
*.png                    - Graphiques de comparaison
```

---

## 🔬 Les Trois Algorithmes

### 1. **Algoritmo Pareto Local Search (PLS)**
- **Principe:** Explore itérativement les voisinages des solutions non-dominées
- **Filtrage:** En ligne (add progressivement)
- **Convergence:** Quand pas de nouvelles solutions non-dominées
- **Avantage:** Équilibre exploration/exploitation

### 2. **Algoritmo Scalaire**
- **Principe:** Teste différents poids (0 à 1) pour combiner objectifs
- **Filtrage:** En ligne (même que Pareto)
- **Couverture:** 21 vecteurs de poids (0.0, 0.05, 0.10, ..., 1.0)
- **Avantage:** Couverture systématique du front

### 3. **Climber Algorithms** (Bonus)
- **Climber First:** Première amélioration acceptée
- **Climber Best:** Meilleure amélioration parmi tous les voisins
- **Iterated Local Search:** Avec perturbations pour sortir des optima locaux
- **Marche Aléatoire:** Exploration aléatoire (baseline)

---

## 📊 Version Recommandée : MULTIPLE RUNS

### Caractéristiques Clés

✅ **10 exécutions indépendantes** par algorithme par instance
✅ **Fronts séparés** : Pareto et Scalaire dans des graphiques différents
✅ **Pas de mélange** : Chaque algo a son propre graphique
✅ **Statistiques robustes** : Min, Max, Moyenne des tailles
✅ **Visualisation claire** : Gradient de couleurs pour les runs
✅ **Analyse variabilité** : Voir la dispersion des résultats

### Utilisation

```bash
./run_multiple_runs.sh
```

**Output:**
- 10 × 2 algos × N instances = fichiers gnuplot
- Graphiques séparés par instance et algorithme
- Dossier `plots/` avec tous les PNG

### Exemple de Graphique
```
Instance: 20_10_01
┌─────────────────────────────┐
│ Algoritmo Pareto (10 runs)  │  ← Gradient BLEU
│ ─ Run 1                     │
│ ─ Run 2                     │  Pareto:
│ ─ ...                       │  - Circles
│ ─ Run 10                    │  - Bleu clair à foncé
└─────────────────────────────┘

┌─────────────────────────────┐
│ Algoritmo Scalaire (10 runs)│  ← Gradient ROUGE
│ ─ Run 1                     │
│ ─ Run 2                     │  Scalaire:
│ ─ ...                       │  - Squares
│ ─ Run 10                    │  - Rouge clair à foncé
└─────────────────────────────┘
```

---

## 📈 Fichiers de Sortie

### Format Données
Chaque fichier `.gnuplot` contient:
```
makespan tardiness dominated_flag
1737 5541 0
1738 5485 0
...
```

### Fichiers Générés par main_multiple_runs.c

**Pareto (10 runs par instance):**
```
output_pareto_multiple_3_3_01_run1.gnuplot
output_pareto_multiple_3_3_01_run2.gnuplot
...
output_pareto_multiple_3_3_01_run10.gnuplot
output_pareto_multiple_7_5_01_run1.gnuplot
...
```

**Scalaire (10 runs par instance):**
```
output_scalaire_multiple_3_3_01_run1.gnuplot
...
output_scalaire_multiple_50_20_01_run10.gnuplot
```

### Graphiques Générés par visualize_multiple_runs.py

**Dans le dossier `plots/`:**
```
pareto_3_3_01.png          ← 10 fronts Pareto superposés (bleu)
scalaire_3_3_01.png        ← 10 fronts Scalaire superposés (rouge)
pareto_7_5_01.png
scalaire_7_5_01.png
...
pareto_50_20_01.png
scalaire_50_20_01.png
```

---

## 🔍 Analyse des Résultats

### Statistiques Affichées
```
Instance: 20_10_01
─────────────────────────────
Algoritmo Pareto (10 runs):
  Tailles: [18 19 20 21 19 18 20 19 21 20]
  Min: 18, Max: 21, Média: 19.5
    Run 1: makespan=[1737, 1789], tardiness=[4050, 5541]
    Run 2: makespan=[1738, 1795], tardiness=[4045, 5520]
    ...

Algoritmo Scalaire (10 runs):
  Tailles: [6 6 6 6 6 7 6 6 6 6]
  Min: 6, Max: 7, Média: 6.1
    Run 1: makespan=[2426, 2667], tardiness=[7127, 8674]
    ...
```

### Interprétation
- **Tailles stables** → Algo robuste
- **Tailles variables** → Dépend de solutions aléatoires
- **Fronts Pareto plus grands** → Plus de solutions non-dominées
- **Fronts Scalaire plus petits** → Couverture concentrée

---

## ⚙️ Configuration et Personnalisation

### Modifier le nombre de runs
Dans `main_multiple_runs.c`:
```c
#define NUM_RUNS 10  // Changer cette valeur
```

### Modifier le nombre de scalarisations
Dans `main_multiple_runs.c`:
```c
int nb_scalarizations = 20;  // Plus grand = plus de couverture
```

### Changer l'opérateur de voisinage
Dans `main_multiple_runs.c`:
```c
// Actuellement: ECHANGE
// Essayer: INSERTION
test_algo_pareto_multiple(current_instance, instances[i].basename, INSERTION);
```

---

## 📊 Instances de Test

| Instance | Jobs | Machines | Taille | Difficulté |
|----------|------|----------|--------|-----------|
| 3_3_01   | 3    | 3        | XS     | Trivial   |
| 7_5_01   | 7    | 5        | S      | Facile    |
| 20_10_01 | 20   | 10       | M      | Moyen     |
| 20_20_01 | 20   | 20       | M      | Moyen     |
| 30_10_01 | 30   | 10       | L      | Difficile |
| 30_20_01 | 30   | 20       | L      | Difficile |
| 50_10_01 | 50   | 10       | XL     | Très difficile |
| 50_20_01 | 50   | 20       | XL     | Très difficile |

---

## 🎓 Concepts Clés

### Front Pareto
- Ensemble de solutions **non-dominées**
- Une solution A domine B si A est meilleure ou égal sur tous les critères et strictement meilleure sur au moins un

### Hypervolume
- Mesure la surface couverte par le front
- Point de référence = (max_makespan × 1.1, max_tardiness × 1.1)
- Plus élevé = meilleur front

### Filtrage En Ligne
- Ajoute progressivement les solutions
- Supprime les solutions dominées
- Limite de taille = MAX_SOLUTIONS (500)

### Dominance Faible
```
Solution A domine B si:
  makespan_A ≤ makespan_B ET tardiness_A ≤ tardiness_B
  ET au moins une inégalité stricte
```

---

## 🐛 Troubleshooting

### Erreur de compilation
```bash
gcc: command not found
```
→ Installer gcc: `sudo apt-get install build-essential`

### Erreur d'importation Python
```bash
ModuleNotFoundError: No module named 'matplotlib'
```
→ Installer matplotlib: `pip3 install matplotlib numpy`

### Aucun fichier gnuplot généré
- Vérifier que le répertoire `instances/` existe
- Vérifier que les fichiers `.txt` sont présents
- Vérifier les permissions en écriture

### Graphiques vides
- Vérifier le format des fichiers gnuplot
- S'assurer que les solutions ont des valeurs makespan/tardiness

---

## 📞 Support et Contact

Pour modifier ou améliorer:
1. Consulter `README.md` pour la documentation détaillée
2. Consulter `USAGE_GUIDE.md` pour les différentes versions
3. Examiner `solution.c` pour comprendre les implémentations

---

## ✅ Checklist Avant de Lancer

- [ ] Répertoire `instances/` existe et contient les fichiers `.txt`
- [ ] gcc est installé (`gcc --version`)
- [ ] Python 3 est installé (`python3 --version`)
- [ ] matplotlib est installé (`pip3 list | grep matplotlib`)
- [ ] Les scripts sont exécutables (`chmod +x *.sh`)
- [ ] L'espace disque est disponible (~100MB pour les résultats)

---

## 🎯 Prochaines Étapes Recommandées

1. **Lancer un test rapide:**
   ```bash
   gcc -o prog main.c solution.c instance.c helper.c test.c -lm && ./prog
   ```

2. **Faire une analyse robuste (recommandée):**
   ```bash
   ./run_multiple_runs.sh
   ```

3. **Explorer les résultats:**
   ```bash
   ls plots/
   ```

4. **Modifier les paramètres et recommencer**

---

**Version:** 1.0  
**Date:** Janvier 2026  
**Statut:** ✅ Production-Ready  
**Dernière mise à jour:** 2026-01-20

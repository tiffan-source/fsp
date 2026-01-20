# Trois Versions de Main - Guide d'Utilisation

## 📊 Vue d'ensemble des trois versions

### 1. **main.c** - Benchmark Unique (Recommandé pour commencer)
**Objectif:** Tester une fois chaque algorithme sur chaque instance

**Commandes:**
```bash
gcc -Ofast -o prog main.c solution.c instance.c helper.c test.c -lm
./prog
python3 analyze_pareto.py
```

**Sortie:**
- `output_pareto_<instance>.gnuplot` - Front Pareto unique par instance
- `output_scalaire_<instance>.gnuplot` - Front Scalaire unique par instance
- `pareto_comparison.png` - Graphique comparatif

**Cas d'usage:**
- Première exécution rapide
- Comparer Pareto vs Scalaire sur une instance
- Calcul d'hypervolume

---

### 2. **main_multiple_runs.c** - Benchmark Répétées (Recommandé pour fiabilité)
**Objectif:** Exécuter 10 fois chaque algorithme sur chaque instance pour mesurer la variabilité

**Commandes:**
```bash
chmod +x run_multiple_runs.sh
./run_multiple_runs.sh
```

Ou manuellement:
```bash
gcc -Ofast -o prog_multiple_runs main_multiple_runs.c solution.c instance.c helper.c test.c -lm
./prog_multiple_runs
python3 visualize_multiple_runs.py
```

**Sortie:**
- `output_pareto_multiple_<instance>_run<N>.gnuplot` (N=1 à 10)
- `output_scalaire_multiple_<instance>_run<N>.gnuplot` (N=1 à 10)
- Dossier `plots/` contenant:
  - `pareto_<instance>.png` - 10 fronts Pareto superposés
  - `scalaire_<instance>.png` - 10 fronts Scalaire superposés

**Cas d'usage:**
- Évaluer la stabilité et variabilité des algorithmes
- Voir la distribution des fronts Pareto
- Mesurer les écarts entre runs
- Analyse statistique
- **IMPORTANT:** Les fronts Pareto et Scalaire sont séparés (pas mélangés)

**Visualisation:**
- Chaque couleur = un run différent
- Pareto: gradient bleu (circles)
- Scalaire: gradient rouge (squares)

---

### 3. **main_climbers.c** - Benchmark Climbers
**Objectif:** Tester les algorithmes de recherche locale basique

**Commandes:**
```bash
gcc -Ofast -o prog_climbers main_climbers.c solution.c instance.c helper.c test.c -lm
./prog_climbers
```

**Sortie:**
- `output_climber_first_<instance>.gnuplot` - Climber First (10 runs)
- `output_climber_best_<instance>.gnuplot` - Climber Best (10 runs)
- `output_ils_<instance>.gnuplot` - Iterated Local Search (5 runs)
- `output_marche_aleatoire_<instance>.gnuplot` - Random Walk (10 runs)

**Cas d'usage:**
- Comparer les performances de base
- Valider les implémentations d'opérateurs
- Baseline de comparaison

---

## 🚀 Script d'Automatisation Complet

```bash
chmod +x run_benchmarks.sh
./run_benchmarks.sh
```

Ce script interactive:
1. Compile prog
2. Compile prog_climbers
3. Propose d'exécuter prog
4. Propose d'exécuter prog_climbers
5. Lance l'analyse Python

---

## 📈 Comparaison des Trois Approches

| Feature | main.c | main_multiple_runs.c | main_climbers.c |
|---------|--------|----------------------|-----------------|
| **Temps d'exécution** | ~10 min | ~60 min | ~30 min |
| **Runs par algo** | 1 | 10 | 5-10 |
| **Mesure variabilité** | Non | ✓ Oui | Oui |
| **Hypervolume** | ✓ Oui | Non | Non |
| **Comparaison Pareto/Scalaire** | ✓ Oui (combiné) | ✓ Oui (séparé) | N/A |
| **Climbers testés** | Non | Non | ✓ Oui |
| **Graphiques séparés** | Non | ✓ Oui | Non |
| **Taille fichiers** | Petite | Grande | Moyenne |

---

## 📊 Recommandations

**Pour un test rapide:**
```bash
./prog
python3 analyze_pareto.py
```

**Pour une analyse robuste:**
```bash
./run_multiple_runs.sh
```

**Pour valider les climbers:**
```bash
./prog_climbers
```

**Pour tout tester:**
```bash
./run_benchmarks.sh
```

---

## 🎯 Points Clés - MULTIPLE RUNS

- ✓ **Fronts séparés:** Pareto et Scalaire dans des graphiques différents
- ✓ **10 runs:** Pour chaque algo, 10 exécutions distinctes
- ✓ **Par instance:** Graphiques séparés pour chaque instance
- ✓ **Couleurs progressives:** Bleu pour Pareto, Rouge pour Scalaire
- ✓ **Statistiques:** Min, Max, Moyenne des tailles de front
- ✓ **Dossier plots:** Tous les graphiques sauvegardés

---

## 📝 Format des Fichiers Gnuplot

Chaque fichier contient 3 colonnes:
```
makespan tardiness dominated_flag
1737 5541 0
1738 5485 0
...
```

- **makespan:** Temps d'achèvement total
- **tardiness:** Retard total cumulé
- **dominated_flag:** 0 = non-dominé, 1 = dominé

---

## 🔧 Personnalisation

Pour modifier le nombre de runs dans `main_multiple_runs.c`:

```c
#define NUM_RUNS 10  // Changer cette valeur
```

Pour modifier le nombre de scalarisations:

```c
int nb_scalarizations = 20;  // Augmenter pour plus de couverture
```

---

**Dernière mise à jour:** Janvier 2026

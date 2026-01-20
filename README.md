# Benchmark Multi-Algorithmes - Front Pareto

Ce projet implémente et compare différents algorithmes pour explorer le front Pareto dans un problème d'ordonnancement d'ateliers (Flow Shop Scheduling) avec deux objectifs : **Makespan** et **Tardiness**.

## Structure du Projet

### Fichiers Principaux

- **`main.c`** : Programme de test des algorithmes Pareto et Scalaire
  - `algo_pareto()` : Algorithme Pareto Local Search avec filtrage en ligne
  - `algo_scalaire()` : Algorithme scalaire (somme pondérée des critères)

- **`main_climbers.c`** : Programme de test des algorithmes de recherche locale
  - `climber_first()` : Recherche locale avec critère du premier meilleur
  - `climber_best()` : Recherche locale avec critère du meilleur
  - `iterated_local_search()` : Recherche locale itérée avec perturbation
  - `marche_aleatoire()` : Marche aléatoire pour exploration

- **`solution.c/solution.h`** : Implémentation des algorithmes

- **`analyze_pareto.py`** : Script Python pour analyser et visualiser les résultats
  - Calcul de l'hypervolume
  - Génération de graphiques comparatifs
  - Résumé des fronts Pareto

### Algorithmes Implémentés

#### 1. Algorithme Pareto Local Search (PLS)
- Génère une solution initiale aléatoire
- À chaque itération :
  1. Génère tous les voisins de chaque solution de l'archive
  2. Ajoute les voisins non-dominés via **filtrage en ligne**
  3. Continue jusqu'à convergence (pas d'amélioration)
- Point fort : Exploration équilibrée du front Pareto

#### 2. Algorithme Scalaire
- Teste différents vecteurs de poids (0 à 1)
- Pour chaque poids :
  1. Génère une solution aléatoire
  2. Applique climber_best avec coût scalaire
  3. Ajoute à l'archive via **filtrage en ligne**
- Point fort : Couverture systématique du front

#### 3. Climber First
- Première amélioration trouvée est acceptée
- Rapide mais peut rester bloqué

#### 4. Climber Best
- Explore tous les voisins et choisit le meilleur
- Plus lent mais meilleure qualité

#### 5. Iterated Local Search (ILS)
- Applique perturbations après chaque optimum local
- Permet l'échappement des optima locaux
- Équilibre entre exploration et exploitation

#### 6. Marche Aléatoire
- Effectue des opérations aléatoires
- Baseline pour comparaison

## Utilisation

### Compilation et Exécution Automatique

```bash
./run_benchmarks.sh
```

Ce script :
1. Compile les deux versions (prog et prog_climbers)
2. Demande quels benchmarks exécuter
3. Lance l'analyse Python des résultats

### Compilation Manuelle

**Version Pareto & Scalaire :**
```bash
gcc -o prog main.c solution.c instance.c helper.c test.c -lm
./prog
```

**Version Climbers :**
```bash
gcc -o prog_climbers main_climbers.c solution.c instance.c helper.c test.c -lm
./prog_climbers
```

### Analyse des Résultats

```bash
python3 analyze_pareto.py
```

Génère :
- `pareto_comparison.png` : Graphique comparatif des fronts Pareto
- Affiche les statistiques d'hypervolume

## Fichiers de Sortie

### Pareto & Scalaire
- `output_pareto_<instance>.gnuplot` : Front Pareto du PLS
- `output_scalaire_<instance>.gnuplot` : Front Pareto de l'algorithme scalaire

### Climbers
- `output_climber_first_<instance>.gnuplot` : Résultats Climber First
- `output_climber_best_<instance>.gnuplot` : Résultats Climber Best
- `output_ils_<instance>.gnuplot` : Résultats ILS
- `output_marche_aleatoire_<instance>.gnuplot` : Résultats Marche Aléatoire

Format de fichier (makespan, tardiness, dominated) :
```
1737 5541 0
1738 5485 0
...
```

## Instances de Test

Les instances sont dans le répertoire `instances/` :
- `3_3_01.txt` : 3 jobs, 3 machines
- `7_5_01.txt` : 7 jobs, 5 machines
- `20_10_01.txt` : 20 jobs, 10 machines
- `20_20_01.txt` : 20 jobs, 20 machines
- etc.

## Métriques d'Évaluation

### Hypervolume
- Mesure la surface du front Pareto par rapport à un point de référence
- Point de référence : max(makespan) × 1.1, max(tardiness) × 1.1
- Plus élevé = meilleur front

### Taille du Front
- Nombre de solutions non-dominées
- Plus grand = meilleure diversité

### Makespan et Tardiness
- Min, Max, Moyenne pour chaque critère

## Filtrage

### Filtrage En Ligne
Utilisé par **Pareto** et **Scalaire** :
- Ajoute progressivement les solutions
- Supprime les solutions dominées
- Limite la taille maximale de l'archive
- **Avantage** : Converge vers le même front

### Filtrage Hors Ligne
Non utilisé dans ces benchmarks
- Filtre toutes les solutions à la fin
- Peut surcharger la mémoire

## Remarques Importantes

1. **Filtrage cohérent** : Pareto et Scalaire utilisent maintenant le même filtrage en ligne pour une comparaison équitable

2. **Hypervolume** : Calculé automatiquement pour chaque front avec point de référence adapté

3. **Graine aléatoire** : Fixée à `123456789` pour reproductibilité

4. **Normalisation** : Pas appliquée, les coûts bruts sont utilisés

## Extensions Possibles

- Tester INSERTION au lieu de ECHANGE
- Augmenter le nombre de scalarisations
- Implémenter d'autres opérateurs (2-opt, 3-opt, etc.)
- Ajouter des critères supplémentaires
- Paralléliser les calculs

## Notes de Développement

- Tous les algorithmes utilisent des allocations dynamiques
- La gestion mémoire est effectuée avec `malloc/free`
- Les solutions sont copiées pour éviter les modifications accidentelles
- Les fichiers de sortie sont au format texte pour flexibilité

---

**Version**: 1.0  
**Date**: Janvier 2026  
**Auteur**: TPIA - Optimization Algorithms

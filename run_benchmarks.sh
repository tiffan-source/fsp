#!/bin/bash

# Script pour compiler et exécuter les différentes versions du benchmark

echo "╔════════════════════════════════════════════════════════╗"
echo "║ COMPILATION DES BENCHMARKS"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Compiler la version Pareto/Scalaire
echo "[1/2] Compilation de prog (Pareto & Scalaire)..."
gcc -o prog main.c solution.c instance.c helper.c test.c -lm
if [ $? -eq 0 ]; then
    echo "✓ Compilation de prog réussie"
else
    echo "✗ Erreur lors de la compilation de prog"
    exit 1
fi

echo ""

# Compiler la version Climbers
echo "[2/2] Compilation de prog_climbers (Climber algorithms)..."
gcc -o prog_climbers main_climbers.c solution.c instance.c helper.c test.c -lm
if [ $? -eq 0 ]; then
    echo "✓ Compilation de prog_climbers réussie"
else
    echo "✗ Erreur lors de la compilation de prog_climbers"
    exit 1
fi

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║ EXÉCUTION DES BENCHMARKS"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

read -p "Voulez-vous exécuter prog (Pareto & Scalaire) ? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Exécution de prog..."
    ./prog
    echo ""
fi

read -p "Voulez-vous exécuter prog_climbers (Climber algorithms) ? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Exécution de prog_climbers..."
    ./prog_climbers
    echo ""
fi

echo "╔════════════════════════════════════════════════════════╗"
echo "║ ANALYSE DES RÉSULTATS"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

read -p "Voulez-vous lancer l'analyse des résultats ? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Exécution du script d'analyse..."
    python3 analyze_pareto.py
    echo ""
fi

echo "✓ Processus terminé"

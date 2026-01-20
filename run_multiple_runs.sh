#!/bin/bash

# Script pour compiler et exécuter la version multiple runs

echo "╔════════════════════════════════════════════════════════╗"
echo "║ COMPILATION ET EXÉCUTION - MULTIPLE RUNS"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Compiler la version Multiple Runs
echo "[1/3] Compilation de prog_multiple_runs..."
gcc -Ofast -o prog_multiple_runs main_multiple_runs.c solution.c instance.c helper.c test.c -lm
if [ $? -eq 0 ]; then
    echo "✓ Compilation réussie"
else
    echo "✗ Erreur lors de la compilation"
    exit 1
fi

echo ""
echo "[2/3] Exécution du benchmark..."
echo ""

./prog_multiple_runs

echo ""
echo "[3/3] Visualisation des résultats..."
echo ""

python3 visualize_multiple_runs.py

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║ PROCESSUS TERMINÉ"
echo "╚════════════════════════════════════════════════════════╝"

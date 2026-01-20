#!/usr/bin/env python3
"""
Résumé rapide des 3 versions de main
Affiche les différences et recommandations
"""

import os

def print_header(text):
    print("\n" + "="*80)
    print(f"  {text}")
    print("="*80)

def main():
    print_header("📊 TROIS VERSIONS DE MAIN - RÉSUMÉ COMPLET")
    
    versions = {
        "main.c": {
            "nom": "Version Simple",
            "runs": 1,
            "temps": "~10 minutes",
            "command": "gcc -o prog main.c solution.c instance.c helper.c test.c -lm && ./prog",
            "output": "output_pareto_<instance>.gnuplot, output_scalaire_<instance>.gnuplot",
            "visualization": "python3 analyze_pareto.py",
            "use_cases": [
                "Test rapide du concept",
                "Vérification que tout fonctionne",
                "Calcul d'hypervolume",
                "Comparaison rapide Pareto vs Scalaire"
            ],
            "avantages": [
                "Exécution rapide",
                "Fichiers de sortie simples",
                "Calcul d'hypervolume"
            ],
            "inconvenients": [
                "Une seule exécution par algo",
                "Pas de mesure de variabilité",
                "Fronts mélangés dans le graphique"
            ]
        },
        "main_multiple_runs.c": {
            "nom": "Version Robuste (RECOMMANDÉE) ⭐",
            "runs": 10,
            "temps": "~60 minutes",
            "command": "chmod +x run_multiple_runs.sh && ./run_multiple_runs.sh",
            "output": "output_pareto_multiple_<instance>_run<N>.gnuplot (N=1..10)",
            "visualization": "python3 visualize_multiple_runs.py",
            "use_cases": [
                "Évaluer la stabilité des algos",
                "Mesurer la variabilité",
                "Analyse statistique",
                "Publication scientifique",
                "Rapport d'analyse complète"
            ],
            "avantages": [
                "10 exécutions indépendantes",
                "Fronts séparés (Pareto vs Scalaire)",
                "Statistiques robustes",
                "Mesure de variabilité",
                "Graphiques professionnels"
            ],
            "inconvenients": [
                "Temps d'exécution plus long",
                "Plus de fichiers générés",
                "Requiert plus d'espace disque"
            ]
        },
        "main_climbers.c": {
            "nom": "Version Climbers",
            "runs": "5-10",
            "temps": "~30 minutes",
            "command": "gcc -o prog_climbers main_climbers.c solution.c instance.c helper.c test.c -lm && ./prog_climbers",
            "output": "output_climber_first/best/ils/marche_aleatoire_<instance>.gnuplot",
            "visualization": "Fichiers gnuplot bruts",
            "use_cases": [
                "Tester les climbers",
                "Comparer les opérateurs",
                "Validation des implémentations",
                "Baseline de comparaison"
            ],
            "avantages": [
                "Test des algo basiques",
                "Validation opérateurs",
                "Baseline pour comparaison"
            ],
            "inconvenients": [
                "Pas d'algo Pareto/Scalaire",
                "Pas de visualization automatique",
                "Moins pertinent pour le projet"
            ]
        }
    }
    
    for version, details in versions.items():
        print_header(f"{details['nom']}")
        print(f"\n📌 Fichier: {version}")
        print(f"🔄 Runs par algorithme: {details['runs']}")
        print(f"⏱️  Temps d'exécution: {details['temps']}")
        
        print(f"\n🚀 Exécution rapide:")
        print(f"  {details['command']}")
        
        print(f"\n📊 Fichiers de sortie:")
        print(f"  {details['output']}")
        
        print(f"\n📈 Visualisation:")
        print(f"  {details['visualization']}")
        
        print(f"\n💡 Cas d'usage:")
        for i, case in enumerate(details['use_cases'], 1):
            print(f"  {i}. {case}")
        
        print(f"\n✅ Avantages:")
        for adv in details['avantages']:
            print(f"  • {adv}")
        
        print(f"\n❌ Inconvénients:")
        for inc in details['inconvenients']:
            print(f"  • {inc}")
    
    print_header("🎯 RECOMMANDATIONS")
    print("""
┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│  Pour UN test rapide:                                                       │
│  └─ main.c (~10 min)                                                        │
│                                                                             │
│  Pour UNE analyse ROBUSTE (RECOMMANDÉ):                                    │
│  └─ main_multiple_runs.c (~60 min) ⭐⭐⭐                                   │
│                                                                             │
│  Pour TOUS les tests:                                                       │
│  └─ ./run_benchmarks.sh (menu interactif)                                  │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
    """)
    
    print_header("📊 COMPARAISON DÉTAILLÉE")
    
    comparison = f"""
╔═══════════════════╦═════════════════════╦════════════════════╦══════════════════╗
║ Caractéristique   ║  main.c             ║  main_multiple_   ║  main_climbers.c ║
║                   ║  (Simple)           ║  runs.c (Robuste) ║  (Climbers)      ║
╠═══════════════════╬═════════════════════╬════════════════════╬══════════════════╣
║ Temps exécution   ║ ~10 min             ║ ~60 min            ║ ~30 min          ║
║ Runs/algo         ║ 1                   ║ 10                 ║ 5-10             ║
║ Pareto inclus     ║ ✓ Oui               ║ ✓ Oui              ║ Non              ║
║ Scalaire inclus   ║ ✓ Oui               ║ ✓ Oui              ║ Non              ║
║ Climbers inclus   ║ Non                 ║ Non                ║ ✓ Oui            ║
║ Fronts séparés    ║ Non (mélangés)      ║ ✓ Oui              ║ N/A              ║
║ Hypervolume       ║ ✓ Oui               ║ Non                ║ Non              ║
║ Statistiques      ║ Basiques            ║ ✓ Complètes        ║ Basiques         ║
║ Variabilité mesu. ║ Non                 ║ ✓ Oui              ║ Oui              ║
║ Graphiques        ║ Combiné             ║ ✓ Séparés          ║ Aucun            ║
║ Récit scientif.   ║ Non                 ║ ✓ Oui              ║ Non              ║
╚═══════════════════╩═════════════════════╩════════════════════╩══════════════════╝
    """
    
    print(comparison)
    
    print_header("🔥 POINTS CLÉS POUR MAIN_MULTIPLE_RUNS.C")
    print("""
✓ FRONTS SÉPARÉS: Pareto dans un graphique, Scalaire dans un autre
  └─ Pas de mélange, comparaison claire

✓ 10 RUNS INDÉPENDANTS: Chaque algo tourne 10 fois
  └─ Mesure la variabilité et la stabilité

✓ PAR INSTANCE: Graphiques séparés pour chaque fichier d'instance
  └─ Voir les comportements spécifiques

✓ STATISTIQUES: Min, Max, Moyenne pour chaque front
  └─ Analyse quantitative

✓ COULEURS PROGRESSIVES:
  └─ Pareto: Bleu clair (run 1) → Bleu foncé (run 10)
  └─ Scalaire: Rouge clair (run 1) → Rouge foncé (run 10)

✓ AUTOMATISÉ: Script shell complet
  └─ Compilation + Exécution + Visualisation en une commande
    """)
    
    print_header("📌 COMMANDE MAGIQUE")
    print("""
Pour lancer TOUT automatiquement:

  ./run_multiple_runs.sh

Cela fait:
  1. Compile main_multiple_runs.c en prog_multiple_runs
  2. Lance prog_multiple_runs (exécute 10×2 algos sur toutes instances)
  3. Lance visualize_multiple_runs.py (génère les graphiques séparés)
  4. Crée le dossier plots/ avec tous les PNG

Résultat: Des graphiques professionnels, prêts pour une présentation!
    """)
    
    print_header("🎓 INTERPRÉTATION DES GRAPHIQUES")
    print("""
MAIN_MULTIPLE_RUNS.C - Exemple Instance: 20_10_01

Graphique 1: "pareto_20_10_01.png"
┌──────────────────────────────────────────────────────────┐
│ Algoritmo Pareto - 10 Runs                               │
│                                                          │
│ Tardiness  ▲                                             │
│           6000 ┼ ╲                  10 (bleu foncé)      │
│           5500 ┼  ╲ 1 (bleu clair)                       │
│           5000 ┼   ╲ ╲ 2,3,4...                         │
│           4500 ┼    ╲ ╲ ╲ ╲ ╲                           │
│           4000 ┼     ╲ ╲ ╲ ╲ ╲ 10                       │
│           3500 ┼      ╲ ╲ ╲ ╲ ╲                         │
│                ╰──────────────────────────── Makespan    │
│                1700   1750   1800   1850                 │
└──────────────────────────────────────────────────────────┘
Interprétation: Tous les runs trouvent un front similaire
                (bonne stabilité)

Graphique 2: "scalaire_20_10_01.png"
┌──────────────────────────────────────────────────────────┐
│ Algoritmo Scalaire - 10 Runs                             │
│                                                          │
│ Tardiness  ▲                                             │
│           9000 ┼ 1 (rouge clair)                         │
│           8500 ┼ ╲ 2,3...10 (rouge progressif)          │
│           8000 ┼  ╲ ╲                                   │
│           7500 ┼   ╲ ╲ ╲                                │
│           7000 ┼    ╲ ╲ ╲ ╲ 10 (rouge foncé)           │
│           6500 ┼     ╲ ╲ ╲ ╲                           │
│                ╰──────────────────────────── Makespan    │
│                2400   2500   2600   2700                 │
└──────────────────────────────────────────────────────────┘
Interprétation: Scalaire génère moins de solutions mais
                plus concentrées. Front plus petit mais
                couverture systématique.
    """)
    
    print_header("✅ ÉTAPES POUR DÉMARRER")
    print("""
1. Vérifier les prérequis:
   gcc --version
   python3 --version
   pip3 install matplotlib numpy

2. Rendre exécutable:
   chmod +x run_multiple_runs.sh

3. LANCER LE BENCHMARK:
   ./run_multiple_runs.sh

4. ATTENDRE (~60 minutes):
   Voir les logs de progression

5. VÉRIFIER LES RÉSULTATS:
   ls -lh plots/
   open plots/  (ou regarder avec votre visionneuse)

6. ANALYSER LES RÉSULTATS:
   Comparer les 10 fronts Pareto
   Comparer les 10 fronts Scalaire
   Évaluer la variabilité
    """)
    
    print("\n" + "="*80)
    print("  FIN DU RÉSUMÉ")
    print("="*80 + "\n")

if __name__ == "__main__":
    main()

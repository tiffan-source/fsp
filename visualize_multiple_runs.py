#!/usr/bin/env python3
"""
Script de visualisation des fronts Pareto pour multiple runs
Affiche les 10 runs séparément pour Pareto et Scalaire par instance
"""

import os
import glob
import numpy as np
from collections import defaultdict
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

class MultipleRunsAnalyzer:
    """Analyse les fichiers gnuplot pour multiple runs"""
    
    def __init__(self):
        self.results = defaultdict(lambda: {'pareto': [], 'scalaire': []})
        
    def read_gnuplot_file(self, filepath):
        """
        Lit un fichier gnuplot et retourne les solutions
        Format: makespan tardiness dominated_flag
        """
        solutions = []
        try:
            with open(filepath, 'r') as f:
                for line in f:
                    line = line.strip()
                    if line and not line.startswith('#'):
                        parts = line.split()
                        if len(parts) >= 3:
                            makespan = int(parts[0])
                            tardiness = int(parts[1])
                            solutions.append({
                                'makespan': makespan,
                                'tardiness': tardiness
                            })
        except Exception as e:
            print(f"Erreur lors de la lecture de {filepath}: {e}")
            return []
        return solutions
    
    def analyze_all(self, directory='.'):
        """Analyse tous les fichiers gnuplot du répertoire"""
        pareto_files = sorted(glob.glob(os.path.join(directory, 'output_pareto_multiple_*.gnuplot')))
        scalaire_files = sorted(glob.glob(os.path.join(directory, 'output_scalaire_multiple_*.gnuplot')))
        
        print(f"Fichiers Pareto trouvés: {len(pareto_files)}")
        print(f"Fichiers Scalaire trouvés: {len(scalaire_files)}\n")
        
        # Parser les fichiers Pareto
        for filepath in pareto_files:
            filename = os.path.basename(filepath)
            # Extraire instance et run number
            # Format: output_pareto_multiple_<instance>_run<N>.gnuplot
            parts = filename.replace('output_pareto_multiple_', '').replace('.gnuplot', '').rsplit('_run', 1)
            if len(parts) == 2:
                instance_name = parts[0]
                solutions = self.read_gnuplot_file(filepath)
                self.results[instance_name]['pareto'].append(solutions)
        
        # Parser les fichiers Scalaire
        for filepath in scalaire_files:
            filename = os.path.basename(filepath)
            # Extraire instance et run number
            # Format: output_scalaire_multiple_<instance>_run<N>.gnuplot
            parts = filename.replace('output_scalaire_multiple_', '').replace('.gnuplot', '').rsplit('_run', 1)
            if len(parts) == 2:
                instance_name = parts[0]
                solutions = self.read_gnuplot_file(filepath)
                self.results[instance_name]['scalaire'].append(solutions)
        
        return self.results
    
    def plot_pareto_multiple_runs(self, output_dir='plots'):
        """Crée des graphiques séparés pour chaque algorithme par instance"""
        
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)
        
        instances = sorted(self.results.keys())
        
        # Créer des graphiques pour chaque instance
        for instance_name in instances:
            data = self.results[instance_name]
            
            # Graphique PARETO
            if data['pareto']:
                fig, ax = plt.subplots(figsize=(12, 8))
                
                pareto_runs = data['pareto']
                colors = plt.cm.Blues(np.linspace(0.3, 0.9, len(pareto_runs)))
                
                for idx, front in enumerate(pareto_runs):
                    if front:
                        makespan_vals = sorted([s['makespan'] for s in front])
                        tardiness_vals = [s['tardiness'] for s in sorted(front, key=lambda x: x['makespan'])]
                        ax.plot(makespan_vals, tardiness_vals, marker='o', 
                               label=f'Run {idx+1} (n={len(front)})',
                               color=colors[idx], linewidth=1.5, markersize=4, alpha=0.7)
                
                ax.set_xlabel('Makespan', fontsize=12, fontweight='bold')
                ax.set_ylabel('Tardiness', fontsize=12, fontweight='bold')
                ax.set_title(f'Algoritmo Pareto - Instance: {instance_name}', 
                           fontsize=14, fontweight='bold')
                ax.legend(fontsize=9, loc='best', ncol=2)
                ax.grid(True, alpha=0.3)
                
                output_file = os.path.join(output_dir, f'pareto_{instance_name}.png')
                plt.tight_layout()
                plt.savefig(output_file, dpi=150, bbox_inches='tight')
                print(f"✓ Graphique Pareto sauvegardé: {output_file}")
                plt.close()
            
            # Graphique SCALAIRE
            if data['scalaire']:
                fig, ax = plt.subplots(figsize=(12, 8))
                
                scalaire_runs = data['scalaire']
                colors = plt.cm.Reds(np.linspace(0.3, 0.9, len(scalaire_runs)))
                
                for idx, front in enumerate(scalaire_runs):
                    if front:
                        makespan_vals = sorted([s['makespan'] for s in front])
                        tardiness_vals = [s['tardiness'] for s in sorted(front, key=lambda x: x['makespan'])]
                        ax.plot(makespan_vals, tardiness_vals, marker='s', 
                               label=f'Run {idx+1} (n={len(front)})',
                               color=colors[idx], linewidth=1.5, markersize=4, alpha=0.7)
                
                ax.set_xlabel('Makespan', fontsize=12, fontweight='bold')
                ax.set_ylabel('Tardiness', fontsize=12, fontweight='bold')
                ax.set_title(f'Algoritmo Scalaire - Instance: {instance_name}', 
                           fontsize=14, fontweight='bold')
                ax.legend(fontsize=9, loc='best', ncol=2)
                ax.grid(True, alpha=0.3)
                
                output_file = os.path.join(output_dir, f'scalaire_{instance_name}.png')
                plt.tight_layout()
                plt.savefig(output_file, dpi=150, bbox_inches='tight')
                print(f"✓ Graphique Scalaire sauvegardé: {output_file}")
                plt.close()
    
    def print_summary(self):
        """Affiche un résumé des runs"""
        print("\n" + "="*80)
        print("RÉSUMÉ DES MULTIPLE RUNS")
        print("="*80)
        
        instances = sorted(self.results.keys())
        
        for instance_name in instances:
            data = self.results[instance_name]
            
            print(f"\n{'Instance:':<20} {instance_name}")
            print("-" * 80)
            
            # Stats Pareto
            if data['pareto']:
                sizes = [len(front) for front in data['pareto']]
                print(f"\nAlgoritmo Pareto ({len(data['pareto'])} runs):")
                print(f"  Tailles: {sizes}")
                print(f"  Min: {min(sizes)}, Max: {max(sizes)}, Média: {np.mean(sizes):.1f}")
                
                # Estadísticas de cada frontera
                for idx, front in enumerate(data['pareto']):
                    if front:
                        makespan_vals = [s['makespan'] for s in front]
                        tardiness_vals = [s['tardiness'] for s in front]
                        print(f"    Run {idx+1}: makespan=[{min(makespan_vals)}, {max(makespan_vals)}], " +
                              f"tardiness=[{min(tardiness_vals)}, {max(tardiness_vals)}]")
            
            # Stats Scalaire
            if data['scalaire']:
                sizes = [len(front) for front in data['scalaire']]
                print(f"\nAlgoritmo Scalaire ({len(data['scalaire'])} runs):")
                print(f"  Tailles: {sizes}")
                print(f"  Min: {min(sizes)}, Max: {max(sizes)}, Média: {np.mean(sizes):.1f}")
                
                # Estadísticas de cada frontera
                for idx, front in enumerate(data['scalaire']):
                    if front:
                        makespan_vals = [s['makespan'] for s in front]
                        tardiness_vals = [s['tardiness'] for s in front]
                        print(f"    Run {idx+1}: makespan=[{min(makespan_vals)}, {max(makespan_vals)}], " +
                              f"tardiness=[{min(tardiness_vals)}, {max(tardiness_vals)}]")


def main():
    analyzer = MultipleRunsAnalyzer()
    
    print("\n" + "="*80)
    print("ANALYSE MULTIPLE RUNS - FRONTS PARETO")
    print("="*80 + "\n")
    
    results = analyzer.analyze_all('.')
    
    if results:
        # Afficher le résumé
        analyzer.print_summary()
        
        # Créer les graphiques séparés
        print("\n" + "="*80)
        print("GÉNÉRATION DES GRAPHIQUES")
        print("="*80 + "\n")
        
        analyzer.plot_pareto_multiple_runs('plots')
        
        print("\n" + "="*80)
        print("ANALYSE TERMINÉE")
        print("="*80)
        print("\nGraphiques générés dans le répertoire 'plots/'")
    else:
        print("Aucun fichier de résultats trouvé")


if __name__ == '__main__':
    main()

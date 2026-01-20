#!/usr/bin/env python3
"""
Script d'analyse des fronts Pareto générés par les algorithmes
Calcule l'hypervolume et affiche les fronts Pareto
"""

import os
import glob
import numpy as np
from collections import defaultdict
import matplotlib.pyplot as plt

class ParetoAnalyzer:
    """Analyse les fichiers gnuplot et calcule les métriques Pareto"""
    
    def __init__(self, reference_point=None):
        """
        Initialise l'analyseur
        reference_point: tuple (makespan_ref, tardiness_ref) pour le calcul d'hypervolume
        Si None, sera calculé automatiquement comme max + 10%
        """
        self.reference_point = reference_point
        self.results = defaultdict(dict)
        
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
                            dominated = int(parts[2])
                            solutions.append({
                                'makespan': makespan,
                                'tardiness': tardiness,
                                'dominated': dominated
                            })
        except Exception as e:
            print(f"Erreur lors de la lecture de {filepath}: {e}")
            return []
        return solutions
    
    def get_pareto_front(self, solutions):
        """
        Extrait le front Pareto des solutions
        Solutions non-dominées sont celles avec dominated=0
        """
        non_dominated = [s for s in solutions if s['dominated'] == 0]
        return sorted(non_dominated, key=lambda x: x['makespan'])
    
    def calculate_hypervolume(self, front, reference_point=None):
        """
        Calcule l'hypervolume du front Pareto
        Utilise l'algorithme WFG (pour 2D c'est simple)
        
        reference_point: tuple (makespan_ref, tardiness_ref)
        """
        if len(front) == 0:
            return 0.0
        
        if reference_point is None:
            # Utiliser automatiquement max + 10%
            max_makespan = max(s['makespan'] for s in front)
            max_tardiness = max(s['tardiness'] for s in front)
            reference_point = (
                int(max_makespan * 1.1),
                int(max_tardiness * 1.1)
            )
        
        # Trier par makespan (croissant)
        sorted_front = sorted(front, key=lambda x: x['makespan'])
        
        hypervolume = 0.0
        prev_tardiness = reference_point[1]
        
        for i, solution in enumerate(sorted_front):
            makespan = solution['makespan']
            tardiness = solution['tardiness']
            
            # Largeur: différence entre le point actuel et le suivant (ou référence)
            if i < len(sorted_front) - 1:
                next_makespan = sorted_front[i + 1]['makespan']
            else:
                next_makespan = reference_point[0]
            
            width = next_makespan - makespan
            height = prev_tardiness - tardiness
            
            if height > 0:
                hypervolume += width * height
            
            prev_tardiness = tardiness
        
        return hypervolume, reference_point
    
    def analyze_file(self, filepath):
        """Analyse un fichier gnuplot et retourne les statistiques"""
        filename = os.path.basename(filepath)
        print(f"\n{'='*70}")
        print(f"Analyse: {filename}")
        print('='*70)
        
        # Lire les solutions
        solutions = self.read_gnuplot_file(filepath)
        
        if not solutions:
            print("Erreur: Aucune solution trouvée")
            return None
        
        print(f"Nombre total de solutions: {len(solutions)}")
        
        # Extraire le front Pareto
        pareto_front = self.get_pareto_front(solutions)
        dominated_count = sum(1 for s in solutions if s['dominated'] == 1)
        
        print(f"Solutions dominées: {dominated_count}")
        print(f"Solutions non-dominées (Front Pareto): {len(pareto_front)}")
        
        # Calculer les statistiques
        makespan_values = [s['makespan'] for s in pareto_front]
        tardiness_values = [s['tardiness'] for s in pareto_front]
        
        print(f"\nMakespan:")
        print(f"  Min: {min(makespan_values)}")
        print(f"  Max: {max(makespan_values)}")
        print(f"  Moyenne: {np.mean(makespan_values):.2f}")
        
        print(f"\nTardiness:")
        print(f"  Min: {min(tardiness_values)}")
        print(f"  Max: {max(tardiness_values)}")
        print(f"  Moyenne: {np.mean(tardiness_values):.2f}")
        
        # Calculer l'hypervolume
        hv, ref_point = self.calculate_hypervolume(pareto_front, self.reference_point)
        print(f"\nPoint de référence: {ref_point}")
        print(f"Hypervolume: {hv:.2f}")
        
        # Afficher le front Pareto
        print(f"\nFront Pareto ({len(pareto_front)} solutions):")
        print(f"{'Rank':<6} {'Makespan':<12} {'Tardiness':<12} {'Somme':<12}")
        print("-" * 42)
        for i, solution in enumerate(pareto_front, 1):
            somme = solution['makespan'] + solution['tardiness']
            print(f"{i:<6} {solution['makespan']:<12} {solution['tardiness']:<12} {somme:<12}")
        
        return {
            'filename': filename,
            'total_solutions': len(solutions),
            'pareto_solutions': len(pareto_front),
            'dominated_solutions': dominated_count,
            'hypervolume': hv,
            'reference_point': ref_point,
            'pareto_front': pareto_front,
            'makespan_min': min(makespan_values),
            'makespan_max': max(makespan_values),
            'tardiness_min': min(tardiness_values),
            'tardiness_max': max(tardiness_values),
        }
    
    def analyze_all(self, directory='.'):
        """Analyse tous les fichiers gnuplot du répertoire"""
        pattern = os.path.join(directory, 'output_*.gnuplot')
        files = sorted(glob.glob(pattern))
        
        if not files:
            print(f"Aucun fichier gnuplot trouvé dans {directory}")
            return {}
        
        print(f"\nFichiers trouvés: {len(files)}")
        
        all_results = {}
        for filepath in files:
            result = self.analyze_file(filepath)
            if result:
                all_results[result['filename']] = result
        
        return all_results
    
    def plot_pareto_fronts(self, results, output_file='pareto_fronts.png'):
        """Crée un graphique avec tous les fronts Pareto"""
        
        # Grouper par type d'algo
        pareto_results = {}
        scalaire_results = {}
        
        for filename, result in results.items():
            if 'pareto' in filename.lower():
                instance_name = filename.replace('output_pareto_', '').replace('.gnuplot', '')
                pareto_results[instance_name] = result
            elif 'scalaire' in filename.lower():
                instance_name = filename.replace('output_scalaire_', '').replace('.gnuplot', '')
                scalaire_results[instance_name] = result
        
        # Créer les graphiques
        instances = sorted(set(list(pareto_results.keys()) + list(scalaire_results.keys())))
        n_instances = len(instances)
        
        if n_instances == 0:
            print("Aucun résultat à tracer")
            return
        
        # Calculer la grille
        cols = min(3, n_instances)
        rows = (n_instances + cols - 1) // cols
        
        fig, axes = plt.subplots(rows, cols, figsize=(15, 5*rows))
        if rows == 1 and cols == 1:
            axes = np.array([[axes]])
        elif rows == 1 or cols == 1:
            axes = axes.reshape(rows, cols)
        
        for idx, instance_name in enumerate(instances):
            ax = axes.flat[idx] if isinstance(axes.flat, np.ndarray) else axes[idx // cols, idx % cols]
            
            # Tracer Pareto
            if instance_name in pareto_results:
                result = pareto_results[instance_name]
                front = result['pareto_front']
                makespan_vals = [s['makespan'] for s in front]
                tardiness_vals = [s['tardiness'] for s in front]
                ax.plot(makespan_vals, tardiness_vals, 'b-o', label='Pareto', linewidth=2, markersize=6)
            
            # Tracer Scalaire
            if instance_name in scalaire_results:
                result = scalaire_results[instance_name]
                front = result['pareto_front']
                makespan_vals = [s['makespan'] for s in front]
                tardiness_vals = [s['tardiness'] for s in front]
                ax.plot(makespan_vals, tardiness_vals, 'r-s', label='Scalaire', linewidth=2, markersize=6)
            
            ax.set_xlabel('Makespan', fontsize=10)
            ax.set_ylabel('Tardiness', fontsize=10)
            ax.set_title(f'Instance: {instance_name}', fontsize=11, fontweight='bold')
            ax.legend(fontsize=9)
            ax.grid(True, alpha=0.3)
        
        # Masquer les axes inutilisés
        for idx in range(n_instances, len(axes.flat)):
            axes.flat[idx].set_visible(False)
        
        plt.tight_layout()
        plt.savefig(output_file, dpi=150, bbox_inches='tight')
        print(f"\nGraphique sauvegardé: {output_file}")
        plt.close()
    
    def print_summary(self, results):
        """Affiche un résumé comparatif"""
        print("\n" + "="*80)
        print("RÉSUMÉ COMPARATIF")
        print("="*80)
        
        # Grouper par instance et algo
        data_by_instance = defaultdict(dict)
        
        for filename, result in results.items():
            instance_name = filename
            # Extraire le nom de l'instance et du type d'algo
            if 'pareto' in filename.lower():
                instance_name = filename.replace('output_pareto_', '').replace('.gnuplot', '')
                key = 'pareto'
            elif 'scalaire' in filename.lower():
                instance_name = filename.replace('output_scalaire_', '').replace('.gnuplot', '')
                key = 'scalaire'
            else:
                continue
            
            data_by_instance[instance_name][key] = result
        
        # Afficher le tableau comparatif
        print(f"\n{'Instance':<15} {'Algo':<12} {'|Pareto|':<10} {'HV':<12} {'Max-Makespan':<14} {'Max-Tardiness':<14}")
        print("-" * 77)
        
        for instance_name in sorted(data_by_instance.keys()):
            results_inst = data_by_instance[instance_name]
            
            for algo_type in ['pareto', 'scalaire']:
                if algo_type in results_inst:
                    result = results_inst[algo_type]
                    algo_display = 'Pareto' if algo_type == 'pareto' else 'Scalaire'
                    print(f"{instance_name:<15} {algo_display:<12} {result['pareto_solutions']:<10} "
                          f"{result['hypervolume']:<12.2f} {result['makespan_max']:<14} {result['tardiness_max']:<14}")


def main():
    import sys
    
    # Point de référence par défaut (sera auto-calculé si None)
    reference_point = None
    
    # Créer l'analyseur
    analyzer = ParetoAnalyzer(reference_point=reference_point)
    
    # Analyser tous les fichiers
    print("\n" + "="*80)
    print("ANALYSE DES FRONTS PARETO")
    print("="*80)
    
    results = analyzer.analyze_all('.')
    
    if results:
        # Afficher le résumé
        analyzer.print_summary(results)
        
        # Créer les graphiques
        analyzer.plot_pareto_fronts(results, 'pareto_comparison.png')
        
        print("\n" + "="*80)
        print("ANALYSE TERMINÉE")
        print("="*80)
    else:
        print("\nAucun résultat trouvé à analyser")


if __name__ == '__main__':
    main()

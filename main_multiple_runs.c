#include "instance.h"
#include "test.h"
#include "solution.h"
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAX_INSTANCES 20
#define MAX_SOLUTIONS 500
#define NUM_RUNS 10
#define MAX_FILENAME 256

// Structure pour stocker les infos d'une instance
typedef struct {
    char filename[MAX_FILENAME];
    char basename[MAX_FILENAME];
} InstanceInfo;

// Récupère la liste des fichiers .txt du répertoire instances
int get_instance_files(const char* instances_dir, InstanceInfo* instances, int max_instances)
{
    DIR* dir = opendir(instances_dir);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 0;
    }

    int count = 0;
    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL && count < max_instances)
    {
        int len = strlen(entry->d_name);
        if (len > 4 && strcmp(entry->d_name + len - 4, ".txt") == 0)
        {
            snprintf(instances[count].filename, MAX_FILENAME, "%s/%s", instances_dir, entry->d_name);
            strncpy(instances[count].basename, entry->d_name, len - 4);
            instances[count].basename[len - 4] = '\0';
            count++;
        }
    }

    closedir(dir);
    return count;
}

// Teste algo_pareto NUM_RUNS fois sur une instance
void test_algo_pareto_multiple(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing ALGO_PARETO (x%d) on instance: %s\n", NUM_RUNS, instance_basename);
    printf("========================================\n");
    
    int all_sizes[NUM_RUNS];
    int all_hypervolumes[NUM_RUNS];
    
    for (int run = 0; run < NUM_RUNS; run++)
    {
        printf("\nRun %d/%d:\n", run + 1, NUM_RUNS);
        
        Solution* archives[MAX_SOLUTIONS];
        int num_solutions = 0;

        // Exécuter algo_pareto
        num_solutions = algo_pareto(instance, archives, MAX_SOLUTIONS, ECHANGE);
        
        all_sizes[run] = num_solutions;
        
        // Tous les résultats sont non-dominés
        int dominated[MAX_SOLUTIONS];
        for (int i = 0; i < num_solutions; i++)
        {
            dominated[i] = 0;
        }

        // Générer le nom du fichier de sortie
        char output_filename[MAX_FILENAME];
        snprintf(output_filename, MAX_FILENAME, "output_pareto_multiple_%s_run%d.gnuplot", 
                 instance_basename, run + 1);

        // Exporter les solutions
        exporter_solutions_gnuplot_flag(instance, archives, dominated, num_solutions, output_filename);

        printf("  Solutions trouvées: %d\n", num_solutions);
        printf("  Exporté vers: %s\n", output_filename);

        // Libérer la mémoire
        for (int i = 0; i < num_solutions; i++)
        {
            free_solution(archives[i]);
        }
    }
    
    // Afficher les statistiques
    printf("\n--- STATISTIQUES ALGO_PARETO ---\n");
    printf("Instance: %s\n", instance_basename);
    printf("Front sizes across %d runs: ", NUM_RUNS);
    for (int i = 0; i < NUM_RUNS; i++)
    {
        printf("%d ", all_sizes[i]);
    }
    printf("\n");
    
    int min_size = all_sizes[0], max_size = all_sizes[0];
    double avg_size = 0;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        if (all_sizes[i] < min_size) min_size = all_sizes[i];
        if (all_sizes[i] > max_size) max_size = all_sizes[i];
        avg_size += all_sizes[i];
    }
    avg_size /= NUM_RUNS;
    
    printf("Min size: %d, Max size: %d, Avg size: %.1f\n\n", min_size, max_size, avg_size);
}

// Teste algo_scalaire NUM_RUNS fois sur une instance
void test_algo_scalaire_multiple(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing ALGO_SCALAIRE (x%d) on instance: %s\n", NUM_RUNS, instance_basename);
    printf("========================================\n");
    
    int all_sizes[NUM_RUNS];
    int nb_scalarizations = 20;
    
    for (int run = 0; run < NUM_RUNS; run++)
    {
        printf("\nRun %d/%d:\n", run + 1, NUM_RUNS);
        
        Solution* archives[MAX_SOLUTIONS];
        int num_solutions = 0;

        // Exécuter algo_scalaire
        num_solutions = algo_scalaire(instance, archives, MAX_SOLUTIONS, nb_scalarizations, ECHANGE);
        
        all_sizes[run] = num_solutions;
        
        // Tous les résultats sont non-dominés
        int dominated[MAX_SOLUTIONS];
        for (int i = 0; i < num_solutions; i++)
        {
            dominated[i] = 0;
        }

        // Générer le nom du fichier de sortie
        char output_filename[MAX_FILENAME];
        snprintf(output_filename, MAX_FILENAME, "output_scalaire_multiple_%s_run%d.gnuplot", 
                 instance_basename, run + 1);

        // Exporter les solutions
        exporter_solutions_gnuplot_flag(instance, archives, dominated, num_solutions, output_filename);

        printf("  Solutions trouvées: %d\n", num_solutions);
        printf("  Exporté vers: %s\n", output_filename);

        // Libérer la mémoire
        for (int i = 0; i < num_solutions; i++)
        {
            free_solution(archives[i]);
        }
    }
    
    // Afficher les statistiques
    printf("\n--- STATISTIQUES ALGO_SCALAIRE ---\n");
    printf("Instance: %s\n", instance_basename);
    printf("Front sizes across %d runs: ", NUM_RUNS);
    for (int i = 0; i < NUM_RUNS; i++)
    {
        printf("%d ", all_sizes[i]);
    }
    printf("\n");
    
    int min_size = all_sizes[0], max_size = all_sizes[0];
    double avg_size = 0;
    for (int i = 0; i < NUM_RUNS; i++)
    {
        if (all_sizes[i] < min_size) min_size = all_sizes[i];
        if (all_sizes[i] > max_size) max_size = all_sizes[i];
        avg_size += all_sizes[i];
    }
    avg_size /= NUM_RUNS;
    
    printf("Min size: %d, Max size: %d, Avg size: %.1f\n\n", min_size, max_size, avg_size);
}

int main(int argc, char *argv[])
{
    const char* instances_dir = "instances";
    InstanceInfo instances[MAX_INSTANCES];
    int num_instances = 0;

    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║ BENCHMARK MULTIPLE RUNS - ALGO PARETO & SCALAIRE\n");
    printf("║ %d runs per algorithm per instance\n", NUM_RUNS);
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    printf("Répertoire des instances: %s\n\n", instances_dir);

    // Récupérer la liste des instances
    num_instances = get_instance_files(instances_dir, instances, MAX_INSTANCES);

    if (num_instances == 0)
    {
        printf("Erreur: Aucun fichier d'instance trouvé dans %s\n", instances_dir);
        return 1;
    }

    printf("Nombre d'instances trouvées: %d\n\n", num_instances);

    for (int i = 0; i < num_instances; i++)
    {
        printf("  %d. %s\n", i + 1, instances[i].basename);
    }

    // Itérer sur chaque instance
    for (int i = 0; i < num_instances; i++)
    {
        printf("\n\n");
        printf("╔════════════════════════════════════════════════════════╗\n");
        printf("║ Instance %d/%d: %s\n", i + 1, num_instances, instances[i].basename);
        printf("╚════════════════════════════════════════════════════════╝\n");

        // Charger l'instance
        Instance* current_instance = load_instance(instances[i].filename);

        if (current_instance == NULL)
        {
            printf("Erreur: Impossible de charger l'instance %s\n", instances[i].filename);
            continue;
        }

        printf("Instance chargée: %d jobs, %d machines\n", 
               current_instance->nombre_jobs, current_instance->nombre_machines);

        // Tester algo_pareto NUM_RUNS fois
        test_algo_pareto_multiple(current_instance, instances[i].basename);

        // Tester algo_scalaire NUM_RUNS fois
        test_algo_scalaire_multiple(current_instance, instances[i].basename);

        // Libérer l'instance
        free_instance(current_instance);

        printf("\n");
    }

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ BENCHMARK MULTIPLE RUNS TERMINÉ\n");
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    printf("Fichiers de sortie générés:\n");
    printf("  - output_pareto_multiple_<instance>_run<N>.gnuplot (pareto run N)\n");
    printf("  - output_scalaire_multiple_<instance>_run<N>.gnuplot (scalaire run N)\n\n");
    printf("Utilisez le script Python pour visualiser les fronts:\n");
    printf("  python3 visualize_multiple_runs.py\n");

    return 0;
}

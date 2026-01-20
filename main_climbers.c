#include "instance.h"
#include "test.h"
#include "solution.h"
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAX_INSTANCES 20
#define MAX_SOLUTIONS 500
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

// Teste climber_first sur une instance
void test_climber_first(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing CLIMBER_FIRST on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* solutions[MAX_SOLUTIONS];
    int dominated[MAX_SOLUTIONS];
    
    int count = 0;
    for (int i = 0; i < 10 && count < MAX_SOLUTIONS; i++)
    {
        Solution* sol = generate_random_solution(instance);
        int initial_cost = cout_solution(instance, sol);
        
        int evals_echange = climber_first(instance, sol, ECHANGE);
        int final_cost_echange = cout_solution(instance, sol);
        
        printf("  Itération %d (ECHANGE): initial_cost=%d, final_cost=%d, evals=%d\n", 
               i+1, initial_cost, final_cost_echange, evals_echange);
        
        solutions[count] = sol;
        dominated[count] = 0;
        count++;
    }
    
    // Exporter les solutions
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_climber_first_%s.gnuplot", instance_basename);
    exporter_solutions_gnuplot_flag(instance, solutions, dominated, count, output_filename);
    
    printf("Résultats exportés dans: %s\n", output_filename);
    
    // Libérer la mémoire
    for (int i = 0; i < count; i++)
    {
        free_solution(solutions[i]);
    }
}

// Teste climber_best sur une instance
void test_climber_best(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing CLIMBER_BEST on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* solutions[MAX_SOLUTIONS];
    int dominated[MAX_SOLUTIONS];
    
    int count = 0;
    for (int i = 0; i < 10 && count < MAX_SOLUTIONS; i++)
    {
        Solution* sol = generate_random_solution(instance);
        int initial_cost = cout_solution(instance, sol);
        
        int evals_echange = climber_best(instance, sol, ECHANGE);
        int final_cost_echange = cout_solution(instance, sol);
        
        printf("  Itération %d (ECHANGE): initial_cost=%d, final_cost=%d, evals=%d\n", 
               i+1, initial_cost, final_cost_echange, evals_echange);
        
        solutions[count] = sol;
        dominated[count] = 0;
        count++;
    }
    
    // Exporter les solutions
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_climber_best_%s.gnuplot", instance_basename);
    exporter_solutions_gnuplot_flag(instance, solutions, dominated, count, output_filename);
    
    printf("Résultats exportés dans: %s\n", output_filename);
    
    // Libérer la mémoire
    for (int i = 0; i < count; i++)
    {
        free_solution(solutions[i]);
    }
}

// Teste iterated_local_search sur une instance
void test_iterated_local_search(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing ITERATED_LOCAL_SEARCH on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* solutions[MAX_SOLUTIONS];
    int dominated[MAX_SOLUTIONS];
    
    int count = 0;
    int iterations_max = 50;
    int force_perturbation = 3;
    
    for (int i = 0; i < 5 && count < MAX_SOLUTIONS; i++)
    {
        Solution* sol = generate_random_solution(instance);
        int initial_cost = cout_solution(instance, sol);
        
        int evals = iterated_local_search(instance, sol, iterations_max, force_perturbation, ECHANGE);
        int final_cost = cout_solution(instance, sol);
        
        printf("  Itération %d: initial_cost=%d, final_cost=%d, evals=%d\n", 
               i+1, initial_cost, final_cost, evals);
        
        solutions[count] = sol;
        dominated[count] = 0;
        count++;
    }
    
    // Exporter les solutions
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_ils_%s.gnuplot", instance_basename);
    exporter_solutions_gnuplot_flag(instance, solutions, dominated, count, output_filename);
    
    printf("Résultats exportés dans: %s\n", output_filename);
    
    // Libérer la mémoire
    for (int i = 0; i < count; i++)
    {
        free_solution(solutions[i]);
    }
}

// Teste marche_aleatoire sur une instance
void test_marche_aleatoire(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing MARCHE_ALEATOIRE on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* solutions[MAX_SOLUTIONS];
    int dominated[MAX_SOLUTIONS];
    
    int count = 0;
    int steps = 100;
    
    for (int i = 0; i < 10 && count < MAX_SOLUTIONS; i++)
    {
        Solution* sol = generate_random_solution(instance);
        int initial_cost = cout_solution(instance, sol);
        
        marche_aleatoire(sol, steps, ECHANGE);
        int final_cost = cout_solution(instance, sol);
        
        printf("  Itération %d: initial_cost=%d, final_cost=%d, steps=%d\n", 
               i+1, initial_cost, final_cost, steps);
        
        solutions[count] = sol;
        dominated[count] = 0;
        count++;
    }
    
    // Exporter les solutions
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_marche_aleatoire_%s.gnuplot", instance_basename);
    exporter_solutions_gnuplot_flag(instance, solutions, dominated, count, output_filename);
    
    printf("Résultats exportés dans: %s\n", output_filename);
    
    // Libérer la mémoire
    for (int i = 0; i < count; i++)
    {
        free_solution(solutions[i]);
    }
}

int main(int argc, char *argv[])
{
    const char* instances_dir = "instances";
    InstanceInfo instances[MAX_INSTANCES];
    int num_instances = 0;
    srand(123456789);

    printf("===== BENCHMARK CLIMBER ALGORITHMS =====\n");
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

        printf("Instance chargée: %d jobs, %d machines\n\n", 
               current_instance->nombre_jobs, current_instance->nombre_machines);

        // Tester les différents climbers
        test_climber_first(current_instance, instances[i].basename);
        test_climber_best(current_instance, instances[i].basename);
        test_iterated_local_search(current_instance, instances[i].basename);
        test_marche_aleatoire(current_instance, instances[i].basename);

        // Libérer l'instance
        free_instance(current_instance);

        printf("\n");
    }

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ BENCHMARK CLIMBERS TERMINÉ\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\nFichiers de sortie générés:\n");
    printf("  - output_climber_first_<instance>.gnuplot (résultats Climber First)\n");
    printf("  - output_climber_best_<instance>.gnuplot (résultats Climber Best)\n");
    printf("  - output_ils_<instance>.gnuplot (résultats Iterated Local Search)\n");
    printf("  - output_marche_aleatoire_<instance>.gnuplot (résultats Marche Aléatoire)\n");

    return 0;
}

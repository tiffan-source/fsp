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
    char basename[MAX_FILENAME]; // nom sans extension (ex: "7_5_01")
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
        // Vérifier si c'est un fichier .txt
        int len = strlen(entry->d_name);
        if (len > 4 && strcmp(entry->d_name + len - 4, ".txt") == 0)
        {
            // Construire le chemin complet
            snprintf(instances[count].filename, MAX_FILENAME, "%s/%s", instances_dir, entry->d_name);
            
            // Extraire le basename (sans chemin ni extension)
            strncpy(instances[count].basename, entry->d_name, len - 4);
            instances[count].basename[len - 4] = '\0';
            
            count++;
        }
    }

    closedir(dir);
    return count;
}

// Teste algo_pareto sur une instance
void test_algo_pareto(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing ALGO_PARETO on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* archives[MAX_SOLUTIONS];
    int num_solutions = 0;

    // Exécuter algo_pareto
    num_solutions = algo_pareto(instance, archives, MAX_SOLUTIONS, ECHANGE);

    // Tous les résultats sont non-dominés
    int dominated[MAX_SOLUTIONS];
    for (int i = 0; i < num_solutions; i++)
    {
        dominated[i] = 0;
    }

    // Générer le nom du fichier de sortie
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_pareto_%s.gnuplot", instance_basename);

    // Exporter les solutions
    exporter_solutions_gnuplot_flag(instance, archives, dominated, num_solutions, output_filename);

    printf("\nArchive finale (ALGO_PARETO) - %d solutions:\n", num_solutions);
    for (int i = 0; i < num_solutions; i++)
    {
        int makespan = cout_solution(instance, archives[i]);
        int tardiness = cout_solution_retard(instance, archives[i]);
        printf("  Solution %d: makespan = %d, tardiness = %d\n", i + 1, makespan, tardiness);
    }

    printf("Résultats exportés dans: %s\n", output_filename);

    // Libérer la mémoire
    for (int i = 0; i < num_solutions; i++)
    {
        free_solution(archives[i]);
    }
}

// Teste algo_scalaire sur une instance
void test_algo_scalaire(Instance* instance, const char* instance_basename)
{
    printf("\n========================================\n");
    printf("Testing ALGO_SCALAIRE on instance: %s\n", instance_basename);
    printf("========================================\n");
    
    Solution* archives[MAX_SOLUTIONS];
    int num_solutions = 0;
    int nb_scalarizations = 20; // Nombre de poids différents à tester

    // Exécuter algo_scalaire
    num_solutions = algo_scalaire(instance, archives, MAX_SOLUTIONS, nb_scalarizations, ECHANGE);

    // Tous les résultats sont non-dominés
    int dominated[MAX_SOLUTIONS];
    for (int i = 0; i < num_solutions; i++)
    {
        dominated[i] = 0;
    }

    // Générer le nom du fichier de sortie
    char output_filename[MAX_FILENAME];
    snprintf(output_filename, MAX_FILENAME, "output_scalaire_%s.gnuplot", instance_basename);

    // Exporter les solutions
    exporter_solutions_gnuplot_flag(instance, archives, dominated, num_solutions, output_filename);

    printf("\nArchive finale (ALGO_SCALAIRE) - %d solutions:\n", num_solutions);
    for (int i = 0; i < num_solutions; i++)
    {
        int makespan = cout_solution(instance, archives[i]);
        int tardiness = cout_solution_retard(instance, archives[i]);
        printf("  Solution %d: makespan = %d, tardiness = %d\n", i + 1, makespan, tardiness);
    }

    printf("Résultats exportés dans: %s\n", output_filename);

    // Libérer la mémoire
    for (int i = 0; i < num_solutions; i++)
    {
        free_solution(archives[i]);
    }
}

int main(int argc, char *argv[])
{
    const char* instances_dir = "instances";
    InstanceInfo instances[MAX_INSTANCES];
    int num_instances = 0;
    srand(123456789);

    printf("===== BENCHMARK MULTI-ALGORITHMES - FRONT PARETO =====\n");
    printf("Répertoire des instances: %s\n\n", instances_dir);

    // Récupérer la liste des instances
    num_instances = get_instance_files(instances_dir, instances, MAX_INSTANCES);

    if (num_instances == 0)
    {
        printf("Erreur: Aucun fichier d'instance trouvé dans %s\n", instances_dir);
        return 1;
    }

    printf("Nombre d'instances trouvées: %d\n\n", num_instances);

    // Trier les instances par ordre de nom (optionnel, pour plus de cohérence)
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

        // Tester algo_pareto
        test_algo_pareto(current_instance, instances[i].basename);

        // Tester algo_scalaire
        test_algo_scalaire(current_instance, instances[i].basename);

        // Libérer l'instance
        free_instance(current_instance);

        printf("\n");
    }

    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║ BENCHMARK TERMINÉ\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("\nFichiers de sortie générés:\n");
    printf("  - output_pareto_<instance>.gnuplot (résultats du Pareto Local Search)\n");
    printf("  - output_scalaire_<instance>.gnuplot (résultats du Scalaire)\n");

    return 0;
}
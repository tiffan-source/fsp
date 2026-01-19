#ifndef SOLUTION_H
#define SOLUTION_H

#include "instance.h"
#include "helper.h"
#include <time.h>

typedef struct Solution
{
    int* solution;
    int taille;
} Solution;

typedef enum Operation {
    ECHANGE,
    INSERTION
}Operation;


Solution* generate_random_solution(Instance* probleme);
void display_solution(Solution* solution);
int cout_solution(Instance* probleme, Solution* solutions);
int cout_solution_retard(Instance* probleme, Solution* solutions);
void echange(Solution* solution, int pos_job_1, int pos_job_2);
void insere(Solution* solution, int posInit, int posEnd);
void marche_aleatoire(Solution* solution, int n, Operation operation_type);
void free_solution(Solution* solution);
int climber_first(Instance* instance, Solution* solution, Operation operation_type);
int climber_best(Instance* instance, Solution* solution, Operation operation_type);
int iterated_local_search(Instance* instance, Solution* solution, int iterations_max, int force_perturbation, Operation operation_type);
int filtrage_offline(Instance *instance, Solution** group_solution, Solution** result_filter, int size_groupe);
int filtrage_online(Instance *instance, Solution** archive, int* archive_size, int max_archive_size, Solution* new_solution);
int cout_solution_scalaire(Instance* instance, Solution* solution, double weight_makespan, double weight_tardiness);
int algo_scalaire(Instance* instance, Solution** archive, int max_archive_size, int nb_scalarizations, Operation operation_type);

#endif // SOLUTION_H
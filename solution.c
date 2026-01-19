#include "solution.h"

/**
 * find best after
 */

Solution* generate_random_solution(Instance* probleme)
{
    int nombre_job = probleme->nombre_jobs;
    Solution* solution = NULL;
    int random_position, copy;

    solution = malloc(sizeof(Solution));
    solution->solution = malloc(sizeof(int) * nombre_job);
    solution->taille = nombre_job;

    for (size_t i = 0; i < nombre_job; i++)
    {
       solution->solution[i] = i;
    }

    for (size_t i = 0; i < nombre_job; i++)
    {
        random_position = random_number(i, nombre_job - 1);
        copy = solution->solution[i];
        solution->solution[i] = solution->solution[random_position];
        solution->solution[random_position] = copy;
    }

    return solution;
}

void free_solution(Solution* solution)
{
    free(solution->solution);
    free(solution);
}

int cout_solution(Instance* probleme, Solution* solutions)
{
    int* previousTime = malloc(sizeof(int) * probleme->nombre_jobs);
    int previousTimeOnRow = 0;

    for (size_t i = 0; i < probleme->nombre_jobs; i++)
    {
        previousTime[i] = 0;
    }

    for (size_t i = 0; i < probleme->nombre_jobs; i++)
    {
        previousTime[i] = probleme->times_jobs_on_machines[0][solutions->solution[i]] + previousTimeOnRow;
        previousTimeOnRow = previousTime[i];
    }

    for (size_t i = 1; i < probleme->nombre_machines; i++)
    {
        for (size_t j = 0; j < probleme->nombre_jobs; j++)
        {
            if (j == 0)
            {
                previousTime[j] = previousTime[j] + probleme->times_jobs_on_machines[i][solutions->solution[j]];
                previousTimeOnRow = previousTime[j];
            }
            else
            {
                if(previousTime[j-1] >= previousTime[j])
                {
                    previousTime[j] = previousTime[j - 1] + probleme->times_jobs_on_machines[i][solutions->solution[j]];
                    previousTimeOnRow = previousTime[j];
                }
                else
                {
                    previousTime[j] = probleme->times_jobs_on_machines[i][solutions->solution[j]] + (previousTime[j]);
                    previousTimeOnRow = previousTime[j];
                }

            }
        }
    }

    int result = previousTime[probleme->nombre_jobs - 1];
    free(previousTime);

    return result;
}

int cout_solution_retard(Instance* probleme, Solution* solutions)
{
    int* previousTime = malloc(sizeof(int) * probleme->nombre_jobs);
    int previousTimeOnRow = 0;

    for (size_t i = 0; i < probleme->nombre_jobs; i++)
    {
        previousTime[i] = 0;
    }

    for (size_t i = 0; i < probleme->nombre_jobs; i++)
    {
        previousTime[i] = probleme->times_jobs_on_machines[0][solutions->solution[i]] + previousTimeOnRow;
        previousTimeOnRow = previousTime[i];
    }

    for (size_t i = 1; i < probleme->nombre_machines; i++)
    {
        for (size_t j = 0; j < probleme->nombre_jobs; j++)
        {
            if (j == 0)
            {
                previousTime[j] = previousTime[j] + probleme->times_jobs_on_machines[i][solutions->solution[j]];
                previousTimeOnRow = previousTime[j];
            }
            else
            {
                if(previousTime[j-1] >= previousTime[j])
                {
                    previousTime[j] = previousTime[j - 1] + probleme->times_jobs_on_machines[i][solutions->solution[j]];
                    previousTimeOnRow = previousTime[j];
                }
                else
                {
                    previousTime[j] = probleme->times_jobs_on_machines[i][solutions->solution[j]] + (previousTime[j]);
                    previousTimeOnRow = previousTime[j];
                }

            }
        }
    }

    int result = 0;

    for (size_t i = 0; i < probleme->nombre_jobs; i++)
    {
        printf("Job %ld : end at %d -- wish %d -- retard %d \n", solutions->solution[i], previousTime[i], probleme->date_end_jobs_wish[solutions->solution[i]], previousTime[i] - probleme->date_end_jobs_wish[solutions->solution[i]]);
        if(previousTime[i] > probleme->date_end_jobs_wish[solutions->solution[i]])
        {
            result += previousTime[i] - probleme->date_end_jobs_wish[solutions->solution[i]];
        }
    }
    
    free(previousTime);
    
    return result;
}

void copy_solution(Solution* source, Solution* destination)
{
    for (size_t i = 0; i < source->taille; i++)
    {
        destination->solution[i] = source->solution[i];
    }
}

void echange(Solution* solution, int pos_job_1, int pos_job_2)
{
    int temp = solution->solution[pos_job_1];
    solution->solution[pos_job_1] = solution->solution[pos_job_2];
    solution->solution[pos_job_2] = temp;
}

void insere(Solution* solution, int posInit, int posEnd)
{
    if (posInit < posEnd)
    {
        int temp = solution->solution[posInit];
        for (size_t i = posInit; i < posEnd; i++)
        {
            solution->solution[i] = solution->solution[i + 1];
        }
        solution->solution[posEnd] = temp;
    }
    else if (posInit > posEnd)
    {
        int temp = solution->solution[posInit];
        for (size_t i = posInit; i > posEnd; i--)
        {
            solution->solution[i] = solution->solution[i - 1];
        }
        solution->solution[posEnd] = temp;
    }
}

void random_operation(Solution* solution, Operation operation_type)
{
    int pos1, pos2;


    if (operation_type == ECHANGE)
    {
        // echange
        pos1 = random_number(0, solution->taille - 1);
        do{
            pos2 = random_number(0, solution->taille - 1);
        } while (pos1 == pos2);

        echange(solution, pos1, pos2);
    }
    else
    {
        // insere
        pos1 = random_number(0, solution->taille - 1);
        do{
            pos2 = random_number(0, solution->taille - 1);
        } while (pos1 == pos2);
        insere(solution, pos1, pos2);
    }
}

/**
 * @brief Operate n random insere or echange on the solution and return the best one (lowest cost)
 * 
 * @param solution 
 * @param n 
 * @return int* best solution found
 */

void marche_aleatoire(Solution* solution, int n, Operation operation_type)
{
    for (int i = 0; i < n; i++)
    {
        random_operation(solution, operation_type);
    }
}

int climber_first(Instance* instance, Solution* solution, Operation operation_type)
{
    int number_of_evaluations = 0;
    int operations_number = operation_type == ECHANGE ? solution->taille * (solution->taille -1) / 2 : solution->taille * (solution->taille -1);
    int operation[operations_number][2];

    int counter_operation_already_done = 0;
    int operation_already_done[operations_number];
    int neighbour_index;
    int i = 0;

    Solution* candidate_solution = malloc(sizeof(Solution));
    candidate_solution->solution = malloc(sizeof(int) * solution->taille);
    candidate_solution->taille = solution->taille;

    int best_score = cout_solution(instance, solution);
    number_of_evaluations++;

    if(operation_type == ECHANGE)
    {
        for (int pos1 = 0; pos1 < solution->taille; pos1++)
        {
            for (int pos2 = pos1 + 1; pos2 < solution->taille; pos2++)
            {
                operation[i][0] = pos1;
                operation[i][1] = pos2;
                i++;
            }
        }
    }else{
        for (int pos1 = 0; pos1 < solution->taille; pos1++)
        {
            for (int pos2 = 0; pos2 < solution->taille; pos2++)
            {
                if (pos1 != pos2)
                {
                    operation[i][0] = pos1;
                    operation[i][1] = pos2;
                    i++;
                }
            }
        }
    }
    
    for (int i = 0; i < operations_number; i++)
    {
        copy_solution(solution, candidate_solution);
        if (operation_type == ECHANGE)
        {
            echange(candidate_solution, operation[i][0], operation[i][1]);
        }
        else
        {
            insere(candidate_solution, operation[i][0], operation[i][1]);
        }
        int candidate_score = cout_solution(instance, candidate_solution);
        number_of_evaluations++;
        if (candidate_score < best_score)
        {
            copy_solution(candidate_solution, solution);
            best_score = candidate_score;
            // reset
            i = -1; // will be incremented to 0 at the end of the loop
        }
    }

    free_solution(candidate_solution);
    return number_of_evaluations;
}

int climber_best(Instance* instance, Solution* solution, Operation operation_type)
{
    int number_of_evaluations = 0;
    int operations_number = operation_type == ECHANGE ? solution->taille * (solution->taille -1) / 2 : solution->taille * (solution->taille -1);
    int operation[operations_number][2];

    int neighbour_index;
    int i = 0;
    int improvement = 1;

    Solution* candidate_solution = malloc(sizeof(Solution));
    candidate_solution->solution = malloc(sizeof(int) * solution->taille);
    candidate_solution->taille = solution->taille;

    int best_score = cout_solution(instance, solution);
    number_of_evaluations++;

    if(operation_type == ECHANGE)
    {
        for (int pos1 = 0; pos1 < solution->taille; pos1++)
        {
            for (int pos2 = pos1 + 1; pos2 < solution->taille; pos2++)
            {
                operation[i][0] = pos1;
                operation[i][1] = pos2;
                i++;
            }
        }
    }else{
        for (int pos1 = 0; pos1 < solution->taille; pos1++)
        {
            for (int pos2 = 0; pos2 < solution->taille; pos2++)
            {
                if (pos1 != pos2)
                {
                    operation[i][0] = pos1;
                    operation[i][1] = pos2;
                    i++;
                }
            }
        }
    }

    while (improvement)
    {
        improvement = 0;
        int best_candidate_index = -1;
        int best_candidate_score = best_score;

        for (int i = 0; i < operations_number; i++)
        {
            copy_solution(solution, candidate_solution);

            if (operation_type == ECHANGE)
            {
                echange(candidate_solution, operation[i][0], operation[i][1]);
            }
            else
            {
                insere(candidate_solution, operation[i][0], operation[i][1]);
            }
            int candidate_score = cout_solution(instance, candidate_solution);
            number_of_evaluations++;
            if (candidate_score < best_candidate_score)
            {
                best_candidate_score = candidate_score;
                best_candidate_index = i;
            }
        }

        if (best_candidate_index != -1)
        {
            // apply the best candidate
            if (operation_type == ECHANGE)
            {
                echange(solution, operation[best_candidate_index][0], operation[best_candidate_index][1]);
            }
            else
            {
                insere(solution, operation[best_candidate_index][0], operation[best_candidate_index][1]);
            }
            best_score = best_candidate_score;
            improvement = 1;
        }
    }
    free_solution(candidate_solution);
    return number_of_evaluations;
}


int iterated_local_search(Instance* instance, Solution* solution, int iterations_max, int force_perturbation, Operation operation_type)
{
    int number_of_evaluations = 0;
    // 1. On commence par une première descente locale
    number_of_evaluations += climber_first(instance, solution, operation_type);
    int best_score = cout_solution(instance, solution);
    number_of_evaluations++;

    // Sauvegarde de la meilleure solution globale
    Solution* best_global = malloc(sizeof(Solution));
    best_global->taille = solution->taille;
    best_global->solution = malloc(sizeof(int) * solution->taille);
    copy_solution(solution, best_global);

    for (int i = 0; i < iterations_max; i++)
    {
        // 2. PERTURBATION : On crée une copie et on la secoue
        Solution* candidate = malloc(sizeof(Solution));
        candidate->taille = solution->taille;
        candidate->solution = malloc(sizeof(int) * solution->taille);
        copy_solution(best_global, candidate);

        // On applique n insertions aléatoires pour sortir de l'optimum local
        for (int p = 0; p < force_perturbation; p++) {
            int p1 = random_number(0, candidate->taille - 1);
            int p2 = random_number(0, candidate->taille - 1);
            insere(candidate, p1, p2);
        }

        // 3. RECHERCHE LOCALE : On relance le climber sur la solution perturbée
        climber_first(instance, candidate, operation_type);

        // 4. CRITÈRE D'ACCEPTATION : Est-ce mieux que notre record ?
        int candidate_score = cout_solution(instance, candidate);
        number_of_evaluations++;
        if (candidate_score < best_score)
        {
            best_score = candidate_score;
            copy_solution(candidate, best_global);
        }

        free_solution(candidate);
    }

    // On remet la meilleure solution trouvée dans l'objet initial
    copy_solution(best_global, solution);
    free_solution(best_global);
    return number_of_evaluations;
}

int filtrage_offline(Instance *instance, Solution** group_solution, Solution** result_filter, int size_groupe)
{
    int index_result = 0;
    int score_bdd[size_groupe][2];
    int is_dominated = 0;

    // Calcul des scores pour toutes les solutions
    for (size_t i = 0; i < size_groupe; i++)
    {
        score_bdd[i][0] = cout_solution(instance, group_solution[i]);
        score_bdd[i][1] = cout_solution_retard(instance, group_solution[i]);
    }

    // Vérifier chaque solution pour voir si elle est dominée
    for (size_t i = 0; i < size_groupe; i++)
    {
        is_dominated = 0;
        for (size_t j = 0; j < size_groupe; j++)
        {
            if (i != j)
            {
                // j domine i si j est meilleur sur les 2 critères (ou égal partout et strictement meilleur quelque part)
                if (score_bdd[j][0] <= score_bdd[i][0] && score_bdd[j][1] <= score_bdd[i][1] &&
                    (score_bdd[j][0] < score_bdd[i][0] || score_bdd[j][1] < score_bdd[i][1]))
                {
                    is_dominated = 1;
                    break;
                }
            }
        }
        // Ajouter la solution au résultat si elle n'est pas dominée
        if (is_dominated == 0)
        {
            result_filter[index_result++] = group_solution[i];
        }
    }

    return index_result;
}

/**
 * @brief Filtre en ligne : maintient une archive de solutions non-dominées
 *        À chaque nouvelle solution, décide si l'ajouter à l'archive et supprime les solutions dominées
 * 
 * @param instance : l'instance du problème
 * @param archive : tableau des solutions non-dominées
 * @param archive_size : pointeur sur la taille actuelle de l'archive
 * @param max_archive_size : taille maximale de l'archive
 * @param new_solution : la nouvelle solution à traiter
 * @return int : 1 si la solution a été ajoutée à l'archive, 0 sinon
 */
int filtrage_online(Instance *instance, Solution** archive, int* archive_size, 
                    int max_archive_size, Solution* new_solution)
{
    int new_score[2];
    int is_dominated = 0;
    int index_to_remove[*archive_size];
    int nb_to_remove = 0;

    // Calcul des scores de la nouvelle solution
    new_score[0] = cout_solution(instance, new_solution);
    new_score[1] = cout_solution_retard(instance, new_solution);

    // Vérifier si la nouvelle solution est dominée par une solution existante
    for (size_t i = 0; i < *archive_size; i++)
    {
        int archive_score[2];
        archive_score[0] = cout_solution(instance, archive[i]);
        archive_score[1] = cout_solution_retard(instance, archive[i]);

        // La solution de l'archive domine la nouvelle solution
        if (archive_score[0] <= new_score[0] && archive_score[1] <= new_score[1] &&
            (archive_score[0] < new_score[0] || archive_score[1] < new_score[1]))
        {
            is_dominated = 1;
            break;
        }

        // La nouvelle solution domine la solution de l'archive
        if (new_score[0] <= archive_score[0] && new_score[1] <= archive_score[1] &&
            (new_score[0] < archive_score[0] || new_score[1] < archive_score[1]))
        {
            index_to_remove[nb_to_remove++] = i;
        }
    }

    // Si la nouvelle solution n'est pas dominée
    if (!is_dominated)
    {
        // Supprimer les solutions dominées (en parcourant de la fin pour éviter les décalages)
        for (int i = nb_to_remove - 1; i >= 0; i--)
        {
            int idx = index_to_remove[i];
            // Décaler les éléments
            for (int j = idx; j < *archive_size - 1; j++)
            {
                archive[j] = archive[j + 1];
            }
            (*archive_size)--;
        }

        // Ajouter la nouvelle solution si l'archive n'est pas pleine
        if (*archive_size < max_archive_size)
        {
            archive[*archive_size] = new_solution;
            (*archive_size)++;
            return 1;  // Solution ajoutée
        }
        else
        {
            // L'archive est pleine : on pourrait implémenter une stratégie de remplacement
            // Pour l'instant, on rejette la solution
            return 0;
        }
    }

    return 0;  // Solution dominée, pas ajoutée
}
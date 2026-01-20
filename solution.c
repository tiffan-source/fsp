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
        // printf("Job %ld : end at %d -- wish %d -- retard %d \n", solutions->solution[i], previousTime[i], probleme->date_end_jobs_wish[solutions->solution[i]], previousTime[i] - probleme->date_end_jobs_wish[solutions->solution[i]]);
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
                    int max_archive_size, Solution** new_solutions, int new_solutions_size)
{
    int solutions_added = 0;

    // Traiter chaque nouvelle solution
    for (int sol_idx = 0; sol_idx < new_solutions_size; sol_idx++)
    {
        Solution* new_solution = new_solutions[sol_idx];
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
                solutions_added++;
            }
            else
            {
                // L'archive est pleine : on pourrait implémenter une stratégie de remplacement
                // Pour l'instant, on rejette la solution
            }
        }
    }

    return solutions_added;  // Nombre de solutions ajoutées
}

/**
 * @brief Fonction de coût scalaire combinant makespan et tardiness avec des poids
 * 
 * @param instance : l'instance du problème
 * @param solution : la solution à évaluer
 * @param weight_makespan : poids pour le makespan (0 à 1)
 * @param weight_tardiness : poids pour le tardiness (0 à 1)
 * @return int : coût scalaire
 */
int cout_solution_scalaire(Instance* instance, Solution* solution, double weight_makespan, double weight_tardiness)
{
    int makespan = cout_solution(instance, solution);
    int tardiness = cout_solution_retard(instance, solution);

    // Normalisation simple : diviser par les valeurs maximales pour mettre à l'échelle
    // On utilise une somme pondérée normalisée
    return (int)(weight_makespan * makespan + weight_tardiness * tardiness);
}

/**
 * @brief Algorithme scalaire pour explorer le front Pareto
 *        Utilise une somme pondérée pour transformer le problème multi-objectifs en mono-objectif
 * 
 * @param instance : l'instance du problème
 * @param archive : archive de solutions non-dominées
 * @param max_archive_size : taille maximale de l'archive
 * @param nb_scalarizations : nombre de scalarisations (nombre de vecteurs de poids à tester)
 * @param operation_type : type d'opérateur à utiliser (ECHANGE ou INSERTION)
 * @return int : nombre de solutions dans l'archive finale
 */
int algo_scalaire(Instance* instance, Solution** archive, int max_archive_size, 
                  int nb_scalarizations, Operation operation_type)
{
    int archive_size = 0;

    printf("===== ALGORITHME SCALAIRE =====\n");
    printf("Nombre de scalarisations : %d\n\n", nb_scalarizations);

    // Itérer sur différents vecteurs de poids
    for (int k = 0; k <= nb_scalarizations; k++)
    {
        // Définir les poids : w_makespan varie de 0 à 1
        double weight_makespan = (double)k / nb_scalarizations;
        double weight_tardiness = 1.0 - weight_makespan;

        printf("Itération %d : w_makespan = %.2f, w_tardiness = %.2f\n", k, weight_makespan, weight_tardiness);

        // Générer une solution initiale aléatoire
        Solution* current_solution = generate_random_solution(instance);

        int initial_cost = cout_solution_scalaire(instance, current_solution, weight_makespan, weight_tardiness);
        printf("  Coût initial : %d\n", initial_cost);

        // Appliquer le climber_best sur la fonction scalaire
        // On modifie temporairement le comportement en gardant la même solons dans l'archive fution
        Solution* best_solution = malloc(sizeof(Solution));
        best_solution->solution = malloc(sizeof(int) * instance->nombre_jobs);
        best_solution->taille = instance->nombre_jobs;
        copy_solution(current_solution, best_solution);

        int best_cost = initial_cost;
        int improvement = 1;
        int iterations = 0;

        // Recherche locale avec climber_best adapté pour la fonction scalaire
        while (improvement)
        {
            improvement = 0;
            int best_candidate_cost = best_cost;
            int best_candidate_index = -1;

            // Générer tous les voisins possibles
            int operations_number = operation_type == ECHANGE ? 
                                    best_solution->taille * (best_solution->taille - 1) / 2 : 
                                    best_solution->taille * (best_solution->taille - 1);

            Solution* candidate = malloc(sizeof(Solution));
            candidate->solution = malloc(sizeof(int) * best_solution->taille);
            candidate->taille = best_solution->taille;

            for (int op_idx = 0; op_idx < operations_number; op_idx++)
            {
                copy_solution(best_solution, candidate);

                // Générer le voisin
                if (operation_type == ECHANGE)
                {
                    int pos1 = op_idx / best_solution->taille;
                    int pos2 = op_idx % best_solution->taille;
                    if (pos2 > pos1)
                    {
                        echange(candidate, pos1, pos2);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    int pos1 = op_idx / best_solution->taille;
                    int pos2 = op_idx % best_solution->taille;
                    if (pos1 != pos2)
                    {
                        insere(candidate, pos1, pos2);
                    }
                    else
                    {
                        continue;
                    }
                }

                // Évaluer le voisin avec la fonction scalaire
                int candidate_cost = cout_solution_scalaire(instance, candidate, weight_makespan, weight_tardiness);

                if (candidate_cost < best_candidate_cost)
                {
                    best_candidate_cost = candidate_cost;
                    best_candidate_index = op_idx;
                    improvement = 1;
                }
            }

            if (improvement)
            {
                // Appliquer la meilleure amélioration trouvée
                if (operation_type == ECHANGE)
                {
                    int pos1 = best_candidate_index / best_solution->taille;
                    int pos2 = best_candidate_index % best_solution->taille;
                    if (pos2 > pos1)
                    {
                        echange(best_solution, pos1, pos2);
                    }
                }
                else
                {
                    int pos1 = best_candidate_index / best_solution->taille;
                    int pos2 = best_candidate_index % best_solution->taille;
                    if (pos1 != pos2)
                    {
                        insere(best_solution, pos1, pos2);
                    }
                }
                best_cost = best_candidate_cost;
                iterations++;
            }

            free_solution(candidate);
        }

        int final_makespan = cout_solution(instance, best_solution);
        int final_tardiness = cout_solution_retard(instance, best_solution);
        printf("  Coût final : %d (makespan : %d, tardiness : %d)\n", best_cost, final_makespan, final_tardiness);
        printf("  Itérations de recherche locale : %d\n", iterations);

        // Ajouter la solution à l'archive via le filtrage en ligne
        filtrage_online(instance, archive, &archive_size, max_archive_size, &best_solution, 1);

        free_solution(current_solution);
    }

    printf("\n===== RÉSULTATS FINAUX =====\n");
    printf("Nombre de solutions non-dominées : %d\n\n", archive_size);

    // Afficher l'archive finale
    for (int i = 0; i < archive_size; i++)
    {
        int makespan = cout_solution(instance, archive[i]);
        int tardiness = cout_solution_retard(instance, archive[i]);
        printf("Solution %d : makespan = %d, tardiness = %d\n", i + 1, makespan, tardiness);
    }

    return archive_size;
}

void display_solution(Solution* solution)
{
    for(int i = 0; i < solution->taille; i++)
    {
        printf("%d ", solution->solution[i]);
    }
    printf("\n");
}

int exporter_solutions_gnuplot_flag(Instance* instance, Solution **solutions, int *dominated, int n, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    for (int i = 0; i < n; ++i) {
        fprintf(f, "%d %d %d\n",
                cout_solution(instance, solutions[i]),
                cout_solution_retard(instance, solutions[i]),
                dominated[i] ? 1 : 0);
    }
    fclose(f);
    return 1;
}


/**
 * @brief Algorithme Pareto Local Search (PLS)
 *        Explore itérativement les voisinages des solutions non-dominées
 * 
 * @param instance L'instance du problème
 * @param archive L'archive qui contiendra le front Pareto final
 * @param max_archive_size Taille maximale de l'archive
 * @param operation_type Opérateur de voisinage (ECHANGE ou INSERTION)
 * @return int La taille finale de l'archive
 */
int algo_pareto(Instance* instance, Solution** archive, int max_archive_size, Operation operation_type)
{
    printf("===== ALGORITHME PARETO LOCAL SEARCH =====\n");
    
    int archive_size = 0;
    int iteration = 0;
    int improvement = 1;
    int max_iterations = 100; // Limite d'itérations pour éviter une boucle infinie
    
    // Étape 1: Générer une solution initiale aléatoire
    Solution* initial_solution = generate_random_solution(instance);
    int initial_makespan = cout_solution(instance, initial_solution);
    int initial_tardiness = cout_solution_retard(instance, initial_solution);
    printf("Solution initiale : makespan = %d, tardiness = %d\n\n", initial_makespan, initial_tardiness);
    
    // Ajouter la solution initiale à l'archive
    archive[archive_size++] = initial_solution;
    
    // Étape 2: Itérer jusqu'à convergence
    while (improvement && iteration < max_iterations)
    {
        improvement = 0;
        iteration++;
        printf("Itération %d : Archive size = %d\n", iteration, archive_size);
        
        // Créer un tableau temporaire pour les nouveaux voisins
        Solution** new_neighbors = malloc(sizeof(Solution*) * max_archive_size * 100);
        int neighbors_count = 0;
        
        // Étape 3: Pour chaque solution dans l'archive, générer les voisins
        for (int sol_idx = 0; sol_idx < archive_size && neighbors_count < max_archive_size * 100; sol_idx++)
        {
            Solution* current_solution = archive[sol_idx];
            int operations_number = operation_type == ECHANGE ? 
                                    current_solution->taille * (current_solution->taille - 1) / 2 : 
                                    current_solution->taille * (current_solution->taille - 1);
            
            // Générer tous les voisins de la solution courante
            if (operation_type == ECHANGE)
            {
                for (int pos1 = 0; pos1 < current_solution->taille && neighbors_count < max_archive_size * 100; pos1++)
                {
                    for (int pos2 = pos1 + 1; pos2 < current_solution->taille && neighbors_count < max_archive_size * 100; pos2++)
                    {
                        // Créer un voisin par échange
                        Solution* neighbor = malloc(sizeof(Solution));
                        neighbor->solution = malloc(sizeof(int) * current_solution->taille);
                        neighbor->taille = current_solution->taille;
                        copy_solution(current_solution, neighbor);
                        
                        echange(neighbor, pos1, pos2);
                        new_neighbors[neighbors_count++] = neighbor;
                    }
                }
            }
            else
            {
                // INSERTION
                for (int pos1 = 0; pos1 < current_solution->taille && neighbors_count < max_archive_size * 100; pos1++)
                {
                    for (int pos2 = 0; pos2 < current_solution->taille && neighbors_count < max_archive_size * 100; pos2++)
                    {
                        if (pos1 != pos2)
                        {
                            // Créer un voisin par insertion
                            Solution* neighbor = malloc(sizeof(Solution));
                            neighbor->solution = malloc(sizeof(int) * current_solution->taille);
                            neighbor->taille = current_solution->taille;
                            copy_solution(current_solution, neighbor);
                            
                            insere(neighbor, pos1, pos2);
                            new_neighbors[neighbors_count++] = neighbor;
                        }
                    }
                }
            }
        }
        
        printf("  Voisins générés : %d\n", neighbors_count);
        
        // Étape 4: Filtrer l'ensemble complet (archive + nouveaux voisins)
        Solution** combined = malloc(sizeof(Solution*) * (archive_size + neighbors_count));
        for (int i = 0; i < archive_size; i++)
        {
            combined[i] = archive[i];
        }
        for (int i = 0; i < neighbors_count; i++)
        {
            combined[archive_size + i] = new_neighbors[i];
        }
        
        Solution** filtered = malloc(sizeof(Solution*) * (archive_size + neighbors_count));
        int filtered_size = filtrage_offline(instance, combined, filtered, archive_size + neighbors_count);
        
        printf("  Solutions après filtrage : %d\n", filtered_size);
        
        // Vérifier s'il y a une amélioration
        if (filtered_size > archive_size)
        {
            improvement = 1;
            archive_size = filtered_size > max_archive_size ? max_archive_size : filtered_size;
            
            // Copier les solutions filtrées dans l'archive
            for (int i = 0; i < archive_size; i++)
            {
                archive[i] = filtered[i];
            }
        }
        
        // Libérer la mémoire temporaire
        for (int i = 0; i < neighbors_count; i++)
        {
            // Ne pas libérer si la solution est dans l'archive
            int is_in_archive = 0;
            for (int j = 0; j < archive_size; j++)
            {
                if (archive[j] == new_neighbors[i])
                {
                    is_in_archive = 1;
                    break;
                }
            }
            if (!is_in_archive)
            {
                free_solution(new_neighbors[i]);
            }
        }
        
        free(new_neighbors);
        free(combined);
        free(filtered);
    }
    
    printf("\n===== RÉSULTATS FINAUX =====\n");
    printf("Nombre d'itérations : %d\n", iteration);
    printf("Nombre de solutions non-dominées : %d\n\n", archive_size);
    
    // Afficher l'archive finale
    for (int i = 0; i < archive_size; i++)
    {
        int makespan = cout_solution(instance, archive[i]);
        int tardiness = cout_solution_retard(instance, archive[i]);
        printf("Solution %d : makespan = %d, tardiness = %d\n", i + 1, makespan, tardiness);
    }
    
    return archive_size;
}
#include "test.h"

void test_all_algo(Instance* instance, int iteration_number)
{
    int all_cost = 0;
    int eval = 0;

    printf("Testing marche_aleatoire for insertion...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        marche_aleatoire(solution, 10, INSERTION);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting marche_aleatoire for echange...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        marche_aleatoire(solution, 10, ECHANGE);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("Testing climber_first for insertion...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = climber_first(instance, solution, INSERTION);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting climber_first for echange...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = climber_first(instance, solution, ECHANGE);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting climber_best for insertion...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = climber_best(instance, solution, INSERTION);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting climber_best for echange...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = climber_best(instance, solution, ECHANGE);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting iterated_local_search for insertion...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = iterated_local_search(instance, solution, 100, 5, INSERTION);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);

    all_cost = 0;

    printf("\nTesting iterated_local_search for echange...\n");
    for (int i = 0; i < iteration_number; i++)
    {
        Solution* solution = generate_random_solution(instance);
        int evals = iterated_local_search(instance, solution, 100, 5, ECHANGE);
        int cost = cout_solution(instance, solution);
        all_cost += cost;
        free_solution(solution);
    }
    printf("Average cost after %d iterations: %d\n\n", iteration_number, all_cost / iteration_number);
}
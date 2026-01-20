#include "instance.h"
#include "test.h"
#include "solution.h"


int main(int argc, char *argv[])
{
    Instance* test = load_instance(argv[1]);

    Solution* archives[500];
    int archive_size = 0;
    Solution * solutions[500];

    for (size_t i = 0; i < 500; i++)
    {
        solutions[i] = generate_random_solution(test);
    }

    // int num_filtered = algo_scalaire(test, archives, 500, 100, ECHANGE);
    int num_filtered = algo_pareto(test, archives, 500, ECHANGE);

    int dominated[500];
    // all archive are non dominated
    for (int i = 0; i < num_filtered; i++)
    {
        dominated[i] = 0;
    }

    exporter_solutions_gnuplot_flag(test, archives, dominated, num_filtered, "output4.gnuplot");

    // loop over archives elements and display them
    printf("Final Archive Solutions:\n");
    for (int i = 0; i < num_filtered; i++)
    {
        int makespan = cout_solution(test, archives[i]);
        int tardiness = cout_solution_retard(test, archives[i]);
        printf("Solution %d: Makespan = %d, Tardiness = %d\n", i, makespan, tardiness);
    }

    // Free all solutions (archives contain pointers to some of them)
    for (size_t i = 0; i < 500; i++)
    {
        free_solution(solutions[i]);
    }
    
    return 0;
}
#include "instance.h"
#include "test.h"
#include "solution.h"


int main(int argc, char *argv[])
{
    Instance* test = load_instance(argv[1]);

    Solution * solutions[500];
    Solution * filtered_solutions[500];

    for (size_t i = 0; i < 500; i++)
    {
        solutions[i] = generate_random_solution(test);
    }

    int num_filtered = filtrage_offline(test, solutions, filtered_solutions, 500);

    int dominated[500];
    // all filtered_solutions are non dominated
    for (size_t i = 0; i < num_filtered; i++)
    {
        dominated[i] = 0;
    }

    exporter_solutions_gnuplot_flag(test, filtered_solutions, dominated, num_filtered, "output.gnuplot");

    // Only free solutions that are NOT in filtered_solutions
    int is_filtered[500];
    for (size_t i = 0; i < 500; i++)
    {
        is_filtered[i] = 0;
    }
    
    for (size_t i = 0; i < num_filtered; i++)
    {
        for (size_t j = 0; j < 500; j++)
        {
            if (solutions[j] == filtered_solutions[i])
            {
                is_filtered[j] = 1;
                break;
            }
        }
    }

    for (size_t i = 0; i < 500; i++)
    {
        if (!is_filtered[i])
        {
            free_solution(solutions[i]);
        }
    }
    
    // Free filtered solutions
    for (size_t i = 0; i < num_filtered; i++)
    {
        free_solution(filtered_solutions[i]);
    }
    
    return 0;
}
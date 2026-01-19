#include "instance.h"
#include "test.h"
#include "solution.h"


int main(int argc, char *argv[])
{
    Instance* instance = load_instance(argv[1]);

    Solution* solution_random = generate_random_solution(instance);

    display_instance(instance);

    printf("Random Solution:\n");
    display_solution(solution_random);

    printf("Cost: %d\n", cout_solution(instance, solution_random));

    free_solution(solution_random);
    free_instance(instance);
    return 0;
}
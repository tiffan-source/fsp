#include "instance.h"
#include "test.h"
#include "solution.h"


int main(int argc, char *argv[])
{
    Instance* test = load_instance(argv[1]);

    test_all_algo(test, 100);

    free_instance(test);
    return 0;
}
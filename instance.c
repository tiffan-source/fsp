#include "instance.h"

Instance* load_instance(const char* filename)
{
    FILE* my_instance_file = NULL;
    int jobs_size, machine_size;
    Instance* new_probleme = NULL;
    int i, seed, fake_reader, j;

    my_instance_file = fopen(filename, "r");

    if (my_instance_file == NULL)
    {
        return NULL;
    }
    
    fscanf(my_instance_file, "%d", &jobs_size);
    fscanf(my_instance_file, "%d", &machine_size);
    fscanf(my_instance_file, "%d", &seed);


    printf("Allocation de l'instance\n");

    new_probleme = (Instance*) malloc(sizeof(Instance));

    if (new_probleme == NULL)
    {
        printf("L'instance ne s'est pas chargé\n");
        return NULL;
    }

    new_probleme->nombre_machines = machine_size;
    new_probleme->nombre_jobs = jobs_size;
    new_probleme->seed = seed;
    new_probleme->times_jobs_on_machines = malloc(sizeof(int*) * machine_size);
    new_probleme->date_end_jobs_wish = malloc(sizeof(int) * jobs_size);

    for (i = 0; i < machine_size; i++)
    {
        new_probleme->times_jobs_on_machines[i] = malloc(sizeof(int) * jobs_size);
    }

    for (i = 0; i < jobs_size; i++)
    {
        fscanf(my_instance_file, "%d", &fake_reader);
        fscanf(my_instance_file, "%d", new_probleme->date_end_jobs_wish + i);

        for (j = 0; j < machine_size; j++)
        {
            fscanf(my_instance_file, "%d", new_probleme->times_jobs_on_machines[j] + i);
            // printf("Time %d value %d \n", j, new_probleme->times_jobs_on_machines[j][i]);
        }
    }

    fclose(my_instance_file);
    
    return new_probleme;
}

void free_instance(Instance* instance)
{
    for (size_t i = 0; i < instance->nombre_machines; i++)
    {
        free(instance->times_jobs_on_machines[i]);
    }
    free(instance->times_jobs_on_machines);
    free(instance->date_end_jobs_wish);
    free(instance);
}


void display_instance(Instance* instance_to_display)
{
    for (size_t i = 0; i < instance_to_display->nombre_machines; i++)
    {
        printf("Machine %ld : \n", i + 1);
        for (size_t j = 0; j < instance_to_display->nombre_jobs; j++)
        {
            printf("Job %ld : %d --", j + 1, instance_to_display->times_jobs_on_machines[i][j]);
        }
        printf("\n");
    }
}

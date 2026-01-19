#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Instance
{
    int nombre_machines;
    int nombre_jobs;
    int** times_jobs_on_machines; // time job on machine
    int seed;
    int* date_end_jobs_wish;
} Instance;


Instance* load_instance(const char* filename);
void free_instance(Instance* instance);
void display_instance(Instance* instance_to_display);

#endif // INSTANCE_H
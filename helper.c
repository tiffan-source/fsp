#include "helper.h"

int random_number(int inf, int sup)
{
    return  inf + rand() % (sup - inf + 1);  
}
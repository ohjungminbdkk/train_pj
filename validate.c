#include <string.h>
#include "validate.h"

int exists_eki(struct eki eki_list[], int eki_count, const char *ekiname)
{
    int i;

    for (i = 0; i < eki_count; i++)
    {
        if (strcmp(eki_list[i].name, ekiname) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int exists_train(struct train train_list[], int train_count, int retsuban)
{
    int i;

    for (i = 0; i < train_count; i++)
    {
        if (train_list[i].retsuban == retsuban)
        {
            return 1;
        }
    }

    return 0;
}
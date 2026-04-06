#ifndef VALIDATE_H
#define VALIDATE_H

#include "database.h"

int exists_eki(struct eki eki_list[], int eki_count, const char *ekiname);
int exists_train(struct train train_list[], int train_count, int retsuban);

#endif
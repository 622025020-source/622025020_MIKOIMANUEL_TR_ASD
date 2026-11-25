#ifndef SEARCHING_H
#define SEARCHING_H

#include "route.h"

int sequential_search_route(Route **items, int count, const char *asal,
                            const char *tujuan, int *result, int maxResult);
int binary_search_id(Route **items, int count, int id);

#endif

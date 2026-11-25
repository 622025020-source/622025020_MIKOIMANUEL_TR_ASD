#include "searching.h"
#include "util.h"

int sequential_search_route(Route **items, int count, const char *asal,
                            const char *tujuan, int *result, int maxResult)
{
    int i;
    int found = 0;

    for (i = 0; i < count && found < maxResult; i++) {
        if (str_contains_ci(items[i]->asal, asal) &&
            str_contains_ci(items[i]->tujuan, tujuan)) {
            result[found++] = i;
        }
    }
    return found;
}

int binary_search_id(Route **items, int count, int id)
{
    int low = 0;
    int high = count - 1;
    int mid;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (items[mid]->id == id) {
            return mid;
        }
        if (items[mid]->id < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

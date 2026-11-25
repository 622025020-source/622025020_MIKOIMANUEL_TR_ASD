#include <string.h>
#include "sorting.h"

static int compare_routes(const Route *a, const Route *b, SortField field)
{
    switch (field) {
    case SORT_BY_ID:    return a->id - b->id;
    case SORT_BY_HARGA: return a->harga - b->harga;
    case SORT_BY_JARAK: return a->jarak - b->jarak;
    case SORT_BY_JAM:   return a->jam - b->jam;
    case SORT_BY_KURSI: return a->kursi - b->kursi;
    case SORT_BY_ASAL:  return strcmp(a->asal, b->asal);
    }
    return 0;
}

static int is_out_of_order(const Route *a, const Route *b,
                           SortField field, SortOrder order)
{
    int diff = compare_routes(a, b, field);

    return (order == ORDER_ASC) ? diff > 0 : diff < 0;
}

static void swap_pointer(Route **a, Route **b)
{
    Route *temp = *a;

    *a = *b;
    *b = temp;
}

void bubble_sort(Route **items, int count, SortField field, SortOrder order)
{
    int i, j, swapped;

    for (i = 0; i < count - 1; i++) {
        swapped = 0;
        for (j = 0; j < count - 1 - i; j++) {
            if (is_out_of_order(items[j], items[j + 1], field, order)) {
                swap_pointer(&items[j], &items[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void selection_sort(Route **items, int count, SortField field, SortOrder order)
{
    int i, j, best;

    for (i = 0; i < count - 1; i++) {
        best = i;
        for (j = i + 1; j < count; j++) {
            if (is_out_of_order(items[best], items[j], field, order)) {
                best = j;
            }
        }
        if (best != i) {
            swap_pointer(&items[i], &items[best]);
        }
    }
}

void insertion_sort(Route **items, int count, SortField field, SortOrder order)
{
    int i, j;
    Route *key;

    for (i = 1; i < count; i++) {
        key = items[i];
        j = i - 1;
        while (j >= 0 && is_out_of_order(items[j], key, field, order)) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = key;
    }
}

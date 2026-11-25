#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "config.h"
#include "util.h"

void trim(char *s)
{
    size_t len = strlen(s);
    size_t start = 0;

    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
    while (s[start] != '\0' && isspace((unsigned char)s[start])) {
        start++;
    }
    if (start > 0) {
        memmove(s, s + start, len - start + 1);
    }
}

/* Membaca satu baris dengan aman (tidak overflow, sisa input dibuang). */
void read_line(const char *prompt, char *buf, size_t size)
{
    size_t len;
    int c;

    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0); /* memori dibebaskan lewat atexit() di main.c */
    }
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* buang sisa baris yang terlalu panjang */
        }
    }
    trim(buf);
}

/* Membaca bilangan bulat dalam rentang [min, max]; mengulang sampai valid. */
int read_int(const char *prompt, int min, int max)
{
    char buf[MAX_INPUT];
    char *end;
    long value;

    while (1) {
        read_line(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') {
            printf("Input tidak boleh kosong.\n");
            continue;
        }
        errno = 0;
        value = strtol(buf, &end, 10);
        if (*end != '\0' || errno != 0 || value < min || value > max) {
            printf("Input tidak valid. Masukkan angka %d - %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

/* Membaca teks tidak kosong tanpa karakter '|' (pemisah berkas). */
void read_text(const char *prompt, char *buf, size_t size)
{
    while (1) {
        read_line(prompt, buf, size);
        if (buf[0] == '\0') {
            printf("Input tidak boleh kosong.\n");
        } else if (strchr(buf, '|') != NULL) {
            printf("Karakter '|' tidak diperbolehkan.\n");
        } else {
            return;
        }
    }
}

int str_equal_ci(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

int str_contains_ci(const char *haystack, const char *needle)
{
    size_t i, j;
    size_t hlen = strlen(haystack);
    size_t nlen = strlen(needle);

    if (nlen == 0) {
        return 1;
    }
    for (i = 0; i + nlen <= hlen; i++) {
        for (j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i + j]) !=
                tolower((unsigned char)needle[j])) {
                break;
            }
        }
        if (j == nlen) {
            return 1;
        }
    }
    return 0;
}

void print_line(void)
{
    printf("------------------------------------------------------------------------\n");
}

void print_title(const char *title)
{
    printf("\n========================================================================\n");
    printf("  %s\n", title);
    printf("========================================================================\n");
}

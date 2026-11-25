#ifndef CONFIG_H
#define CONFIG_H

/* Batas input dan data */
#define MAX_INPUT            100
#define MAX_LINE             256
#define MAX_USERNAME         20
#define MIN_USERNAME         3
#define MAX_PASSWORD         20
#define MIN_PASSWORD         4
#define MAX_CITY_NAME        30
#define MAX_TYPE_NAME        15
#define MAX_ROUTE            100
#define MAX_CITY             30
#define MAX_SEATS            100
#define MAX_TICKET_PER_ORDER 5
#define MAX_PRICE            10000000
#define MAX_DISTANCE         5000
#define MAX_ID               9999
#define INITIAL_CAPACITY     8

/* Hash table */
#define HASH_SIZE            31
#define HASH_SEED            5381UL
#define HASH_MASK            0xFFFFFFFFUL

/* Akun bawaan (dibuat otomatis bila belum ada akun sama sekali) */
#define DEFAULT_ADMIN_USER   "admin"
#define DEFAULT_ADMIN_PASS   "admin123"

/* Peran akun */
#define ROLE_USER            0
#define ROLE_ADMIN           1

/* Berkas database */
#define FILE_USERS           "data/users.txt"
#define FILE_ROUTES          "data/routes.txt"
#define FILE_BOOKINGS        "data/bookings.txt"
#define FILE_WAITING         "data/waiting.txt"

#endif

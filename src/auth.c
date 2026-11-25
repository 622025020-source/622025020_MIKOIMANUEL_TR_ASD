#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"
#include "util.h"

static int is_valid_username(const char *username)
{
    size_t i;
    size_t len = strlen(username);

    if (len < MIN_USERNAME || len > MAX_USERNAME) {
        return 0;
    }
    for (i = 0; i < len; i++) {
        if (!isalnum((unsigned char)username[i]) && username[i] != '_') {
            return 0;
        }
    }
    return 1;
}

static int is_valid_password(const char *password)
{
    size_t len = strlen(password);

    if (len < MIN_PASSWORD || len > MAX_PASSWORD) {
        return 0;
    }
    return strchr(password, '|') == NULL && strchr(password, ' ') == NULL;
}

static void create_default_admin(UserTable *table)
{
    User admin;

    strcpy(admin.username, DEFAULT_ADMIN_USER);
    admin.passHash = hash_string(DEFAULT_ADMIN_PASS);
    admin.role = ROLE_ADMIN;
    table_insert(table, &admin);
}

int auth_save(const UserTable *table)
{
    int i;
    UserNode *node;
    FILE *file = fopen(FILE_USERS, "w");

    if (file == NULL) {
        printf("Gagal menyimpan %s (pastikan folder data/ ada).\n", FILE_USERS);
        return 0;
    }
    for (i = 0; i < HASH_SIZE; i++) {
        for (node = table->bucket[i]; node != NULL; node = node->next) {
            fprintf(file, "%s|%lu|%d\n", node->data.username,
                    node->data.passHash, node->data.role);
        }
    }
    fclose(file);
    return 1;
}

int auth_load(UserTable *table)
{
    char line[MAX_LINE];
    User user;
    FILE *file = fopen(FILE_USERS, "r");

    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            if (sscanf(line, "%20[^|]|%lu|%d", user.username,
                       &user.passHash, &user.role) == 3) {
                table_insert(table, &user);
            }
        }
        fclose(file);
    }
    if (table->count == 0) {
        create_default_admin(table);
        auth_save(table);
    }
    return 1;
}

void auth_register(UserTable *table)
{
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    char confirm[MAX_INPUT];
    User user;

    print_title("REGISTER AKUN BARU");
    read_line("Username (3-20 huruf/angka/_): ", username, sizeof(username));
    if (username[0] == '\0') {
        printf("Username tidak boleh kosong.\n");
        return;
    }
    if (!is_valid_username(username)) {
        printf("Username tidak valid (3-20 karakter, hanya huruf, angka, dan _).\n");
        return;
    }
    if (table_search(table, username) != NULL) {
        printf("Username sudah digunakan.\n");
        return;
    }
    read_line("Password (4-20 karakter, tanpa spasi): ", password, sizeof(password));
    if (password[0] == '\0') {
        printf("Password tidak boleh kosong.\n");
        return;
    }
    if (!is_valid_password(password)) {
        printf("Password tidak valid (4-20 karakter, tanpa spasi atau '|').\n");
        return;
    }
    read_line("Ulangi password: ", confirm, sizeof(confirm));
    if (strcmp(password, confirm) != 0) {
        printf("Konfirmasi password tidak cocok.\n");
        return;
    }
    strcpy(user.username, username);
    user.passHash = hash_string(password);
    user.role = ROLE_USER;
    if (table_insert(table, &user) && auth_save(table)) {
        printf("Registrasi berhasil. Silakan login.\n");
    } else {
        printf("Registrasi gagal.\n");
    }
}

User *auth_login(UserTable *table)
{
    char username[MAX_INPUT];
    char password[MAX_INPUT];
    User *user;

    print_title("LOGIN");
    read_line("Username: ", username, sizeof(username));
    read_line("Password: ", password, sizeof(password));
    if (username[0] == '\0' || password[0] == '\0') {
        printf("Login gagal: username dan password tidak boleh kosong.\n");
        return NULL;
    }
    user = table_search(table, username);
    if (user == NULL || user->passHash != hash_string(password)) {
        printf("Login gagal: username atau password salah.\n");
        return NULL;
    }
    printf("Login berhasil. Selamat datang, %s!\n", user->username);
    return user;
}

#ifndef AUTH_H
#define AUTH_H

#include "hashtable.h"

int   auth_load(UserTable *table);
int   auth_save(const UserTable *table);
void  auth_register(UserTable *table);
User *auth_login(UserTable *table);

#endif

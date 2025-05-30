#ifndef VAULT_H
#define VAULT_H

#include <stdbool.h>

#include "../include/hash.h"
#include <sys/_types/_u_int32_t.h>
#include <sys/_types/_u_int64_t.h>

#define MAX_FIELD_SIZE  256

typedef struct {
    u_int32_t hash;
    u_int64_t offset;
} vault_index_entry;

typedef struct {
    char key[MAX_FIELD_SIZE];
    char identifier[MAX_FIELD_SIZE];
    char password[MAX_FIELD_SIZE];
} vault_entry;

typedef enum {
    OK = 1,
    ERROR = 2,
} file_operation_result_t;

file_operation_result_t put_to_vault(const vault_entry *entry);
file_operation_result_t get_from_vault(const u_int32_t hash_key, vault_entry *entry);
file_operation_result_t get_all_from_vault(vault_entry **array_out, size_t *count_out);

vault_entry *vault_entry_create();
void vault_entry_destroy(vault_entry *entry);
bool vault_entry_exist(const char *key);




#endif // !VAULT_H

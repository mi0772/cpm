#include <stdio.h>
#include "../include/vault.h"

#include <stdlib.h>

static const char *VAULT_FILENAME = "vault.dat";
static const char *VAULT_IDX = "vault.idx";
static const char *VAULT_IDX_TEMP = "vault.idx_temp";

file_operation_result_t get_all_from_vault(vault_entry **array_out, size_t *count_out) {
    FILE *f_vault = fopen(VAULT_FILENAME, "r");
    if (!f_vault) {
        return ERROR;
    }

    size_t capacity = 10;
    size_t count = 0;

    vault_entry *array = malloc(sizeof(vault_entry) * capacity);
    if (!array) {
        fclose(f_vault);
        return ERROR;
    }
    while (fread(&array[count], sizeof(vault_entry), 1, f_vault) == 1) {
        count++;
        if (count == capacity) {
            capacity *= 2;
            vault_entry *new_array = realloc(array, sizeof(vault_entry) * capacity);
            array = new_array;
        }
    }
    fclose(f_vault);
    *count_out = count;
    *array_out = array;
    return OK;
}

file_operation_result_t get_from_vault(const uint32_t hash_key, vault_entry *entry) {
    bool found = false;

    FILE *f_idx = fopen(VAULT_IDX, "r");
    if (f_idx == NULL) {
        return ERROR;
    }
    vault_index_entry index_entry;

    while (fread(&index_entry, sizeof(vault_index_entry), 1, f_idx) == 1) {
        if (index_entry.hash == hash_key) {
            found = true;
            break;
        }
    }
    fclose(f_idx);

    if (!found)
        return ERROR;

    FILE *f_vault = fopen(VAULT_FILENAME, "r");
    if (f_vault == NULL) {
        return ERROR;
    }
    if (fseek(f_vault, index_entry.offset, SEEK_SET) != 0) {
        fclose(f_vault);
        return ERROR;
    }
    if (fread(entry, sizeof(vault_entry), 1, f_vault) != 1) {
        fclose(f_vault);
        return ERROR;
    }
    fclose(f_vault);
    return OK;
}

file_operation_result_t put_to_vault(const vault_entry *entry) {

    //aggiungi entry nel file .dat
    FILE *f_vault_dat = fopen(VAULT_FILENAME, "ab+");
    if (!f_vault_dat) {
        return ERROR;
    }
    size_t current_offset = ftell(f_vault_dat);
    fwrite(entry, sizeof(vault_entry), 1, f_vault_dat);
    fclose(f_vault_dat);

    vault_index_entry *entry_index = calloc(1, sizeof(vault_index_entry));
    entry_index->hash = calc_hash(entry->key);
    entry_index->offset = current_offset;

    //aggiungi l'entry ordinata nell'index con l'offset corretto
    FILE *f_vault_idx = fopen(VAULT_IDX, "rb+");
    if (!f_vault_idx) {
        //file not exist, create new one
        f_vault_idx = fopen(VAULT_IDX, "wb");
        if (!f_vault_idx) {
            return ERROR;
        }
        fwrite(entry_index, sizeof(vault_index_entry), 1, f_vault_idx);
        fclose(f_vault_idx);
        free(entry_index);
        return OK;
    }

    //il file esiste, quindi ne creo uno nuovo, inserisco ne entry mano a mano e appena trovo la posizione giusta per il nuovo la inserisco
    FILE *f_value_idx_temp = fopen(VAULT_IDX_TEMP, "wb");
    vault_index_entry *buffer = malloc(sizeof(vault_index_entry));

    int inserted = 0;
    while (fread(buffer, sizeof(vault_index_entry), 1, f_vault_idx) == 1) {
        if (!inserted && buffer->offset > current_offset) {
            fwrite(entry_index, sizeof(vault_index_entry), 1, f_value_idx_temp);
            inserted = 1;
        }
        fwrite(buffer, sizeof(vault_index_entry), 1, f_value_idx_temp);
    }

    if (!inserted) {
        fwrite(entry_index, sizeof(vault_index_entry), 1, f_value_idx_temp);
    }

    fclose(f_value_idx_temp);
    fclose(f_vault_idx);
    free(buffer);
    free(entry_index);

    remove(VAULT_IDX);
    rename(VAULT_IDX_TEMP, VAULT_IDX);
    return OK;
}

vault_entry *vault_entry_create() {
    vault_entry *entry = calloc(1, sizeof(vault_entry));
    return entry;
}

void vault_entry_destroy(vault_entry *entry) {
    free(entry);
}

bool vault_entry_exist(const char *key) {
    uint32_t hash = calc_hash(key);
    FILE *f_vault_idx = fopen(VAULT_IDX, "rb");
    if (!f_vault_idx) return false;

    vault_index_entry entry_index;
    while (fread(&entry_index, sizeof(entry_index), 1, f_vault_idx) == 1) {
        if (entry_index.hash == hash) {
            fclose(f_vault_idx);
            return true;
        }
    }

    fclose(f_vault_idx);
    return false;
}
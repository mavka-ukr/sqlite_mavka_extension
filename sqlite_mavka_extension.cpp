#include <stdlib.h>
#include <memory.h>
#include "sqlite/sqlite3.h"

extern "C" void *sqlite_mavka_extension_open(char *path, size_t path_size) {
    char *path_str = (char *) malloc(path_size + 1);
    memcpy(path_str, path, path_size);
    path_str[path_size] = '\0';

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(path_str, &db);
    free(path_str);

    if (rc) {
        return nullptr;
    }

    return db;
}
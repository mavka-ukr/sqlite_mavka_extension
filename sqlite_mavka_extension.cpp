#include <stdlib.h>
#include <memory.h>
#include "sqlite/sqlite3.h"
#include <iostream>

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

typedef int (*SqliteExecCallback)(void *data, int argc, char **argv, char **azColName);

extern "C" int
sqlite_mavka_extension_exec(void *db, char *sql, size_t sql_size, SqliteExecCallback callback, void *data,
                            char **error_ptr, size_t *error_size_ptr) {
    char *sql_str = (char *) malloc(sql_size + 1);
    memcpy(sql_str, sql, sql_size);
    sql_str[sql_size] = '\0';

    char *zErrMsg = 0;
    int rc = sqlite3_exec((sqlite3 *) db, sql_str, callback, data, &zErrMsg);

    free(sql_str);

    if (rc != SQLITE_OK) {
        *error_ptr = zErrMsg;
        *error_size_ptr = strlen(zErrMsg);
        return 0;
    }

    return 1;
}

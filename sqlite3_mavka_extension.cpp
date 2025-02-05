#include <stdlib.h>
#include <memory.h>
#include "sqlite3/sqlite3.h"
#include <iostream>

extern "C" void *sqlite_mavka_extension_open(char *path, size_t path_size, char **error, size_t *error_size) {
    char *path_str = (char *) malloc(path_size + 1);
    memcpy(path_str, path, path_size);
    path_str[path_size] = '\0';

    sqlite3 *db = nullptr;

    int rc = sqlite3_open(path_str, &db);
    free(path_str);

    if (rc) {
        *error = strdup("cannot open database");
        *error_size = strlen(*error);
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

extern "C" void *sqlite_mavka_extension_prepare_v2(void *db, char *sql, size_t sql_size) {
    char *sql_str = (char *) malloc(sql_size + 1);
    memcpy(sql_str, sql, sql_size);
    sql_str[sql_size] = '\0';

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2((sqlite3 *) db, sql_str, sql_size, &stmt, nullptr);

    free(sql_str);

    if (rc != SQLITE_OK) {
        return nullptr;
    }

    return stmt;
}

extern "C" int sqlite_mavka_extension_bind_double(void *stmt, int i, double value) {
    return sqlite3_bind_double((sqlite3_stmt *) stmt, i, value);
}

extern "C" int sqlite_mavka_extension_bind_int(void *stmt, int i, int value) {
    return sqlite3_bind_int((sqlite3_stmt *) stmt, i, value);
}

extern "C" int sqlite_mavka_extension_bind_int64(void *stmt, int i, long value) {
    return sqlite3_bind_int64((sqlite3_stmt *) stmt, i, value);
}

extern "C" int sqlite_mavka_extension_bind_null(void *stmt, int i) {
    return sqlite3_bind_null((sqlite3_stmt *) stmt, i);
}

extern "C" int sqlite_mavka_extension_bind_text(void *stmt, int i, char *value, size_t value_size) {
    return sqlite3_bind_text((sqlite3_stmt *) stmt, i, value, value_size, nullptr);
}

extern "C" size_t sqlite_mavka_extension_expanded_sql(void *stmt, char **out) {
    *out = sqlite3_expanded_sql((sqlite3_stmt *) stmt);
    return strlen(*out);
}
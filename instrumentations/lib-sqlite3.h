#ifndef CALI_LIB_SQLITE3_H
#define CALI_LIB_SQLITE3_H

#include <zlib.h>
#include <sqlite3.h>
#include "utils.h"

void instrument_before_sqlite3_malloc(int size) {
	if (SQLITE_VERBOSE) debug_printf("sqlite3_malloc(%d)", size);
}

void instrument_after_sqlite3_malloc(void *result) {
	if (SQLITE_VERBOSE | check_ptr(result)) debug_printf(" => %p", result);
}

void instrument_before_sqlite3_free(void *ptr) {
	if (SQLITE_VERBOSE | check_ptr(ptr)) debug_printf("sqlite3_free(%p)", ptr);
}

void instrument_before_sqlite3_finalize(sqlite3_stmt *pStmt) {
	if (SQLITE_VERBOSE | check_ptr(pStmt)) debug_printf("sqlite3_finalize(%p)", pStmt);
}

void instrument_before_sqlite3_open(const char *filename, sqlite3 **ppDb) {
	if (SQLITE_VERBOSE | check_ptr(filename) | check_ptr(ppDb)) debug_printf("sqlite3_open(%p, %p)", filename, ppDb);
}

void instrument_before_sqlite3_close(sqlite3 *db) {
	if (SQLITE_VERBOSE | check_ptr(db)) debug_printf("sqlite3_close(%p)", db);
}

void instrument_before_sqlite3_errmsg(sqlite3 *db) {
	if (SQLITE_VERBOSE | check_ptr(db)) debug_printf("sqlite3_errmsg(%p)", db);
}

void instrument_before_sqlite3_busy_timeout(sqlite3 *db, int ms) {
	if (SQLITE_VERBOSE | check_ptr(db)) debug_printf("sqlite3_busy_timeout(%p, %d)", db, ms);
}

void instrument_before_sqlite3_busy_handler(sqlite3 *db, int(*f)(void *, int), void *param) {
	if (SQLITE_VERBOSE | check_ptr(db) | check_ptr(param)) debug_printf("sqlite3_busy_handler(%p, %p, %p)", db, f, param);
}

void instrument_before_sqlite3_exec(sqlite3 *db,                               /* An open database */
									const char *sql,                           /* SQL to be evaluated */
									int (*callback)(void *, int, char **, char **),  /* Callback function */
									void *param,                               /* 1st argument to callback */
									char **errmsg                              /* Error msg written here */) {
	if (SQLITE_VERBOSE | check_ptr(db) | check_ptr(sql) | check_ptr(param) | check_ptr(errmsg))
		debug_printf("sqlite3_exec(%p, %p, %p, %p, %p)", db, sql, callback, param, errmsg);
}

void instrument_before_sqlite3_vmprintf(const char *s, va_list va) {
	debug_printf("Cannot check vmprintf arguments (\"%s\")", s);
	/*for (int i = 0; s[i]; i++) {
		if (s[i] == '%') {
			if (s[i+1] == 'p' || s[i+1] == 'Q') {
				void* p = va_arg(va, void*);
				debug_printf("- %d: %p", i, p);
				check_ptr(p);
			} else {
				int n = va_arg(va, int);
				debug_printf("- %d: %d", i, n);
			}
		}
	}*/
	if (SQLITE_VERBOSE | check_ptr(s)) debug_printf("sqlite3_vmprintf(%p, ...)", s);
}

void instrument_after_sqlite3_vmprintf(char *result) {
	debug_printf(" => %p \"%s\"", result, result);
	if (SQLITE_VERBOSE | check_ptr(result)) debug_printf(" => %p", result);
}

void instrument_before_sqlite3_db_handle(sqlite3_stmt *stmt) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_db_handle(%p)", stmt);
}

void instrument_before_sqlite3_prepare_v2(sqlite3 *db,            /* Database handle */
										  const char *zSql,       /* SQL statement, UTF-8 encoded */
										  int nByte,              /* Maximum length of zSql in bytes. */
										  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
										  const char **pzTail     /* OUT: Pointer to unused portion of zSql */) {
	if (SQLITE_VERBOSE | check_ptr(db) | check_ptr(zSql) | check_ptr(ppStmt) | check_ptr(pzTail))
		debug_printf("sqlite3_prepare_v2(%p, %p, %d, %p, %p)", db, zSql, nByte, ppStmt, pzTail);
}

void instrument_before_sqlite3_bind_parameter_count(sqlite3_stmt *stmt) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_bind_parameter_count(%p)", stmt);
}

void instrument_before_sqlite3_bind_int(sqlite3_stmt *stmt, int n, int i) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_bind_int(...)");
}

void instrument_before_sqlite3_bind_int64(sqlite3_stmt *stmt, int n, sqlite3_int64 i) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_bind_int64(...)");
}

void instrument_before_sqlite3_bind_text(sqlite3_stmt *stmt, int n, const char *s, int n2, void(*f)(void *)) {
	if (SQLITE_VERBOSE | check_ptr(stmt) | check_ptr(s)) debug_printf("sqlite3_bind_text(%p, %d, %p, %d, %p)", stmt, n, s, n2, f);
}

void instrument_before_sqlite3_bind_blob(sqlite3_stmt *stmt, int n, void *s, int n2, void(*f)(void *)) {
	if (SQLITE_VERBOSE | check_ptr(stmt) | check_ptr(s)) debug_printf("sqlite3_bind_blob(%p, %d, %p, %d, %p)", stmt, n, s, n2, f);
}

void instrument_before_sqlite3_step(sqlite3_stmt *stmt) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_step(%p)", stmt);
}

void instrument_before_sqlite3_column_count(sqlite3_stmt *stmt) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_count(%p)", stmt);
}

void instrument_before_sqlite3_column_name(sqlite3_stmt *stmt, int N) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_name(...)");
}

void instrument_before_sqlite3_column_text(sqlite3_stmt *stmt, int iCol) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_text(...)");
}

void instrument_before_sqlite3_column_type(sqlite3_stmt *stmt, int iCol) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_type(...)");
}

void instrument_before_sqlite3_reset(sqlite3_stmt *pStmt) {
	if (SQLITE_VERBOSE | check_ptr(pStmt)) debug_printf("sqlite3_reset(%p)", pStmt);
}

void instrument_before_sqlite3_errstr(int x) {
	if (SQLITE_VERBOSE) debug_printf("sqlite3_errstr(%d)", x);
}


void instrument_before_sqlite3_bind_null(sqlite3_stmt *stmt, int x) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_bind_null(...)");
}

void instrument_before_sqlite3_column_bytes(sqlite3_stmt *stmt, int iCol) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_bytes(...)");
}

void instrument_before_sqlite3_column_int(sqlite3_stmt *stmt, int iCol) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_int(...)");
}

void instrument_before_sqlite3_column_int64(sqlite3_stmt *stmt, int iCol) {
	if (SQLITE_VERBOSE | check_ptr(stmt)) debug_printf("sqlite3_column_int64(...)");
}

void instrument_before_sqlite3_last_insert_rowid(sqlite3 *db) {
	if (SQLITE_VERBOSE | check_ptr(db)) debug_printf("sqlite3_last_insert_rowid(...)");
}


#endif //CALI_LIB_SQLITE3_H

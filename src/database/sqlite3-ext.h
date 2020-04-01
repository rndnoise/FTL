/* Pi-hole: A black hole for Internet advertisements
*  (c) 2020 Pi-hole, LLC (https://pi-hole.net)
*  Network-wide ad blocking via your own hardware.
*
*  FTL Engine
*  SQLite3 database engine extension prototypes
*
*  This file is copyright under the latest version of the EUPL.
*  Please see LICENSE file for your rights under this license. */

// Make implementations globally available
extern void subnet_match_impl(sqlite3_context *context, int argc, sqlite3_value **argv);

// Initialization point for SQLite3 extensions
extern int sqlite3_pihole_extensions_init(sqlite3 *db, const char **pzErrMsg, const struct sqlite3_api_routines *pApi);

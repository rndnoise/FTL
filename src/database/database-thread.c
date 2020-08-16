/* Pi-hole: A black hole for Internet advertisements
*  (c) 2017 Pi-hole, LLC (https://pi-hole.net)
*  Network-wide ad blocking via your own hardware.
*
*  FTL Engine
*  Database thread
*
*  This file is copyright under the latest version of the EUPL.
*  Please see LICENSE file for your rights under this license. */

#include "../FTL.h"
#include "database-thread.h"
#include "common.h"
// [un]lock_shm();
#include "../shmem.h"
// parse_neighbor_cache()
#include "network-table.h"
// DB_save_queries()
#include "query-table.h"
#include "../config.h"
#include "../log.h"
#include "../timers.h"
// global variable killed
#include "../signals.h"

void *DB_thread(void *val)
{
	// Set thread name
	prctl(PR_SET_NAME,"database",0,0,0);

	// First check if the user doesn't want to use the database.
	// If this is the case, we terminate the database thread as
	// there is no point in having it around
	if(!use_database())
		return NULL;

	// Save timestamp as we do not want to store immediately
	// to the database
	time_t lastDBsave = time(NULL) - time(NULL)%config.DBinterval;

	while(!killed && database)
	{
		if(time(NULL) - lastDBsave >= config.DBinterval)
		{
			// Update lastDBsave timer
			lastDBsave = time(NULL) - time(NULL)%config.DBinterval;

			// Lock FTL's data structures, since it is
			// likely that they will be changed here
			lock_shm();

			// Save data to database
			DB_save_queries();

			// Release data lock
			unlock_shm();

			// Check if GC should be done on the database
			if(DBdeleteoldqueries)
			{
				// No thread locks needed
				delete_old_queries_in_DB();
				DBdeleteoldqueries = false;
			}

			// Request neighbor cache parsing
			want_neighbor_parsing = true;
		}

		// Parse neighbor cache (fill network table)
		// if requested and enabled
		if(want_neighbor_parsing && config.parse_arp_cache)
		{
			want_neighbor_parsing = false;
			parse_neighbor_cache();
		}

		sleepms(100);
	}

	return NULL;
}

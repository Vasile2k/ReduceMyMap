#pragma once

/*
 * Rank 0 is always master
 * Other ranks are ordinary servants
 */
 #define MASTER_ID 0

/*
 * The packet sent to master when the worker is free
 */
#define WORKER_FREE 30

#pragma once

/*
 * Argument to specify input path
 */
const char* const INPUT_PATH_ARGUMENT = "-i";

/*
 * Argument to specify output path
 */
const char* const OUTPUT_PATH_ARGUMENT = "-o";

/*
 * Rank 0 is always master
 * Other ranks are ordinary servants
 */
constexpr int MASTER_ID = 0;

/*
 * The packet sent to master when the worker is free
 */
#define PACKET_WORKER_FREE ((char)0x30)

/*
 * The packet sent to servant when it has to map document to word count
 */
#define PACKET_MAP_TO_WORDS ((char)0x69)

/*
 * The packet sent to servant when it has to reduce word counts to one with words
 * starting with one letter
 */
#define PACKET_REDUCE_TO_LETTER ((char)0xA0)

/*
 * The packet sent to servant when the work is done and it can close
 */
#define PACKET_SUICIDE ((char)0x45)

/*
 * This packet is kept into the master and it knows it has to wait after all servants
 */
#define PACKET_BARRIER ((char)0x77)

/*
 * Separator for data in messages
 * Somethin' that can't be in filename or path
 */
constexpr char MESSAGE_SEPARATOR = '(';

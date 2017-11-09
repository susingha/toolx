#include "sharedmem.h"

#define ZERO 0
#define BEGIN_LOC_INDEX ZERO                                // 0
#define END_LOC_INDEX (BEGIN_LOC_INDEX + 1)                 // 1
#define COUNT_LOC_INDEX (END_LOC_INDEX + 1)                 // 2
#define QUEUE_LOC_INDEX (COUNT_LOC_INDEX + 1)               // 3
#define QUEUE_LENGTH (SHMSZ - QUEUE_LOC_INDEX)             // 27 - 3 = 24

/*

|              |            |              |                   | | | | | | | | | |                    |
| stores begin | stores end | stores count | 1st cell in queue | | | | | | | | | | last cell in queue |
|       0      |     1      |      2       |        3          | | | | | | | | | |         26         |    27

*/


char * get_queue(char * s) {
    return &s[QUEUE_LOC_INDEX];
}

unsigned int get_begin(char * s) {
    return s[BEGIN_LOC_INDEX];
}
unsigned int get_end(char * s) {
    return s[END_LOC_INDEX];
}

unsigned int get_count(char * s) {
    return s[COUNT_LOC_INDEX];
}

unsigned int inc_begin(char * s) {
    ++s[BEGIN_LOC_INDEX];
    --s[COUNT_LOC_INDEX];
    if(s[BEGIN_LOC_INDEX] == SHMSZ) {
	s[BEGIN_LOC_INDEX] = QUEUE_LOC_INDEX;
    }
    return s[BEGIN_LOC_INDEX];
}
unsigned int inc_end(char * s) {
    ++s[END_LOC_INDEX];
    ++s[COUNT_LOC_INDEX];
    if(s[END_LOC_INDEX] == SHMSZ) {
	s[END_LOC_INDEX] = QUEUE_LOC_INDEX;
    }
    return s[END_LOC_INDEX];
}

int queue_full(char * s) {
    if(get_count(s) == QUEUE_LENGTH)
	return 1;
    else
	return 0;
}

int queue_empty(char * s) {
    if(get_count(s) == ZERO)
	return 1;
    else
	return 0;
}





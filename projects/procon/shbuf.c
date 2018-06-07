#include "sharedmem.h"
#include "shbuf.h"

#define ZERO 0

#if 0
#define BEGIN_LOC_INDEX ZERO                                // 0
#define END_LOC_INDEX (BEGIN_LOC_INDEX + 1)                 // 1
#define COUNT_LOC_INDEX (END_LOC_INDEX + 1)                 // 2
#define QUEUE_LOC_INDEX (COUNT_LOC_INDEX + 1)               // 3
#define QUEUE_LENGTH (SHMSZ - QUEUE_LOC_INDEX)             // 27 - 3 = 24
#endif


char * get_queue(shbuf_t * s) {
    return &s->buf[0];
}

unsigned int get_begin(shbuf_t * s) {
    return s->begin;
}
unsigned int get_end(shbuf_t * s) {
    return s->end;
}

unsigned int get_count(shbuf_t * s) {
    return s->count;
}

unsigned int inc_begin(shbuf_t * s) {
    ++s->begin;
    --s->count;
    if(s->begin == SHMBUFSiZE)
	s->begin = ZERO;
    return s->begin;
}
unsigned int inc_end(shbuf_t * s) {
    ++s->end;
    ++s->count;
    if(s->end == SHMBUFSiZE) {
	s->end = ZERO;
    }
    return s->end;
}

int queue_full(shbuf_t * s) {
    if(get_count(s) == SHMBUFSiZE)
	return 1;
    else
	return 0;
}

int queue_empty(shbuf_t * s) {
    if(get_count(s) == ZERO)
	return 1;
    else
	return 0;
}





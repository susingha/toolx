#include <stdio.h>
#include "sharedmem.h"
#include "shbuf.h"

#define ZERO 0

int f = 0;
int e = 0;

int buf_init(shbuf_t * s) {
    s->begin = 0;
    s->end = SHMBUFSiZE-1;
    s->count = 0;
    return 0;
}

void insert_item(shbuf_t * s, char c) {
    inc_end(s);
    s->buf[get_end(s)] = c;

    if (queue_full(s)) {
	printf("\nqueue is full\n");
	f++;
    } else {
	printf("%u, ", get_count(s));
    }
}
char remove_item(shbuf_t * s) {
    char c = s->buf[get_begin(s)];
    s->buf[get_begin(s)] = 0;
    inc_begin(s);

    if (queue_empty(s)) {
	printf("\nqueue is empty\n");
	e++;
    } else {
	printf("%u, ", get_count(s));
    }

    return c;
}


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





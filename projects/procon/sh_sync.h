
#define SHMBUFSiZE 32 // keep this a multiple of 4, avoid padding issues

typedef struct shbuf {
    int begin;
    int end;
    int count;
    int padding;
    char buf[SHMBUFSiZE];
} shbuf_t;

#if 0

|              |            |              |                   | | | | | | | | | |                    |
| stores begin | stores end | stores count | 1st cell in queue | | | | | | | | | | last cell in queue |
|       0      |     1      |      2       |        3          | | | | | | | | | |         26         |    27

#endif

#define SHMSIZE (sizeof(shbuf_t))

char * get_queue(shbuf_t * s);
unsigned int get_begin(shbuf_t * s);
unsigned int get_end(shbuf_t * s);
unsigned int get_count(shbuf_t *s);
unsigned int inc_begin(shbuf_t * s);
unsigned int inc_end(shbuf_t * s);
int queue_full(shbuf_t * s);
int queue_empty(shbuf_t * s);

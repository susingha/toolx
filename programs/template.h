#include <time.h>
#include <unistd.h>

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

// Random Numbers

#define RANDMAX (100)
#define RANDINIT (time(NULL)) // can be 0 also

int randmax = RANDMAX;

int getrandom()
{
    static int randinit = FALSE;
    if (!randinit) {
	srand(RANDINIT);
	randinit = TRUE;
    }
    return rand()%randmax;
}


// Arguments

#define printsize(x) printf(#x": %lu\n", sizeof(x));

void printargs(int argc, char *argv[])
{
    int c;
    printf("pid: %u ", getpid());
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);
#if 1
    printsize(char);
    printsize(short int);
    printsize(int);
    printsize(long);
    printsize(long int);
    printsize(long long int);
    printsize(double);
    printsize(long double);
#endif
}


void loadcpu(unsigned int i)
{
    while (1) while(--i);
}

void loadmem()
{
    unsigned long int nbytes = 1;
    unsigned long int tbytes = 0;
    void * ptr = NULL;

    do {
	nbytes = nbytes + nbytes;
	ptr = (void *)malloc(nbytes);
	printf("malloc %lu bytes at %p\n", nbytes, ptr);
	if (ptr) tbytes += nbytes;
	sleep(1);
    } while (ptr);

    printf("malloc fail for %lu bytes. total %lu allocated\n", nbytes, tbytes);
}

// Run Time Tracking

clock_t _tsall_ = 0;
clock_t _tscur_ = 0;
int trackonstate = FALSE;

void trackon()
{
    if (trackonstate)
	return;

    _tsall_ = 0;
    _tscur_ = clock();
    trackonstate = TRUE;
}

void trackoff()
{
    if (!trackonstate)
	return;

    trackonstate = FALSE;
    _tscur_ = clock() - _tscur_;
    _tsall_ += _tscur_;
}

void trackprint()
{
    int trackerwason = trackonstate;
    trackoff();

    printf("Runtime: %f seconds\n", ((double)_tsall_) / CLOCKS_PER_SEC); 

    if (trackerwason)
    trackon();
}



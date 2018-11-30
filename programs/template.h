#include <time.h>

#define TRUE  1
#define FALSE 0
#define ENDS  ('\0')

// Random Numbers

#define RANDMAX (100)
#define RANDINIT (time(NULL)) // can be 0 also

int getrandom()
{
    static int randinit = FALSE;
    if (!randinit) {
	srand(RANDINIT);
	randinit = TRUE;
    }
    return rand()%RANDMAX;
}


// Arguments

void printargs(int argc, char *argv[])
{
    int c;
    for (c = 0; c < argc; ++c) {
	printf("%s ", argv[c]);
    }
    printf("(%d)\n", argc);
}


// Run Time Tracking

clock_t _tsall_ = 0;
clock_t _tscur_ = 0;

void trackon()
{
    _tscur_ = clock();
}

void trackoff()
{
    _tscur_ = clock() - _tscur_;
    _tsall_ += _tscur_;
}

void trackprint()
{
    printf("Runtime: %f seconds\n", ((double)_tsall_) / CLOCKS_PER_SEC); 
}



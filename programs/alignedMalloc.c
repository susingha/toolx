
// Write an aligned malloc and free function
// Th ptr returned must be on the alignment boundary
// Assumption: aligned_malloc() and aligned_free() must work with the same alignment value per allocation

#include <stdio.h>
#include <stdlib.h>

#define GET (0)
void alignment(size_t align)
{
    static size_t alignmentval;
    if (align)
	alignmentval = align;
    return alignmentval;
}

void *
mallocaligned(size_t size, size_t align) {

    unsigned long long int i;

    size_t total = align + align + size;
    void * ptr = malloc(total);
    printf("\n");
    printf("%p to %lu bytes allocated for %lu bytes request\n", ptr, total, size);

    unsigned long long int tmp = (unsigned long long int)ptr / align;
    printf("%llu is at tmp\n", tmp);
    void * flor = (void *) (tmp * align);
    void * ceil = (void *)((tmp + 1) * align);
    printf("%p is at ceiling\n", ceil);

    i = (unsigned long long)ceil;
    printf("0x%llx is at ceiling\n", i);

    printf("rem = %llu\n", (unsigned long long)ptr % align);

    size_t padding = ceil - ptr;
    *(size_t *)ceil = padding;

    return ceil + sizeof(size_t);
}

void
freealigned(void * freeptr) {
    void * ceil = freeptr - sizeof(size_t);
    size_t padding = *(size_t *)ceil;
    void * ptr = freeptr - sizeof(size_t) - padding;

    free(ptr);
    printf("%p freed\n", ptr);
    return;
}

int main() {

    void * ptr;
    size_t size, align;

    
#if 0
    ize = 8; align = 4;
    ptr = aligned_malloc(size, align);
    aligned_free(ptr);
    if ((size_t)ptr % align) printf("WARNING: alignment not ok: size: %lu, align: %lu\n", size, align);
#endif

    size = 8; align = 128;
    ptr = aligned_malloc(size, align);
    aligned_free(ptr);
    if ((size_t)ptr % align)
	printf("WARNING: alignment not ok: size: %lu, align: %lu\n", ptr, size, align);
    else
	printf("OK: alignment is correct: %p, size: %lu, align: %lu\n", ptr, size, align);

#if 0
    size = 81; align = 12;
    ptr = aligned_malloc(size, align);
    aligned_free(ptr);
    if ((size_t)ptr % align) printf("WARNING: alignment not ok: size: %lu, align: %lu\n", size, align);

    size = 123; align = 127;
    ptr = aligned_malloc(size, align);
    aligned_free(ptr);
    if ((size_t)ptr % align) printf("WARNING: alignment not ok: size: %lu, align: %lu\n", size, align);

    size = 13; align = 21;
    ptr = aligned_malloc(size, align);
    aligned_free(ptr);
    if ((size_t)ptr % align) printf("WARNING: alignment not ok: size: %lu, align: %lu\n", size, align);
#endif

    return 0;
}

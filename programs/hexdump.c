#include <stdio.h>
#include <stdint.h>
#include <string.h>


uintptr_t xtoi(char hexc)
{
    uintptr_t ptr;

    switch (hexc) {
	case '0': ptr = (uintptr_t)0;  break;
	case '1': ptr = (uintptr_t)1;  break;
	case '2': ptr = (uintptr_t)2;  break;
	case '3': ptr = (uintptr_t)3;  break;
	case '4': ptr = (uintptr_t)4;  break;
	case '5': ptr = (uintptr_t)5;  break;
	case '6': ptr = (uintptr_t)6;  break;
	case '7': ptr = (uintptr_t)7;  break;
	case '8': ptr = (uintptr_t)8;  break;
	case '9': ptr = (uintptr_t)9;  break;
	case 'a':
	case 'A': ptr = (uintptr_t)10; break;
	case 'b':
	case 'B': ptr = (uintptr_t)11; break;
	case 'c':
	case 'C': ptr = (uintptr_t)12; break;
	case 'd':
	case 'D': ptr = (uintptr_t)13; break;
	case 'e':
	case 'E': ptr = (uintptr_t)14; break;
	case 'f':
	case 'F': ptr = (uintptr_t)15; break;
	default:
		  return 0;
    }

    return ptr;
}

unsigned int hexok(char c)
{
    if (('0' <= c && c <= '9') ||
	('a' <= c && c <= 'f') ||
	('A' <= c && c <= 'F'))
	return 1;
    return 0;
}

void * addressat(char * address) 
{
    int       i = -1;
    uintptr_t x = 0;
    uintptr_t ptr;
    void     *ptrx = NULL;
    char     *ptrs;
    int       len;
    int       shiftby;
 
    ptrs = address;
    len = strlen(ptrs);

#ifdef DEBUG
    printf("%s\n", ptrs);
    ptr = 0;
    printf("%llx, size: %ld\n", ptr, sizeof(ptr));
    ptr = ptr - 1;
    printf("%llx, size: %ld\n", ptr, sizeof(ptr));
#endif

    ptr = 0;
    for (i = len-1; i >= 0 && hexok(ptrs[i]); i--) {
	ptrs[i+1] = '\0';
	x = xtoi(ptrs[i]);

#ifdef DEBUG
	printf("%llx\n", x);
#endif
	shiftby = (4 * (len-1 - i));
	ptr |= ((uintptr_t)x  <<  shiftby);
    }

    ptrx = (void *)ptr;
#ifdef DEBUG
    printf("%p\n", ptrx);
#endif
    return ptrx;
}


void hexDump(char *desc, void *addr, int len) 
{
    int i, diff;
    unsigned char buff[17];
    unsigned char *pc;

    uintptr_t begin = (uintptr_t)addr & ~((uintptr_t)0xF);

    pc = (unsigned char*)begin;
    diff = (uintptr_t)addr - (uintptr_t)pc;
    len += diff;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %p ", pc + i);
        }

	if ((i % 8) == 0) {
	    printf(" ");
	}

	if (i < diff) {
	    printf("   ");
	} else {
	    // Now the hex code for the specific character.
	    printf(" %02x", pc[i]);
	}

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
	if ((i % 8) == 0)
	    printf(" ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

int main ()
{
    unsigned int i = 19851103;
    void * ptr;
    char address[1024];
    printf("&i = %p\n", &i);
    printf("Enter an address: ");
    scanf("%s", address);

    ptr = addressat(address);
    printf("i = %u\n", *(unsigned int *)ptr);

    hexDump("Testing", ptr, 4);
}



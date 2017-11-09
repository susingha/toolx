#include <stdio.h>
#include "../types_global.h"
#include "sharedmem.h"
#include "sh_sync.h"

char source_arr[] = "the create_ras is called for any type of RAS message irrespective of whether its a RRQ or an SCI. This may lead to GK being recorded as EP ip address and v.v. add this exception as a special case";
char get_next() {
    static int i = 0;
    return source_arr[i++];
}

int write(char packet, char * s) {
    if(!queue_full(s)) {
	s[get_end(s)] = packet;
	inc_end(s);
	return TRUE;
    } else {
	return FALSE;
    }
}


int main() {
    char packet = '\0';
    char * s = NULL;
    s = get_sharedmem(SHKEY, SHMSZ);
    if (s) {
	printf("producer: shared memory was obtained at %p\n", s);
    }

    printf("Write: ");
    do {
	packet = get_next();
	while(!write(packet, s)); // sup:edit
	printf("%c", packet);
    } while(packet != '\0');

    printf("\n");
    return 0;
}



#ifdef SUP

write()
{

    s = shm;

    for (c = 'a'; c <= 'z'; c++)
	*s++ = c;
    *s = NULL;

    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
	sleep(1);

    exit(0);
}

#endif

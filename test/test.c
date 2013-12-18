#include <stdio.h>
#include <stdlib.h>

/******************************************************************************************************
 * Offset Setter Functions                                                                             *
 ******************************************************************************************************/
void setAdminOffset(void *base_address, unsigned long ap){
    printf("\nsup: base_address = %p", base_address);
    if(base_address && ap > 0){
	unsigned long *index_location = (unsigned long *)(base_address + sizeof(unsigned long) * 0);
	printf("\nsup:1 ap = %d", ap);
	while(((unsigned long)base_address + (unsigned long)ap) % sizeof(int) != 0)
	    ap++;
	printf("\nsup:2 ap = %d", ap);
	*index_location = ap;    
    }
}


struct mov {
    int a;
};

int main() {
    void * a = (void *)malloc(25);


    printf("unsigned long = %d, struct = %d", sizeof(unsigned long), sizeof(struct mov));
    system("/bin/zsh");
    return 0;

    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);
    setAdminOffset(a++, 64);

    return 0;
}




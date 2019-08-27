/*
 * A C program to demonstrate Zombie Process.  
 * Child becomes Zombie as parent is sleeping 
 * when child process exits. 
 */

#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define ONE 1

int main() 
{ 
    pid_t child_pid = fork(); 
    unsigned int i = 0;
  
    if (child_pid > 0) { // Parent process  

	while (1) while(--i); 
  
    } else {             // Child process 

        exit(0);
    }
  
    return 0; 
} 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>




int ascix(char c) {
    return c - 'a';
}

void swap(int i, int j, char **arr) {
    char * tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}



int lexicographic_sort(const char* a, const char* b) {
    return strcmp(a, b);
}

int lexicographic_sort_reverse(const char* a, const char* b) {
    return strcmp(b, a);
}

int sort_by_number_of_distinct_characters(const char* a, const char* b) {
    int len = 0;
    int i;
    char hash[26];
    int counta = 0, countb = 0;
    
    memset(hash, 0, sizeof(hash));
    len = strlen(a);
    for (i = 0; i < len; ++i) {
        if (hash[ascix(a[i])] == 0) {
            hash[ascix(a[i])] = 1;
            counta++;
        }
    }
    
    memset(hash, 0, sizeof(hash));
    len = strlen(b);
    for (i = 0; i < len; ++i) {
        if (hash[ascix(b[i])] == 0) {
            hash[ascix(b[i])] = 1;
            countb++;
        }
    }
    
    if (counta == countb)
	return strcmp(a, b);

    return counta - countb;
}


int sort_by_length(const char* a, const char* b) {
    int diff = strlen(a) - strlen(b);
    
    if (!diff) {
        return strcmp(a, b);
    }
    
    return diff;
}

void string_sort(char** arr,const int len, int (*cmp_func)(const char* a, const char* b)){
    
    int i, j, mini = 0;
    /* selection sort */
    for (i = 0; i < len; i++) {
        mini = i;
        for (j = i; j < len; ++j) {
            if (cmp_func(arr[mini], arr[j]) >= 0) {
                mini = j;
            }
        }
        swap (mini, i, arr);
    }
}

#define rnd() (rand()%100)

int main() 
{
    unsigned int i, n;
    char** arr;
    void * ptr;


    srand(time(NULL));
    for (i = 0; i < 20; ++i) {
	n = rnd();
	// printf("n = %u\n", n);
	ptr = malloc(n);
	printf("ptr = %p, rem  4 = %lu, "
		         "rem  8 = %lu, "
		         "rem 16 = %lu, "
			 "rem 32 = %lu\n",
		ptr, (uintptr_t)ptr %  4,
		     (uintptr_t)ptr %  8,
		     (uintptr_t)ptr % 16,
		     (uintptr_t)ptr % 32);
    }

    return 0;


    scanf("%d", &n);
  
    arr = (char**)malloc(n * sizeof(char*));
  
    for(int i = 0; i < n; i++){
        *(arr + i) = malloc(1024 * sizeof(char));
        scanf("%s", *(arr + i));
        *(arr + i) = realloc(*(arr + i), strlen(*(arr + i)) + 1);
    }
  
    string_sort(arr, n, lexicographic_sort);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, lexicographic_sort_reverse);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]); 
    printf("\n");

    string_sort(arr, n, sort_by_length);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);    
    printf("\n");

    string_sort(arr, n, sort_by_number_of_distinct_characters);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]); 
    printf("\n");

}

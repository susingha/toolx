#include <stdio.h>


#include <stdio.h>
#include <stdlib.h>

/*
    * This stores the total number of books in each shelf.
     */
int* total_number_of_books;

/*
    * This stores the total number of pages in each book of each shelf.
     * The rows represent the shelves and the columns represent the books.
      */
int** total_number_of_pages;

int main () {

    int bookcount;
    int total_number_of_shelves;
    // scanf("%d", &total_number_of_shelves);
    total_number_of_shelves  = 9; // 99788;


    int total_number_of_queries;
    // scanf("%d", &total_number_of_queries);
    total_number_of_queries  = 9; // 99568;

    printf("l:%d\n", __LINE__);
    total_number_of_pages = (int (*)[1100]) calloc (sizeof(int), total_number_of_shelves * 1100);
    printf("l:%d, total_number_of_pages = %p\n", __LINE__, total_number_of_pages);
    total_number_of_books = (int *) calloc (sizeof(int), total_number_of_shelves);
    printf("l:%d, total_number_of_books = %p\n", __LINE__, total_number_of_books);


    while (total_number_of_queries--) {
    printf("l:%d\n", __LINE__);
        int type_of_query;
    printf("l:%d\n", __LINE__);
        scanf("%d", &type_of_query);
    printf("l:%d\n", __LINE__);

        if (type_of_query == 1) {
    printf("l:%d\n", __LINE__);
            /*
             * Process the query of first type here.
             */
            int x, y;
    printf("l:%d\n", __LINE__);
            scanf("%d %d", &x, &y);
    printf("l:%d\n", __LINE__);


#if 0
            bookcount = total_number_of_books[x];
            *(*((int *)total_number_of_pages + x) + bookcount) = y;
            *(total_number_of_books + x) = bookcount + 1;
#else

	     bookcount = total_number_of_books[x];
    printf("l:%d, x = %d, y = %d, bookcount = %d\n", __LINE__, x, y, bookcount);
            total_number_of_pages[x][bookcount] = y;
    printf("l:%d\n", __LINE__);
            total_number_of_books[x] = bookcount+1;
    printf("l:%d\n", __LINE__);
#endif

	}
    }



    printf("l:%d\n", __LINE__);
    if (total_number_of_books) {
    printf("l:%d\n", __LINE__);
        free(total_number_of_books);
    }
    
    printf("l:%d\n", __LINE__);
    for (int i = 0; i < total_number_of_shelves; i++) {
    printf("l:%d\n", __LINE__);
        if (*(total_number_of_pages + i)) {
    printf("l:%d\n", __LINE__);
            free(*(total_number_of_pages + i));
        }
    printf("l:%d\n", __LINE__);
    }
    printf("l:%d\n", __LINE__);
    
    if (total_number_of_pages) {
    printf("l:%d\n", __LINE__);
        free(total_number_of_pages);
    }
    
    printf("l:%d\n", __LINE__);


    return 0;


}


#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "zen_types.h"
#include "zen_server.h"

static int zen_write_socket = 0;
static int init_print_done = 0;

int init_print()
{
    zen_write_socket = zen_get_write_socket();
    return zen_write_socket;
}

int zen_sock_write(char * arr, int len)
{
    int n = 0;

    if (!init_print_done) {
	init_print_done = init_print();
    }

    if (write_sock) {
	n = write(zen_write_socket, arr, len);
    }

    return n;
}

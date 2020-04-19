#include "flowcontext_commons.h"

/*
 * wrapper for 
 * __builtin_frame_address()
 * __builtin_return_address()
 */

void * my__builtin_frame_address(unsigned int level) {
    switch (level) {
#include "builtin_frame_address_gen.h"
    }
    return NULL;
}

void * my__builtin_return_address(unsigned int level) {
    switch (level) {
#include "builtin_return_address_gen.h"
    }
    return NULL;
}


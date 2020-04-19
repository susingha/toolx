
#if 0
https://stackoverflow.com/questions/5863122/how-to-include-c-backtrace-in-a-kernel-module-code
http://baruch.siach.name/blog/posts/linux_kernel_deferred_stack_trace/
https://stackoverflow.com/questions/36692315/what-exactly-does-rdynamic-do-and-when-exactly-is-it-needed
#endif

#include "flowcontext_commons.h"
#include "builtin.h"

#ifdef __KERNEL__
#include <linux/stacktrace.h>
#else
#include <execinfo.h>
#endif

/*
 * in trigger function do setcontext(mycontext())
 * in other function do if (getcontext() == mycontext()) { then print the log }
 * in setcontext() { static context = getcontext() }
 */

void * large_array[LARGE_NUMBER];
void * __my_global_flow_context__ = NULL;


void *
curr_stack_first_frame()
{
#ifdef __KERNEL__
#else
    unsigned int stack_size = backtrace(large_array, LARGE_NUMBER);
#endif
    return my__builtin_frame_address(stack_size-1); // or my__builtin_return_address(stack_size-2);
}

unsigned int
curr_stack_frame_level()
{
#ifdef __KERNEL__
#else
    return backtrace(large_array, LARGE_NUMBER);
#endif

}


// ============= User Facign Functions ==============

void * 
currcontext()
{
    return curr_stack_first_frame();
}

void 
setmycontext(void * mycontext)
{
    __my_global_flow_context__ = mycontext;
}

void 
setmycontext_once(void * mycontext)
{
    if (__my_global_flow_context__)
	return;

    __my_global_flow_context__ = mycontext;
}

void * 
getmycontext()
{
    return __my_global_flow_context__;
}

unsigned int 
getmycontext_level()
{
    return curr_stack_frame_level();
}

void resetmycontext()
{
    __my_global_flow_context__ = NULL;
}

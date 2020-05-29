
#define SPACE ' '

#define _AH_FCON_
#ifdef _AH_FCON_
#include "ah_kdbg.h"
#include "ah_ssysapi.h"
#endif

#include <linux/spinlock_types.h>


static
unsigned int  __fcon_logonce = 1;
unsigned int  __fcon_logdepth = 0;
unsigned char __fcon_caller_func_str[200];
spinlock_t    __fcon_caller_func_spinlock = __SPIN_LOCK_UNLOCKED(__fcon_caller_func_spinlock);

static inline
void __attribute__((no_instrument_function))
printlogActual(char * buf)
{
#ifdef _AH_FCON_
	ah_dbgk(AH_SYS_KMOD_FE, KDBG_FE_BASIC, "%s", buf);
#else
	printk(KERN_INFO                       "%s", buf);
#endif
}

void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    char  __fcon_logbuf[200];

#if 0
    if (__fcon_logdepth) {

	sprintf(__fcon_logbuf, "%ps", call_site);
	spin_lock(&__fcon_caller_func_spinlock);
	if (strcmp(__fcon_caller_func_str, __fcon_logbuf)) {
	    spin_unlock(&__fcon_caller_func_spinlock);
	    return;
	}
	sprintf(__fcon_caller_func_str, "%ps", this_fn);
	spin_unlock(&__fcon_caller_func_spinlock);


	if (__fcon_logonce) {
	    __fcon_logonce--;
	    sprintf(__fcon_logbuf, "En %d %*c %pF\n", __fcon_logdepth, __fcon_logdepth, '>', call_site);
	    printlogActual(__fcon_logbuf);
	}

	__fcon_logdepth++;
	sprintf(__fcon_logbuf, "En %d %*c %pF -- (%p / %p / %p)\n", __fcon_logdepth, __fcon_logdepth, '>', this_fn,
		                                                                     __builtin_frame_address(0),
		                                                                     __builtin_frame_address(1),
		                                                                     __builtin_frame_address(2));
	printlogActual(__fcon_logbuf);
    }
#endif
}
void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
    char  __fcon_logbuf[200];

#if 0
    if (__fcon_logdepth) {

	sprintf(__fcon_logbuf, "%ps", this_fn);
	spin_lock(&__fcon_caller_func_spinlock);
	if (strcmp(__fcon_caller_func_str, __fcon_logbuf)) {
	    spin_unlock(&__fcon_caller_func_spinlock);
	    return;
	}
	sprintf(__fcon_caller_func_str, "%ps", call_site);
	spin_unlock(&__fcon_caller_func_spinlock);



	sprintf(__fcon_logbuf, "Ex %d %*c %pF -- (%p / %p / %p)\n", __fcon_logdepth, __fcon_logdepth, '<', this_fn,
		                                                                     __builtin_frame_address(0),
		                                                                     __builtin_frame_address(1),
		                                                                     __builtin_frame_address(2));
	printlogActual(__fcon_logbuf);

	__fcon_logdepth--;

	if (!__fcon_logdepth) {
	    __fcon_logonce = 1;
	    // dump_stack();
	}
    }
#endif
}







#if 0
#include <linux/stacktrace.h>
#define STACK_DEPTH 100
static unsigned long stackentries[STACK_DEPTH];
static struct stack_trace trace = {
    .nr_entries = page_ext->nr_entries,
    .entries = &stackentries[0],
};
#endif



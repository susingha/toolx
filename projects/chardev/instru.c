
#include <linux/kernel.h>    // included for printk KERN_INFO only

void __attribute__((no_instrument_function))
printfunE(void *this_fn, void *call_site)
{
    printk(KERN_ERR "sup: entering %p - %pF\n", this_fn, this_fn);
}
void __attribute__((no_instrument_function))
printfunX(void *this_fn, void *call_site)
{
    printk(KERN_ERR "sup: exitiing %p\n", this_fn);
}


void __attribute__((no_instrument_function))
__cyg_profile_func_enter (void *this_fn, void *call_site)
{
    printfunE(this_fn, call_site);
}
void __attribute__((no_instrument_function))
__cyg_profile_func_exit  (void *this_fn, void *call_site)
{
    printfunX(this_fn, call_site);
}

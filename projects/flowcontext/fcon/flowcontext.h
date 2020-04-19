#ifndef __FLOWCONTEXT_H__
#define __FLOWCONTEXT_H__

#include "flowcontext_commons.h"

extern void * __my_global_flow_context__;

void *        currcontext();
void          setmycontext(void * mycontext);
void          setmycontext_once(void * mycontext);
void *        getmycontext();
unsigned int  getmycontext_level();
void          resetmycontext();

#define INITFLOWCONTEXT(x) \
{ \
    setmycontext(currcontext()); \
}

#define INITFLOWCONTEXT_ONCE(x) \
{ \
    setmycontext_once(currcontext()); \
}

#define PRINTFLOWCONTEXT(x) \
{ \
    if (getmycontext() == currcontext()) { \
	printf("sup: %*c %s\n", getmycontext_level(), SPACE, __func__); \
    } \
}

#define DEINITFLOWCONTEXT(x) \
{ \
    if (getmycontext() == currcontext()) { \
	resetmycontext(); \
    } \
}

#endif // __FLOWCONTEXT_H__

#ifndef __BUILTIN_H__
#define __BUILTIN_H__

void * my__builtin_frame_address(unsigned int level);
void * my__builtin_return_address(unsigned int level);

#endif // __BUILTIN_H__

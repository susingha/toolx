

include ../rules.global.xq

CC = gcc

OBJECTS_MAIN = \
	       read4k.o \
	       readnb_main.o \

TARGETS = a.out \


a.out : $(OBJECTS_MAIN)
	$(CC) $^ -o $@

.c.o :
	$(CC) -c $< -o $@ $(ADDED_C_FLAGS) $(ADDED_C_INCLUDES) $(ADDED_C_LD) $(ADDED_C_LD_PATHS) 


all: $(TARGETS)

clean : localclean

localclean :
	rm *.o $(TARGETS)

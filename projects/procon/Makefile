

include ../rules.global.xq

CC = gcc
ADDED_C_FLAGS += -g

OBJECTS_COMMON = \
	       sharedmem.o \
	       sh_sync.o

OBJECTS_MAIN_P = \
	       $(OBJECTS_COMMON) \
	       producer.o
OBJECTS_MAIN_C = \
	       $(OBJECTS_COMMON) \
	       consumer.o

TARGETS = a.out \
          b.out \

all: $(TARGETS)

a.out : $(OBJECTS_MAIN_P)
	$(CC) $^ -o $@
b.out : $(OBJECTS_MAIN_C)
	$(CC) $^ -o $@

.c.o :
	$(CC) -c $< -o $@ $(ADDED_C_FLAGS) $(ADDED_C_INCLUDES) $(ADDED_C_LD) $(ADDED_C_LD_PATHS) 


clean : localclean

localclean :
	rm *.o $(TARGETS)

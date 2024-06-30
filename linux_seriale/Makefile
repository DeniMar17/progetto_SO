CC=gcc
CCOPTS=--std=gnu99 -pthread -Wall -D_LIST_DEBUG_ -g
AR=ar

OBJS=usart.o\

HEADERS=usart.h

LIBS=

BINS=usart\

#disastros_test

.phony: clean all


all:	$(LIBS) $(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libusart.a: $(OBJS) 
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)


usart:	usart.c $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^

clean:
	rm -rf *.o *~ $(LIBS) $(BINS)

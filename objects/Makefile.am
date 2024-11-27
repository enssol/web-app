.POSIX:

include ../common-rules.am

OBJS = config_loader.o env_loader.o error_handler.o garbage_collector.o \
       hello.o logger.o main.o validator.o

CLEANFILES = $(OBJS)

all: $(OBJS)

%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

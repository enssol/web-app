.POSIX:

include ../common-rules.am

OBJDIR = $(top_builddir)/objects

OBJS = $(OBJDIR)/config_loader.o $(OBJDIR)/env_loader.o $(OBJDIR)/error_handler.o \
       $(OBJDIR)/garbage_collector.o $(OBJDIR)/hello.o $(OBJDIR)/logger.o \
       $(OBJDIR)/main.o $(OBJDIR)/validator.o

CLEANFILES = $(OBJS)

all: $(OBJS)

$(OBJDIR)/%.o: ../src/%.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS)

# objects/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Use libtool to create static libraries
lib_LTLIBRARIES = libenveng.la libwebapp.la

# Explicitly list source files to avoid wildcard issues
libenveng_la_SOURCES = ../src/config_loader.c ../src/env_loader.c ../src/error_handler.c \
                       ../src/garbage_collector.c ../src/hello.c ../src/logger.c \
                       ../src/main.c ../src/validator.c ../src/parser.y ../src/lexer.l
libenveng_la_LDFLAGS = -static

libwebapp_la_SOURCES = $(libenveng_la_SOURCES)
libwebapp_la_LDFLAGS = -static

# Rules for Bison and Flex
BUILT_SOURCES = ../src/parser.c ../src/lexer.c
CLEANFILES = ../src/parser.c ../src/lexer.c

../src/parser.c ../src/parser.h: ../src/parser.y
    $(AM_V_GEN)bison -d -o ../src/parser.c ../src/parser.y

../src/lexer.c: ../src/lexer.l ../src/parser.h
    $(AM_V_GEN)flex -o ../src/lexer.c ../src/lexer.l

# Compilation rules for each source file
objects/config_loader.o: ../src/config_loader.c
    $(CC) $(CFLAGS) -c ../src/config_loader.c -o objects/config_loader.o

objects/env_loader.o: ../src/env_loader.c
    $(CC) $(CFLAGS) -c ../src/env_loader.c -o objects/env_loader.o

objects/error_handler.o: ../src/error_handler.c
    $(CC) $(CFLAGS) -c ../src/error_handler.c -o objects/error_handler.o

objects/garbage_collector.o: ../src/garbage_collector.c
    $(CC) $(CFLAGS) -c ../src/garbage_collector.c -o objects/garbage_collector.o

objects/hello.o: ../src/hello.c
    $(CC) $(CFLAGS) -c ../src/hello.c -o objects/hello.o

objects/logger.o: ../src/logger.c
    $(CC) $(CFLAGS) -c ../src/logger.c -o objects/logger.o

objects/main.o: ../src/main.c
    $(CC) $(CFLAGS) -c ../src/main.c -o objects/main.o

objects/validator.o: ../src/validator.c
    $(CC) $(CFLAGS) -c ../src/validator.c -o objects/validator.o

clean: clean_common
    rm -f objects/*.o ../src/parser.c ../src/lexer.c

# Add autoindent to the build process
all: generate_tags autoindent generate_metadata process_metadata include_metadata splint cscope

# Add clean_tags to the clean process
clean: clean_tags

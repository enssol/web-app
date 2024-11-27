.POSIX:

include ../common-rules.am

lib_LTLIBRARIES = libwebapp.la
libwebapp_la_SOURCES = config_loader.c env_loader.c error_handler.c \
                       garbage_collector.c hello.c logger.c main.c validator.c

BUILT_SOURCES = parser.c lexer.c
CLEANFILES = $(BUILT_SOURCES)

parser.c: parser.y
	bison -d -o $@ $<

lexer.c: lexer.l
	flex -o $@ $<

# Add this rule to compile config_loader.c
../objects/config_loader.o: config_loader.c
    $(CC) $(CFLAGS) -c config_loader.c -o ../objects/config_loader.o

# src/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the source files for the src directory
lib_LTLIBRARIES = libwebapp.la
libwebapp_la_SOURCES = config_loader.c env_loader.c error_handler.c garbage_collector.c hello.c logger.c main.c validator.c parser.c lexer.c

# Add autoindent to the build process
all: all_common

# Define splint target
.PHONY: splint
splint:
    @echo "Running splint static code analysis..."
    splint $(SRC_DIR)/*.c

# Define cscope target
.PHONY: cscope
cscope:
    @echo "Generating cscope database..."
    cscope -b -R -s $(SRC_DIR)

# Add gettext rules
all-local: generate_mo_files

# Add clean_tags to the clean process
clean: clean_common

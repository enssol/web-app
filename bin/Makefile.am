# bin/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the binary files for the bin directory
bin_PROGRAMS = webapp
webapp_SOURCES = main.c
webapp_LDADD = ../lib/libwebapp.la

# Add autoindent to the build process
all: clib_install perf generate_tags cflow stow autoindent generate_metadata process_metadata include_metadata splint cscope

# Add clean_tags to the clean process
clean: clean_tags

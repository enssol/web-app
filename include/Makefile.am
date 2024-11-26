# include/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Add autoindent to the build process
all: generate_tags autoindent generate_metadata process_metadata include_metadata splint cscope

# Add clean_tags to the clean process
clean: clean_tags

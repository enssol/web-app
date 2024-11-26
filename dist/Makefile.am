# dist/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Add autoindent to the build process
all: generate_tags cflow dist-xz generate_metadata process_metadata include_metadata stow autoindent splint cscope

# Add clean_tags to the clean process
clean: clean_tags

# Define the distribution files for the dist directory
dist_noinst_DATA = README.md LICENSE INSTALL AUTHORS ChangeLog NEWS

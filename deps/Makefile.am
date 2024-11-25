# deps/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Generate the dependency files list
gen_deps_list = $(srcdir)/deps/generate_deps.sh

# Define the dependency files for the deps directory
DEPS_FILES = $(depsdir)/deps_list.txt

# Specify the directory for the dependency files
depsdir = $(srcdir)/deps

# Install the dependency files
dist_noinst_DATA = $(DEPS_FILES)

# Hook to generate the dependency files list before building
all: $(gen_deps_list) clib_install generate_tags cflow
    @$(gen_deps_list)

# Add clean_tags to the clean process
clean: clean_tags

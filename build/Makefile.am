# build/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the auxiliary files for the build directory
EXTRA_DIST = build_script.sh

# Specify the directory for the auxiliary files
builddir = $(srcdir)/build

# Install the auxiliary files
dist_noinst_SCRIPTS = build_script.sh

# Custom rule to process auxiliary files with gawk
process_build:
    @echo "Processing auxiliary files with gawk..."
    @gawk -f ../etc/process_files.gawk build/*.sh

# Add autoindent to the build process
all: process_build autoindent clib_install generate_tags stow generate_metadata process_metadata include_metadata splint cscope

clean:
    rm -f build_script.sh

# Add clean_tags to the clean process
clean: clean_tags

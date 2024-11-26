# tmp/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the temporary files for the tmp directory
dist_noinst_DATA = *.tmp

# Custom rule to process temporary files with gawk
process_tmp:
    @echo "Processing temporary files with gawk..."
    @gawk -f ../etc/process_files.gawk tmp/*.tmp

# Add autoindent to the build process
all: process_tmp generate_tags cflow dist-xz stow autoindent generate_metadata process_metadata include_metadata splint cscope

clean: clean_tags
    rm -f *.tmp

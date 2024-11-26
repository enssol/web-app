# etc/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Add autoindent to the build process
all: generate_tags autoindent generate_metadata process_metadata include_metadata splint cscope

# Add clean_tags to the clean process
clean: clean_tags

# Define the configuration files for the etc directory
configdir = $(sysconfdir)
dist_config_DATA = config.conf config.ini gcc.spec header_sources.txt sources.txt .autoindent process_files.gawk

# logs/Makefile.am

.POSIX: # Enforce POSIX mode for portability

# Include common rules
include ../common-rules.am

# Define the log files for the logs directory
dist_noinst_DATA = *.log

# Custom rule to run gnu-complexity and output results to log files
complexity_logs:
    @echo "Running gnu-complexity on source files and saving logs..."
    @for dir in ../src ../include ../etc; do \
        gnu-complexity $$dir/*.c > logs/complexity_$$(basename $$dir).log; \
    done

# Custom rule to process log files with gawk
process_logs:
    @echo "Processing log files with gawk..."
    @gawk -f ../etc/process_files.gawk logs/*.log

# Add autoindent to the build process
all: complexity_logs process_logs generate_tags autoindent generate_metadata process_metadata include_metadata splint cscope

clean:
    rm -f *.log

.POSIX:

include ../common-rules.am

DEPS_FILES = *.d
dist_noinst_DATA = $(DEPS_FILES)

all: generate-deps

generate-deps:
	@echo "Generating dependency files..."
	./generate_deps.sh

clean:
	rm -f $(DEPS_FILES)

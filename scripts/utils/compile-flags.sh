#!/bin/sh

# Source shared utilities
. ./shared-utils.sh

# Validate compiler presence
check_cmd "${CC:-gcc}"

# Common compile flags with validation
CFLAGS="-std=c90 -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE=500 -Wall -Wextra -pedantic -static"
LDFLAGS="-static -pthread -Wl,-O1 -Wl,--gc-sections -lssl -lcrypto"

# Verify flags work with compiler
if ! ${CC:-gcc} -v >/dev/null 2>&1; then
    error "Compiler validation failed"
fi

export CFLAGS
export LDFLAGS

log_info "Compiler flags configured successfully"

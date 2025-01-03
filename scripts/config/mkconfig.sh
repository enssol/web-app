#!/bin/sh

#######################################
# Generate build configuration
#
# Usage: ./mkconfig.sh
#######################################

# Source common configuration
. ./common-config.sh
. ./shared-config.sh

# Generate config.h and config-test.h
generate_config "config/mkconfig/config.h.in" "config/build/config.h"
cp "config/build/config.h" "config/mkconfig/config-test.h"

printf "Configuration completed successfully\n"

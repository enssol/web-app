# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# include/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the header files for the include directory
include_HEADERS = config_loader.h env_loader.h error_handler.h garbage_collector.h hello.h logger.h parser.h validator.h gettext.h

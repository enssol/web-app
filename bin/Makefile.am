# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# bin/Makefile.am

# Include the .env file if it exists
-include .env
export $(shell cat .env | grep -v '#' | awk '/=/ {print $$1}')

# Define the binary files for the bin directory
bin_PROGRAMS = webapp
webapp_SOURCES = main.c
webapp_LDADD = ../lib/libwebapp.la

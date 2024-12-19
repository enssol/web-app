# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later
CC = gcc
CFLAGS = -static -std=c90 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -Wall -ansi \
		 -Wextra -pedantic -Werror -Wshadow -Wconversion -Wstrict-prototypes \
		 -Wmissing-prototypes -fanalyzer -fstack-protector-strong -fstack-check \
		 -fdata-sections -ffunction-sections -fno-common -fstrict-aliasing \
		 -Warray-bounds -Wstack-protector -Wformat=2 -Wformat-security \
		 -Wformat-overflow=2 -Wformat-truncation=2 -Walloca -Wvla \
		 -fno-omit-frame-pointer

LDFLAGS = -Wl,--gc-sections

TARGET = web_server
TEST_TARGET = test_web_server
TEST_LIBS = -lcunit

all: $(TARGET)

$(TARGET): web_server.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

test: test_web_server.c
	$(CC) -std=c90 -Wall -ansi -Wextra -pedantic -Werror $< -o $(TEST_TARGET) $(TEST_LIBS) && ./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

CC = musl-gcc
CFLAGS = -std=c90 -Wall -Wextra -pedantic -O3 -D_POSIX_C_SOURCE=200809L \
            -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500 \
            -Werror -Wstrict-prototypes -Wmissing-prototypes \
            -fstack-protector-strong -fPIE -fPIC -fomit-frame-pointer -Wmissing-declarations -Wconversion -Wshadow \
            --Wcast-align -Wpointer-arith -Wnested-externs -Wwrite-strings
LDFLAGS = -lssl -lcrypto

SRC = src/main.c src/server.c src/auth.c src/profile.c src/audit.c \
      src/quic.c src/file_io.c src/request_validation.c \
      src/request_processing.c src/ssl.c src/socket.c src/data_structures.c

OBJ = $(SRC:.c=.o)
BIN = app

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)

clean:
	rm -f $(OBJ) $(BIN)

# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# Pull Alpine image
FROM docker.io/alpine:3.18

# Install necessary packages for C development and other tools
RUN apk update && \
    apk add --no-cache \
    build-base \
    musl-dev \
    dropbear \
    dropbear-dbclient \
    dropbear-ssh \
    gpg \
    gpg-agent \
    pinentry \
    tree \
    git \
    cunit-dev

# Add a user for non-root execution
RUN adduser -D appuser -u 1000 -g 1000
USER appuser

# Expose port 8888
EXPOSE 8888

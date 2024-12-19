<!--
 Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
 SPDX-License-Identifier: 	AGPL-3.0-or-later
-->
# Web Server

This is a basic web server implementation in C89, compliant with POSIX standards, serving static HTML files.

## Features
- Serves `index.html` over HTTP.
- Designed for lightweight environments using musl-libc.
- Thorough error handling and static binary production.

## Prerequisites
- musl-libc (`musl-gcc`).
- CUnit (for tests).

## Build Instructions
### Production Binary
```bash
make

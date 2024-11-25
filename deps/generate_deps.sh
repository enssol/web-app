# Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
# SPDX-License-Identifier: 	AGPL-3.0-or-later

# deps/generate_deps.sh
#!/bin/sh
find "$(dirname "$0")" -name '*.d' > "$(dirname "$0")/deps_list.txt"

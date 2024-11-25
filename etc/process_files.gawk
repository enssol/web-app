// Copyright 2024 Enveng Group - Simon French-Bluhm and Adrian Gallo.
// SPDX-License-Identifier: 	AGPL-3.0-or-later

/**
 * File: etc/process_files.gawk
 * ----------------------------
 * Processes source files to count functions, total lines, and comment lines.
 * Outputs the total number of functions, total lines of code, total comment lines,
 * and comment density as a percentage.
 */

BEGIN {
    print "Processing source files with gawk..."
    function_count = 0
    total_lines = 0
    comment_lines = 0
    in_comment = 0
}

# Match function definitions
/^.*\(.*\)\s*{/ {
    function_count++
}

# Count total lines and comment lines
{
    total_lines++
    if (in_comment) {
        comment_lines++
    }
    if ($0 ~ /\/\*/) {
        in_comment = 1
        comment_lines++
    }
    if ($0 ~ /\*\//) {
        in_comment = 0
    }
    if ($0 ~ /^\/\//) {
        comment_lines++
    }
}

END {
    print "Total number of functions:", function_count
    print "Total lines of code:", total_lines
    print "Total comment lines:", comment_lines
    print "Comment density:", comment_lines / total_lines * 100 "%"
}


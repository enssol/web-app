#ifndef CONFIG_H
#define CONFIG_H

/* Language standard and POSIX compliance flags */
#define LANG_FLAGS "-std=c90 -ansi -pedantic" \
                  " -D_POSIX_C_SOURCE=200809L" \
                  " -D_XOPEN_SOURCE=500" \
                  " -D_FILE_OFFSET_BITS=64"

/* Warning and error detection flags */
#define WARN_FLAGS "-Wall -Wextra -Werror" \
                  " -Wshadow -Wconversion" \
                  " -Wstrict-prototypes -Wmissing-prototypes" \
                  " -Warray-bounds -Wformat=2" \
                  " -Wformat-security -Wformat-overflow=2" \
                  " -Wformat-truncation=2 -Wvla" \
                  " -Wstrict-aliasing=2 -Wnull-dereference" \
                  " -Wpointer-arith -Wwrite-strings" \
                  " -Wcast-align -Wcast-qual" \
                  " -Wredundant-decls -Wundef" \
                  " -Wmissing-declarations -Wsign-conversion"

/* Security hardening flags */
#define SEC_FLAGS "-fstack-protector-strong" \
                 " -fstack-check -fPIE" \
                 " -fstack-protector-all" \
                 " -fdata-sections -ffunction-sections" \
                 " -fno-common -fstrict-aliasing" \
                 " -finput-charset=iso-8859-1" \
                 " -fexec-charset=iso-8859-1" \
                 " -fwide-exec-charset=iso-8859-1"

/* Optimization flags */
#define OPT_FLAGS "-O3" \
                 " -fdata-sections" \
                 " -ffunction-sections" \
                 " -fno-strict-overflow" \
                 " -flto=auto"

/* CPU-Specific Optimization Flags */
#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"

/* Cache Optimization Flags */
#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"

/* Network Performance Flags */
#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"

/* Build configuration structure */
struct build_config {
    const char *prod_cflags;   /* Production compiler flags */
    const char *prod_ldflags;  /* Production linker flags */
    const char *test_cflags;   /* Test build compiler flags */
    const char *test_ldflags;  /* Test build linker flags */
};

/* Default build configuration */
#define BUILD_CONFIG_DEFAULT { \
    .prod_cflags = LANG_FLAGS " " WARN_FLAGS " " SEC_FLAGS " " OPT_FLAGS " " ARCH_FLAGS " " CACHE_FLAGS " " NET_FLAGS " -static", \
    .prod_ldflags = "-static -Wl,--gc-sections -Wl,-O3", \
    .test_cflags = LANG_FLAGS " " WARN_FLAGS " " SEC_FLAGS " -O0 -g3", \
    .test_ldflags = "-Wl,--warn-common -Wl,--no-undefined" \
}

#endif /* CONFIG_H */

#ifndef MACHINE_H
#define MACHINE_H

#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"

#endif /* MACHINE_H */#ifndef MACHINE_H
#define MACHINE_H

/* CPU-specific optimization flags */
#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"

/* Cache optimization flags */
#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"

/* Network performance flags */
#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"

#endif /* MACHINE_H */#define ARCH_FLAGS "-march=native -mtune=native -msse4.2 -mavx2 -mfma -mavx512f -mpclmul -maes -msha -mprefer-vector-width=512"
#define CACHE_FLAGS "-falign-functions=64 -falign-jumps=32 -falign-loops=32 -falign-labels=32"
#define NET_FLAGS "-fno-schedule-insns -fno-schedule-insns2 -fomit-frame-pointer -fno-stack-check"

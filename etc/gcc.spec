*cc1:
  std=iso9899:2024: -std=iso9899:2024 \
  D_POSIX_C_SOURCE: -D_POSIX_C_SOURCE=2024L \
  D_XOPEN_SOURCE: -D_XOPEN_SOURCE=800 \
  %{!O3: -O3} \
  %{!fstack-protector: -fstack-protector} \
  %{!march=native: -march=native} \
  %{!mtune=generic: -mtune=generic} \
  %{!fPIC: -fPIC} \
  %{!fno-gnu: -fno-gnu} \
  %{!fno-common: -fno-common} \
  %{!ffunction-sections: -ffunction-sections} \
  %{!fdata-sections: -fdata-sections} \
  %{!flto: -flto} \
  %{!funroll-all-loops: -funroll-all-loops} \
  %{!Wl,--gc-sections: -Wl,--gc-sections} \
  %{!fomit-frame-pointer: -fomit-frame-pointer} \
  %{!Wall: -Wall} \
  %{!Wextra: -Wextra} \
  -pedantic \
  %{!Wconversion: -Wconversion} \
  %{!Wfloat-equal: -Wfloat-equal} \
  %{!fvisibility=hidden: -fvisibility=hidden} \
  %{!DPLATFORM_LINUX: -DPLATFORM_LINUX} \
  %{!fno-strict-aliasing: -fno-strict-aliasing} \
  %{!fdiagnostics-color=always: -fdiagnostics-color=always} \
  -I${INCLUDE_DIR}

*link:
  %{L${LIB_DIR}} \
  -Wl,--gc-sections \
  -static -lmylib -Wl,-Bstatic -lc -lgcc -Wl,-Bdynamic

*as:
  -march=native -mtune=generic

*ar:
  rcs

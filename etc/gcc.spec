*cc1:
  std=gnu23: -std=gnu23 \
  D_POSIX_C_SOURCE: -D_POSIX_C_SOURCE=2024L \
  D_XOPEN_SOURCE: -D_XOPEN_SOURCE=800 \
  %{!O0: -O0} \
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
  -I${INCLUDE_DIR} \
  -I/usr/local/include/glib-2.0 \
  -I/usr/local/lib/glib-2.0/include \
  -I/opt/homebrew/include/glib-2.0 \
  -I/opt/homebrew/lib/glib-2.0/include \
  -pg -fprofile-arcs -ftest-coverage

*link:
  %{L${LIB_DIR}} \
  -L/usr/local/lib \
  -L/opt/homebrew/lib \
  -Wl,--gc-sections \
  "-fuse-ld=mold" \
  -static -lmylib -Wl,-Bstatic -lc -lgcc -Wl,-Bdynamic \
  -pg -lgcov

*as:
  -march=native -mtune=generic

*ar:
  rcs

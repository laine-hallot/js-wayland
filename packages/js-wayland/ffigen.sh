set -e

castxml --castxml-gccxml --castxml-cc-gnu-c clang $(pkg-config --cflags wayland-client) -I./protocols -o - ./src/js_externs.c | ./fix-castxml-out.ts > bindgen-out/js_externs.xml 


WAYLAND_INCLUDE_PATH=$(pkg-config --cflags wayland-client | sed -e 's#^-I##; s#$##')
npx ffigen js ./bindgen-out/js_externs.xml  "./src/js_externs.h,$WAYLAND_INCLUDE_PATH/wayland-client.h,wayland-client-core.h,$WAYLAND_INCLUDE_PATH/wayland-client-protocol.h,protocols/xdg-shell-client-protocol.h" > ./bindgen-out/js_externs.js


npx ffigen cwrap ./bindgen-out/js_externs.xml "./src/js_externs.h,$WAYLAND_INCLUDE_PATH/wayland-client.h,wayland-client-core.h,$WAYLAND_INCLUDE_PATH/wayland-client-protocol.h,protocols/xdg-shell-client-protocol.h" > ./bindgen-out/tmp-js_externs_cwrap.c

cat << "EOF" - ./bindgen-out/tmp-js_externs_cwrap.c > ./bindgen-out/js_externs_cwrap.c
#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <limits.h>
#include <syscall.h>
#include <sys/mman.h>
#include <errno.h>
#include "xdg-shell-client-protocol.h"
EOF

rm ./bindgen-out/tmp-js_externs_cwrap.c

set -e

mkdir -p ./dist/c
clang -c -o dist/c/js_externs_cwrap.o bindgen-out/js_externs_cwrap.c -Iprotocols -fPIC
clang -c -o dist/c/js_externs.o src/js_externs.c -Iprotocols -fPIC
clang -c -o dist/c/xdg-shell-client-protocol.o protocols/xdg-shell-client-protocol.c -fPIC
clang -c -o dist/c/ffi_helpers.o shermes_helpers/ffi_helpers.h -fPIC
ar r dist/c/libjswayland.a dist/c/js_externs_cwrap.o dist/c/js_externs.o dist/c/xdg-shell-client-protocol.o

# npx shermes -typed -c --exported-unit=libjswayland bindgen-out/js_externs.js src/helper.js \
# -Xes6-block-scoping -Xline-directives \
# -o dist/c/libjswayland.o \
# -l dist/c/js_externs_cwrap.a

#ar r dist/c/libjswayland.a dist/c/libjswayland.o dist/c/js_externs_cwrap.o

npx shermes shermes_helpers/ffi_helpers.js shermes_helpers/asciiz.js bindgen-out/js_externs.js run.ts \
-typed  \
-Xes6-block-scoping \
-g \
-o dist/run.o \
-Ldist/c \
-ljswayland \
-lwayland-client \
-Wc,-Ishermes_helpers


#rm dist/c/libjswayland.o dist/c/js_externs_cwrap.o dist/c/hello-world.o dist/c/libjswayland-temp.a
#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE=${BUILD_TYPE:-Release}  

HERMES_SRC=$(realpath ${HERMES_SRC:-../../external/hermes})
HERMES_BUILD=$(realpath ${HERMES_BUILD:-../../external/hermes/build})
ROOT_DIR=$(dirname "$0")
SRC_DIR="${ROOT_DIR}/src"
INC_DIR="${SRC_DIR}"
OBJ_DIR="${ROOT_DIR}/dist/${BUILD_TYPE}"
SRC_FILE="${SRC_DIR}/runtime.cpp"

mkdir -p "${OBJ_DIR}"
clang++-18 \
"${SRC_DIR}/MappedFileBuffer.cpp" \
-c \
-o "${OBJ_DIR}/libmappedfilebuffer.o" \
-I"${HERMES_SRC}/include" \
-I"${HERMES_SRC}/API" \
-I"${HERMES_SRC}/lib" \
-I"${HERMES_SRC}/public" \
-I"${HERMES_SRC}/lib/VM" \
-I"${HERMES_SRC}/API/jsi" \
-I"${HERMES_BUILD}/lib/config"

ar r "${OBJ_DIR}/libmappedfilebuffer.a" "${OBJ_DIR}/libmappedfilebuffer.o"
rm "${OBJ_DIR}/libmappedfilebuffer.o"

echo "=== Building ${BUILD_TYPE} version of static-hermes-program ==="

obj="${OBJ_DIR}/$(basename "$SRC_FILE" .cpp).a"
echo "Compiling $SRC_FILE → ${obj}"
clang++-18 \
"$SRC_FILE" \
-o "$obj" \
-I"${HERMES_SRC}/include" \
-I"${HERMES_SRC}/API" \
-I"${HERMES_SRC}/lib" \
-I"${SRC_DIR}" \
-I"${HERMES_SRC}/public" \
-I"${HERMES_SRC}/lib/VM" \
-I"${HERMES_SRC}/API/jsi" \
-I"${HERMES_BUILD}/lib/config" \
-L"${HERMES_BUILD}/lib" \
-L"${HERMES_BUILD}/jsi" \
-L"${OBJ_DIR}" \
-L"$(realpath ../js-wayland/dist/c)" \
-l hermesvm \
-l jsi \
-l jswayland \
-l mappedfilebuffer \
-rpath "${HERMES_BUILD}/lib"

echo "=== Build complete ==="

node esbuild.config.js --outdir "${OBJ_DIR}"
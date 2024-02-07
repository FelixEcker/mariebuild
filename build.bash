#!/bin/bash

CC="clang"
SRCDIR="src/"
OBJDIR="obj/"

CFLAGS="-ggdb -Iinclude/ -Isrc/ -DDEFAULT_LOG_LEVEL=LOG_DEBUG"
LDFLAGS="-lm -Llib/ -lmcfg_2"

BIN_NAME="mb"

function build_objs() {
  COMPILED_OBJECTS=()

  for i in $1
  do
    OUTNAME="$OBJDIR$i.o"
    INNAME="$SRCDIR$i.c"

    echo "  CC $INNAME"

    $CC $CFLAGS -c -o $OUTNAME $INNAME || exit

    COMPILED_OBJECTS+=("${OUTNAME}")
  done
}

function build() {
  OBJECTS=("xmem strlist logging types executor c_rule target build main")

  echo "==> Compiling Sources for \"$BIN_NAME\""
  build_objs "${OBJECTS[@]}"
  
  echo "==> Linking \"$BIN_NAME\""
  echo "  LD -o $BIN_NAME ${COMPILED_OBJECTS[@]} $LDFLAGS"
  $CC $CFLAGS -o $BIN_NAME ${COMPILED_OBJECTS[@]} $LDFLAGS
}

echo "MB build script. "

build
echo "==> Finished build!"

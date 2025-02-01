#!/usr/bin/env sh
set -e
set -x

export CC="clang"
export CXX="clang++"
export AR="llvm-ar"
export RANLIB="llvm-ranlib"
export TSIL="ціль"

$TSIL розширення_мавки_sqlite.ll скомпілювати розширення_мавки_sqlite.ц

if [ ! -f sqlite/sqlite3.o ]; then
  $CC -shared -fPIC \
     -march=native \
     -o sqlite/sqlite3.o \
     sqlite/sqlite3.c
fi

$CXX -shared -fPIC \
  -march=native \
  -std=c++20 \
  -flto \
  -o розширення_мавки_sqlite.so \
  розширення_мавки_sqlite.ll sqlite_mavka_extension.cpp sqlite/sqlite3.o
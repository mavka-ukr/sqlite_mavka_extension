#!/usr/bin/env sh
set -e
set -x

export CC="clang"
export CXX="clang++"
export AR="llvm-ar"
export RANLIB="llvm-ranlib"
export TSIL="ціль"

$TSIL розширення_мавки_sqlite3.ll скомпілювати розширення_мавки_sqlite3.ц

if [ ! -f sqlite3/sqlite3.o ]; then
  $CC -c -fPIC \
     -march=native \
     -o sqlite3/sqlite3.o \
     sqlite3/sqlite3.c
fi

$CXX -shared -fPIC \
  -march=native \
  -std=c++20 \
  -flto \
  -o розширення_мавки_sqlite3.so \
  розширення_мавки_sqlite3.ll sqlite3_mavka_extension.cpp sqlite3/sqlite3.o
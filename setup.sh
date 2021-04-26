#!/bin/bash 

set -v

source ~/spack/share/spack/setup-env.sh
spack install mochi-sdskv^leveldb@1.22 mochi-ssg+mpi mochi-ch-placement pkg-config
source <(spack module tcl loads mochi-sdskv mochi-ssg mochi-ch-placement autoconf automake libtool pkg-config)
autoreconf --force --install && ./configure CXX=mpic++ && make

mpirun -n 2 tests/trivial

# SDS Distributed Key-Value
sdsdkv is a configurable distributed key-value service.

## Building
sdsdkv depends on several pieces of software infrastructure, so it is convenient
to build these dependencies using spack. If spack is already configured in your
environment, then please skip to **Install dependencies**. Please note that a
bash-like environment is assumed herein.

### Get spack.

```
git clone https://github.com/spack/spack.git
```

### Configure spack and build environment.

```
source spack/share/spack/setup-env.sh
# Setup spack for modules support.
spack bootstrap
```

### Install dependencies.

```
# Checkout sds-repo
git clone https://github.com/mochi-hpc/mochi-spack-packages.git
# Add to spack.
spack repo add mochi-spack-packages
# Install via spack.
#spack install --dirty mochi-sdskv+leveldb
#spack install --dirty mochi-ssg+mpi
#spack install --dirty mochi-ch-placement


# Be sure and have libfabric build the proper network layers. For the trivial test,
# the tcp and rxm network layers are required. To build these, add the variants to
# the spack packages in ~/.spack/packages.yaml or ~/.spack/linux/packages.yaml
#   libfabric:
#     variants: fabrics=tcp,rxm

spack install mochi-sdskv^leveldb@1.22 mochi-ssg+mpi mochi-ch-placement pkg-config
# Refresh spack environment.
source spack/share/spack/setup-env.sh
# Load required modules.
#spack load -r autoconf
#spack load -r automake
#spack load -r libtool
source <(spack module tcl loads mochi-sdskv mochi-ssg mochi-ch-placement mochi-margo mercury argobots json-c mochi-abt-io leveldb berkeley-db autoconf automake libtool pkg-config)
```

### Get and build sdsdkv.

```
git clone https://github.com/mochi-hpc/mochi-sdsdkv.git
cd mochi-sdsdkv && autoreconf --force --install && ./configure CXX=mpic++ && make
```

### Test Distributed key value build

```
mpirun -n 2 tests/trivial
```

Some of the possible errors and their solutions:

configure.ac:53: error: possibly undefined macro: AC\_MSG\_ERROR
   This is caused by not having pkg-config

sdsdkv-config.h:17:10: fatal error: sdskv-common.h: No such file or directory
   17 | #include "sdskv-common.h"

   This is from a change in behavior of the PKG\_CHECK\_MODULES macro in configure.ac. A fix has been added
   to append the the paths to the CXX\_FLAGS and LIBS variables

sdsdkv-misci.h:29:15: error: ‘SDSKV\_ERR\_MERCURY’ was not declared in this scope; did you mean ‘SDSKV\_ERR\_ERASE’?
   29 |         case (SDSKV\_ERR\_MERCURY):     return SDSDKV\_ERR\_SERVICE;

   The error codes have changed in SDSKV. Check the versions of SDSKV and SDSDKV.

[error] Could not initialize hg\_class while running the trivial test

    Check that you are building libfabric with tcp and rxm support

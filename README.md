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
git clone https://xgitlab.cels.anl.gov/sds/sds-repo.git
# Add to spack.
spack repo add sds-repo
# Install via spack.
spack install sdskeyval
# Refresh spack environment.
source spack/share/spack/setup-env.sh
# Load sdskeyval modules.
source <(spack module loads --dependencies sdskeyval)
```

### Get and build sdsdkv.

```
git clone https://xgitlab.cels.anl.gov/sds/sdsdkv.git
cd sdsdkv && ./autogen && ./configure CXX=mpic++ && make
```

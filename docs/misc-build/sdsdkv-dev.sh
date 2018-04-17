# Source to get requisite packages loaded by spack.

. $HOME/src/spack/share/spack/setup-env.sh

spack load leveldb@1.20
spack load berkeley-db@6.2.32
spack load abtsnoozer@master
spack load ch-placement@master
spack load ssg@master
spack load argobots@dev-get-dev-basic
spack load mercury@master
spack load margo@master
spack load sdskeyval@master
spack load boost@1.66.0
spack load mpich@3.2.1

module list

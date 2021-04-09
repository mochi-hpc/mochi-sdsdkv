#!/bin/sh

#spack repo rm sds
#spack mirror rm local_filesystem
spack module tcl rm -y
rm -f ${SPACK_ROOT}/share/spack/dotkit/linux-centos7-x86_64/*.dk
spack uninstall --all -y
spack clean --all
#rm -f modules
#rm -f ${HOME}/.spack/linux/packages.yaml
#rm -f ${HOME}/.spack/linux/compilers.yaml

#cd sds-repo
#git checkout packages/sdskeyval/package.py
#cd ..

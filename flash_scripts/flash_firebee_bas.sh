#!/bin/bash

pushd ../../BaS_gcc
make clean all -j
popd
time bdmctrl flash_firebee_bas.bdm /dev/bdmcf3

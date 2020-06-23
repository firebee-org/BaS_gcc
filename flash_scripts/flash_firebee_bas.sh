#!/bin/bash

pushd ../../BaS_gcc
make clean all
popd
time bdmctrl flash_firebee_bas.bdm /dev/bdmcf3

#!/bin/bash

pushd ../../BaS_gcc
make clean all
popd
time bdmctrl flash_m548x_bas.bdm /dev/tblcf4


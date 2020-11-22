############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project Max_pool
set_top max_pool
add_files max_pool.cpp
add_files params.h
add_files -tb tb.cpp
open_solution "solution1"
set_part {xczu9eg-ffvb1156-2-e}
create_clock -period 10 -name default
csim_design -clean
csynth_design
cosim_design -trace_level all -tool xsim
export_design -format ip_catalog

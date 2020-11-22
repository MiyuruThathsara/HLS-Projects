############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project Convolution
set_top convolution
add_files Convolution/convolution.cpp
add_files Convolution/params.h
add_files -tb Convolution/tb.cpp
open_solution "solution1"
set_part {xczu9eg-ffvb1156-2-e}
create_clock -period 10 -name default
#source "./Convolution/solution1/directives.tcl"
csim_design -clean
csynth_design
cosim_design -trace_level all -tool xsim
export_design -format ip_catalog

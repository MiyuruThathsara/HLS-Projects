# HLS-Projects
This repo contains HLS projects only

1) 2D convolution of a single channel image, is added, Before input another size image you have to change FEATURE_MAP_HEIGHT and FEATURE_MAP_WIDTH macros in "params.h" file. If you want to change the filter change the you can change it by changing weights variable in "convolution.cpp" script. Please make sure to change KERNEL_WIDTH and KERNEL_HEIGHT macros change as of the shape of your new kernel
2) Maxpool with 2 x 2 neighbour with stride 2 is implemented and results are available within this repo. Expecting to design a general architecture for different neighbourhoods and different strides.

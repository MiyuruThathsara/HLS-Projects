#include "hls_stream.h"
#include "hls_video.h"
#include "ap_int.h"
#include <iostream>

#define KERNEL_DEPTH 3
#define KERNEL_HEIGHT 3
#define KERNEL_WIDTH 3

#define FEATURE_MAP_HEIGHT 375
#define FEATURE_MAP_WIDTH 500

#define NUM_OF_SHIFTS_LINE_BUFFER FEATURE_MAP_HEIGHT * FEATURE_MAP_WIDTH + (FEATURE_MAP_WIDTH - KERNEL_WIDTH)
#define INITIAL_CONV_VALUE_OUT_COUNT FEATURE_MAP_WIDTH * KERNEL_HEIGHT

using namespace hls;
using namespace std;

typedef float data_t;

typedef struct{
	data_t data;
//	ap_uint<1> valid;
	ap_uint<1> last;
} channel_data;

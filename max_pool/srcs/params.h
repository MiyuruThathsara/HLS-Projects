#include "ap_int.h"
#include "hls_stream.h"
#include "hls_video.h"
#include <iostream>

#define KERNEL_SHAPE 2
#define FEATURE_MAP_HEIGHT 224
#define FEATURE_MAP_WIDTH 224

#define OUT_FEATURE_MAP_HEIGHT FEATURE_MAP_HEIGHT / KERNEL_SHAPE
#define OUT_FEATURE_MAP_WIDTH FEATURE_MAP_WIDTH / KERNEL_SHAPE

#define LINE_BUFFER_WIDTH FEATURE_MAP_WIDTH / KERNEL_SHAPE

using namespace hls;
using namespace std;

typedef float data_t;

typedef struct{
	data_t data;
	ap_uint<1> last;
} ch_data;

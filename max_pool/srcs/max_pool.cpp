#include "params.h"

void max_pool(
		stream<ch_data> &in_stream,
		stream<ch_data> &out_stream){

#pragma HLS interface axis register both port=in_stream
#pragma HLS interface axis register both port=out_stream

	ap_uint<1> hor_max_valid = 0;
	ap_uint<1> max_stream_flag = 0;
	ch_data in_ch;
	ch_data out_ch;
	ch_data prev_in_ch;
	data_t in_ch_data;
	data_t prev_in_ch_data;
	data_t hor_max;
	data_t ver_max;
	data_t odd_pixel_counter = 0;
	data_t even_pixel_counter = 0;
	data_t buffer_out_val;

	LineBuffer<LINE_BUFFER_WIDTH, 1, data_t> buffer;

	for(unsigned int f = 0; f < FEATURE_MAP_HEIGHT * FEATURE_MAP_WIDTH; f++){
//		ODD and EVEN rows selector
		while(in_stream.empty());
		if(odd_pixel_counter < FEATURE_MAP_WIDTH){
			odd_pixel_counter += 1;
			max_stream_flag = 0;
		}
		else{
			if(even_pixel_counter < FEATURE_MAP_WIDTH){
				even_pixel_counter += 1;
				max_stream_flag = 1;
			}
			if(even_pixel_counter >= FEATURE_MAP_WIDTH){
				odd_pixel_counter = 0;
				even_pixel_counter = 0;
			}
		}

		if(hor_max_valid == 0){
			prev_in_ch = in_stream.read();
			prev_in_ch_data = prev_in_ch.data;
			hor_max_valid = 1;
		}
		else{
			for(unsigned int i = 0; i < KERNEL_SHAPE - 1; i++){
//			    Assuming that FEATURE_MAP_HEIGHT and FEATURE_MAP_WIDTH are divisible by any value of KERNEL_SIZE
				in_ch = in_stream.read();
				in_ch_data = in_ch.data;
				if(prev_in_ch_data < in_ch_data){
					hor_max = in_ch_data;
				}
				else{
					hor_max = prev_in_ch_data;
				}
			}
			hor_max_valid = 0;
			if(max_stream_flag == 0){
				buffer.shift_pixels_up(0);
				buffer.insert_bottom_row(hor_max, 0);
			}
			else{
				buffer_out_val = buffer.getval(0,0);
				if(hor_max > buffer_out_val){
					ver_max = hor_max;
				}
				else{
					ver_max = buffer_out_val;
				}
				out_ch.data = ver_max;
				out_stream.write(out_ch);
				buffer.shift_pixels_up(0);
			}
		}
	}
}

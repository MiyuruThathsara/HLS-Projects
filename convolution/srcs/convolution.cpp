#include "params.h"

void convolution(
		stream<channel_data> &img_stream_in,
		stream<channel_data> &conv_stream_out){
#pragma HLS interface axis register both port=img_stream_in
#pragma HLS interface axis register both port=conv_stream_out

	data_t weights[KERNEL_HEIGHT][KERNEL_WIDTH] = {{-1.0,-2.0,-1.0}, {0.0, 0.0, 0.0}, {1.0,2.0,1.0}};
#pragma HLS array_partition variable=weights complete dim=0

	LineBuffer<FEATURE_MAP_WIDTH, KERNEL_HEIGHT, data_t> line_buffer;
	Window<KERNEL_WIDTH, KERNEL_HEIGHT, data_t> window_buffer;
	data_t window_buff_new_col[KERNEL_HEIGHT];
	data_t accumilator;
	channel_data conv_out;
	ap_uint<1> conv_calc_decision = 1;
	ap_uint<16> conv_op_row_counter = 0;
	ap_uint<16> conv_op_row_idle_counter = 0;

	for(unsigned int l = 1; l <= NUM_OF_SHIFTS_LINE_BUFFER; l++){
		for(unsigned int kh = 0; kh < KERNEL_HEIGHT; kh++){
			line_buffer.shift_pixels_up(kh);
			if(kh < KERNEL_HEIGHT - 1){
				line_buffer.insert_bottom_row(line_buffer.getval(0, kh + 1), kh);
			}
			else{
				if(l < FEATURE_MAP_HEIGHT * FEATURE_MAP_WIDTH){
					while(img_stream_in.empty());
					channel_data pixel_stream = img_stream_in.read();
					data_t pixel_val = pixel_stream.data;
					line_buffer.insert_bottom_row(pixel_val, kh);
				}
			}
		}

		window_buffer.shift_pixels_up();
		for(unsigned int kh = 0; kh < KERNEL_HEIGHT; kh++){
			window_buff_new_col[kh] = line_buffer.getval(KERNEL_HEIGHT - 1, kh);
		}
		window_buffer.insert_bottom_row(window_buff_new_col);

		if(l >= INITIAL_CONV_VALUE_OUT_COUNT){
			if(conv_calc_decision == 1){
				conv_op_row_counter += 1;
				accumilator = 0;
				for(unsigned int kh = 0; kh < KERNEL_HEIGHT; kh++){
					for(unsigned int kw = 0; kw < KERNEL_WIDTH; kw++){
						accumilator += weights[kh][kw] * window_buffer.getval(kw, kh);
					}
				}
				conv_out.data = accumilator / 1.0;
				if(l == NUM_OF_SHIFTS_LINE_BUFFER - 1){
					conv_out.last = 1;
				}
				else{
					conv_out.last = 0;
				}
				conv_stream_out.write(conv_out);
				if(conv_op_row_counter >= FEATURE_MAP_WIDTH - KERNEL_WIDTH + 1){
					conv_calc_decision = 0;
					conv_op_row_counter = 0;
				}
			}
			else{
				conv_op_row_idle_counter += 1;
				if(conv_op_row_idle_counter >= KERNEL_WIDTH - 1){
					conv_op_row_idle_counter = 0;
					conv_calc_decision = 1;
				}
			}

		}
	}
}

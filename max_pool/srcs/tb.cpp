#include "params.h"
#include <opencv2\opencv.hpp>
#include "hls_opencv.h"

using namespace cv;

void max_pool(
		stream<ch_data> &in_stream,
		stream<ch_data> &out_stream);

int main(){

	cv::Mat img = cv::imread("D:/Vivado/Vivado_Projects/HLS_Projects/Max_pool/Lena.jpg");
	cv::cvtColor(img, img, CV_BGR2GRAY);
	cv::imshow("Image", img);
	cv::waitKey(0);

	stream<ch_data> in_stream("In_Stream");
	stream<ch_data> out_stream("Out_Stream");
	ch_data in_stream_ch;
	ch_data out_stream_ch;
	data_t in_stream_data;
	data_t out_stream_data;

	for(int h = 0; h < FEATURE_MAP_HEIGHT; h++){
		for(int w = 0; w < FEATURE_MAP_WIDTH; w++){
			in_stream_data = (data_t)img.at<uchar>(h,w);
			in_stream_ch.data = in_stream_data;
			in_stream.write(in_stream_ch);
		}
	}

	max_pool(in_stream, out_stream);

	cv::Mat max_pool_img(OUT_FEATURE_MAP_HEIGHT, OUT_FEATURE_MAP_WIDTH, CV_32FC1);
	cv::Mat max_pool_img_char(OUT_FEATURE_MAP_HEIGHT, OUT_FEATURE_MAP_WIDTH, CV_8UC1);
	for(int ho = 0; ho < OUT_FEATURE_MAP_HEIGHT; ho++){
		for(int wo = 0; wo < OUT_FEATURE_MAP_WIDTH; wo++){
			while(out_stream.empty());
			out_stream_ch = out_stream.read();
			out_stream_data = out_stream_ch.data;
			max_pool_img.at<data_t>(ho,wo) = out_stream_data;
		}
	}

	double max;
	double min;
	cv::minMaxLoc(max_pool_img, &min, &max);
	cv::convertScaleAbs(max_pool_img, max_pool_img_char, 255/max);
	if(max_pool_img.empty()){
			cout << "Could not open an Conv Image!!!!!!" << endl;
			return -1;
		}
		else{
			cv::imshow("Max Pooled Image",max_pool_img_char);
			cv::waitKey(0);
			cv::imwrite("Max_Pool_img.jpg", max_pool_img_char);
		}

	return 0;
}

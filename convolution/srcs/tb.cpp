#include "params.h"
#include "hls_opencv.h"
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;

void convolution(
		stream<channel_data> &img_stream_in,
		stream<channel_data> &conv_stream_out);

int main(){
	cv::Mat img = cv::imread("D:/Vivado/Vivado_Projects/HLS_Projects/Convolution/ILSVRC2012_val_00009346.jpeg");
	cv::Mat conv_img(FEATURE_MAP_HEIGHT - KERNEL_HEIGHT + 1, FEATURE_MAP_WIDTH - KERNEL_WIDTH + 1, CV_32FC1);
	cv::Mat conv_img_char(FEATURE_MAP_HEIGHT - KERNEL_HEIGHT + 1, FEATURE_MAP_WIDTH - KERNEL_WIDTH + 1, CV_8UC1);
	cv::cvtColor(img, img, CV_BGR2GRAY);
//	if(img.empty()){
//		cout << "Could not open an Image!!!!!!" << endl;
//		return -1;
//	}
//	else{
//		cv::imshow("Image",img);
//		cv::waitKey(0);
//	}

//	cv::Mat soft_conv_img(FEATURE_MAP_HEIGHT, FEATURE_MAP_WIDTH, CV_32F);
//	data_t weights[KERNEL_HEIGHT][KERNEL_WIDTH] = {{-1.0,-2.0,-1.0}, {0.0, 0.0, 0.0}, {1.0,2.0,1.0}};
//	cv::Mat weights_mat(KERNEL_HEIGHT, KERNEL_WIDTH, CV_32F, weights);
//	cv::filter2D(img, soft_conv_img, -1, weights_mat, cv::Point(-1,-1), 0);
//	cv::imshow("Software Convoluted Image", soft_conv_img);
//	cv::waitKey(0);

	data_t in_pixel;
	data_t out_pixel;
	channel_data in_pixel_type;
	channel_data out_pixel_type;

	stream<channel_data> in_stream("In_Stream");
	stream<channel_data> out_stream("Out_Stream");

//	cout << img.size().height << endl;
//	cout << img.size().width << endl;

	for(int i1 = 0; i1 < img.size().height; i1++){
		for(int i2 = 0; i2 < img.size().width; i2++){
			in_pixel = (data_t)img.at<uchar>(i1,i2);
			in_pixel_type.data = in_pixel;
			in_stream.write(in_pixel_type);
		}
	}

	convolution(in_stream, out_stream);

	for(int j1 = 0; j1 < FEATURE_MAP_HEIGHT - KERNEL_HEIGHT + 1; j1++){
		for(int j2 = 0; j2 < FEATURE_MAP_WIDTH - KERNEL_WIDTH + 1; j2++){
			while(out_stream.empty());
			out_pixel_type = out_stream.read();
			out_pixel = out_pixel_type.data;
			conv_img.at<data_t>(j1, j2) = out_pixel;
		}
	}

	double max;
	double min;
	cv::minMaxLoc(conv_img, &min, &max);
	cv::convertScaleAbs(conv_img, conv_img_char, 255/max);
	if(conv_img.empty()){
			cout << "Could not open an Conv Image!!!!!!" << endl;
			return -1;
		}
		else{
//			cv::imshow("Convolved Image",conv_img_char);
//			cv::waitKey(0);
			cv::imwrite("Conv_img.jpg", conv_img_char);
		}
	return 0;
}

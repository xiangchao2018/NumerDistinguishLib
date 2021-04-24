#include<iostream>
#include<opencv.hpp>
#include<stdint.h>
#include<tesseract\baseapi.h>
#include <leptonica/allheaders.h>
#include "interface_fore.h"



using namespace std;
using namespace cv;
using namespace tesseract;

int main()
{
	//VideoCapture capture;
	//Mat frame;
	//frame = capture.open("demo1.mp4"); //读取gif文件
	//if (!capture.isOpened())
	//{
	//	printf("can not open ...\n");
	//	return -1;
	//}
	//std::vector<cv::Mat> frames;  //存放gif的所有帧，每个frame都是Mat格式
	//while (capture.read(frame))
	//{
	//	std::cout << frame.size() << std::endl;   //打印每一帧的尺寸
	//	
	//	OCRDetect ocrdetect;
	//	vector<Rect> vec;
	//	vector<std::string>vecStrNumber;
	//	ocrdetect.DetectAllImg(frame, vec, vecStrNumber);
	//	for (int ii = 0; ii < vec.size(); ii++)
	//	{
	//		rectangle(frame, vec[ii], Scalar(0, 0, 255), 1);
	//		int fontFace = FONT_HERSHEY_PLAIN;
	//		double fontScale = 2;
	//		int thickness = 2;
	//		putText(frame, vecStrNumber[ii],Point(vec[ii].x, vec[ii].y), fontFace, fontScale, Scalar(0, 0, 255), thickness, 2);
	//	}
	//	imshow("dst",frame);
	//	waitKey(20);
	//	frames.push_back(frame);
	//}
	//capture.release();

	Mat frame = imread("test0419.jpg",1);
	OCRDetect ocrdetect;
	vector<Rect> vec;
	vector<std::string>vecStrNumber;
	ocrdetect.DetectAllImg(frame, vec, vecStrNumber);

	TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	for (int ii = 0; ii < vec.size(); ii++)
	{
		rectangle(frame, vec[ii], Scalar(0, 0, 255), 1);
		int fontFace = FONT_HERSHEY_PLAIN;
		double fontScale = 1;
		int thickness = 2;
		putText(frame, vecStrNumber[ii],Point(vec[ii].x, vec[ii].y), fontFace, fontScale, Scalar(0, 255, 0), thickness, 2);
	}
	imwrite("after_adjust_rect.bmp", frame);
	imshow("dst",frame);
	
	waitKey(0);
	return 0;
}
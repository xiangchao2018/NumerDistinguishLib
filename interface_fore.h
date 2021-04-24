#pragma once
/*
生成dll的工程时，vs默认定义宏:DLL_EXPORTS，不是dll工程，没有该宏定义
可以在"属性->预处理器->预处理器定义"里面看见该宏
以下语句的含义：如果定义该宏，则DLL_API是导出.(dll生成工程定义了该宏，所以是导出)
如果未定义该宏，则DLL_API是导入.(调用dll的工程没有定义该宏，所以是导入dll)
*/
#ifndef CREATDLL_EXPORTS
#define DLL_API __declspec(dllexport)  
#else
#define DLL_API __declspec(dllimport)  
#endif

#include<stdint.h>
#include<opencv.hpp>
#include<tesseract\baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;
using namespace cv;

class OCRDetect {
public:
	DLL_API OCRDetect();
	DLL_API ~OCRDetect();
private:
	void ChangeGreenToWhite(Mat &src);
	//调整其最小外接矩形至最优位置
	Rect AdjustCorrectArea(Rect rect);
	std::string OCRDiscriminate(Mat pData);

public:
	DLL_API void DetectAllImg(Mat pData,  vector<Rect> &vec, vector<std::string> &vecStrName);
private:
	Mat m_src;
	int x1;   //有效位置的
	int x2;
	int y1;
	int y2;
};

#pragma once
/*
����dll�Ĺ���ʱ��vsĬ�϶����:DLL_EXPORTS������dll���̣�û�иú궨��
������"����->Ԥ������->Ԥ����������"���濴���ú�
�������ĺ��壺�������ú꣬��DLL_API�ǵ���.(dll���ɹ��̶����˸ú꣬�����ǵ���)
���δ����ú꣬��DLL_API�ǵ���.(����dll�Ĺ���û�ж���ú꣬�����ǵ���dll)
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
	//��������С��Ӿ���������λ��
	Rect AdjustCorrectArea(Rect rect);
	std::string OCRDiscriminate(Mat pData);

public:
	DLL_API void DetectAllImg(Mat pData,  vector<Rect> &vec, vector<std::string> &vecStrName);
private:
	Mat m_src;
	int x1;   //��Чλ�õ�
	int x2;
	int y1;
	int y2;
};

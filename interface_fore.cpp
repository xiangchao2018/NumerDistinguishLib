#include "interface_fore.h"

DLL_API OCRDetect::OCRDetect()
{
}

DLL_API OCRDetect::~OCRDetect()
{
}

void OCRDetect::ChangeGreenToWhite(Mat &src)
{
	//遍历所有像素将绿色变成白色
	for (int ii=0;ii<src.rows;ii++)
	{
		for(int jj=0;jj<src.cols;jj++)
		{
			uint16_t b = src.at<Vec3b>(ii, jj)[0];
			uint16_t g = src.at<Vec3b>(ii, jj)[1];
			uint16_t r = src.at<Vec3b>(ii, jj)[2];
			//cout <<"x:"<<ii<<"y:"<<jj<<"scalar:"<< b<<"," << g << "," << r << endl;
			if (b<=100 && g>=160 && r<=100)
			{
				src.at<Vec3b>(ii, jj)[0] = 255;
				src.at<Vec3b>(ii, jj)[1] = 255;
				src.at<Vec3b>(ii, jj)[2] = 255;
			}
		}
	}
}

Rect OCRDetect::AdjustCorrectArea(Rect rect)
{
	int x = rect.x;
	int y = rect.y;
	int width = rect.width;
	int height = rect.height;

	int x1 = x;
	int y1 = y;
	
	int x2 = x + width;
	int y2 = y + height;
	
	bool flag = false;

	//查看其左上角x是否有意义	
	for (int ii=x;ii<x+width;ii++)
	{
		int ycnt = 0;
		for (int jj=y;jj<y+height;jj++)
		{
			uint16_t g_left_top = m_src.at<Vec3b>(jj, ii)[1];
			if (g_left_top>160)
			{
				ycnt += 1;
			}
		}
		float threshold = (float)ycnt / (float)height;
		if (threshold>0.5)
		{
			x1 = ii;  //ii为合理的左上角的x坐标
			flag = true;
			break;
		}
		if (flag)
		{
			break;
		}
	}

	//查看其左上角y是否有意义
	flag = false;
	for (int jj = y; jj<y + height; jj++)
	{
		int xcnt = 0;
		for (int ii = x; ii<x + width; ii++)
		{
			uint16_t g_left_top = m_src.at<Vec3b>(jj, ii)[1];
			if (g_left_top>160)
			{
				xcnt += 1;
			}
		}
		float threshold = (float)xcnt / (float)width;
		if (threshold>0.5)
		{
			y1 = jj;  //ii为合理的左上角的x坐标
			flag = true;
			break;
		}
		if (flag)
		{
			break;
		}
	}

	int tmpWidth = x + width - x1;
	int tempHeight = y + height - y1;

	//查看其右下角x是否有意义
	flag = false;
	for (int ii=rect.x-1+width;ii>x1;ii--)
	{
		int ycnt = 0;
		for (int jj = rect.y+height-1; jj>y1; jj--)
		{
			uint16_t g_left_top = m_src.at<Vec3b>(jj, ii)[1];
			if (g_left_top>160)
			{
				ycnt += 1;
			}
		}
		float threshold = (float)ycnt / (float)tempHeight;
		if (threshold>0.7)
		{
			x2 = ii;  //ii为合理的右下角的x坐标
			flag = true;
			break;
		}
		if (flag)
		{
			break;
		}
	}

	//查看其右下角y是否有意义
	flag = false;
	for (int jj = rect.y + height-1; jj>y1; jj--)
	{
		int xcnt = 0;
		for (int ii = rect.x + width-1; ii>x1; ii--)
		{
			uint16_t g_left_top = m_src.at<Vec3b>(jj, ii)[1];
			if (g_left_top>160)
			{
				xcnt += 1;
			}
		}
		float threshold = (float)xcnt / (float)tmpWidth;
		if (threshold>0.7)
		{
			y2 = jj;  //ii为合理的右下角的x坐标
			flag = true;
			break;
		}
		if (flag)
		{
			break;
		}
	}

	return Rect(x1,y1, x2-x1,y2-y1);

}

DLL_API void OCRDetect::DetectAllImg(Mat src, vector<Rect> &vec, vector<std::string> &vecStrName)
{
    ChangeGreenToWhite(src);
	//imshow("src", src);
	x1 = src.rows*0.1*0;
	y1 = 0;
	x2 = src.rows;
	y2 = src.cols;
	m_src = src(Rect(y1,x1,y2,x2));
	//imshow("src", m_src);
	Mat gray, thresh_canny;
	cvtColor(m_src, gray, COLOR_BGR2GRAY);

	threshold(gray, thresh_canny, 200, 255, 0);
	//imshow("sr2", thresh_canny);
	//形态学操作滤波
	Mat element;
	element = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat dstImage;
	//去噪  
	morphologyEx(thresh_canny, dstImage, MORPH_OPEN, element);
	//imshow("dst", dstImage);

	//canny边缘
	Mat canny;
	Canny(dstImage, canny, 0, 10);
	
	//形态学
	morphologyEx(canny, canny, MORPH_DILATE, element);
	//imshow("canny", canny);
	//定义轮廓和层次结构
	vector<vector<Point> >contours;
	vector<Vec4i> hierarchy;
	findContours(dstImage, contours, hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
	
	vector<Rect >validContours;
	//筛选有效的边界通过面积，宽高比
	for (int ii=0;ii<contours.size();ii++)
	{
		RotatedRect RotRect = minAreaRect(contours[ii]);
		Rect rect = RotRect.boundingRect();
		double area = rect.height*rect.width;
		double ratio = (double)rect.height / (double)rect.width;
		if (area > 1000)
		{
			if (ratio>=0.15 && ratio<=0.5)
			{
				if (rect.y<20)   //删除界面上非监控有效区的框
				{
					continue;
				}
				Rect rectTmp = AdjustCorrectArea(rect);
				Mat tmp=m_src(rectTmp);
				cv::cvtColor(tmp, tmp, COLOR_BGR2GRAY);
				std::string strName = OCRDiscriminate(tmp);
				validContours.push_back(rectTmp);
				vecStrName.push_back(strName);
			}
		}
	}	
	vec = validContours;
	return ;
}

std::string OCRDetect::OCRDiscriminate(Mat pData)
{
	imwrite("pic.jpg", pData);
	tesseract::TessBaseAPI *myOCR = new tesseract::TessBaseAPI();
	if (myOCR->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	myOCR->TesseractRect(pData.data, 1, pData.step1(), 0, 0, pData.cols, pData.rows);
	const char *text1 = myOCR->GetUTF8Text();
	if (text1 == NULL)
	{
		return "";
	}
	string t1(text1);
	t1.erase(std::remove(t1.begin(), t1.end(), '\n'), t1.end());
	return t1.c_str();
}

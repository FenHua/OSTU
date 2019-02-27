// ostu.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<iostream>
using namespace cv;
using namespace std;

int ostu(Mat image)
{
	cvtColor(image.clone(), image, CV_RGB2GRAY);
	int width = image.cols;
	int height = image.rows;
	int x = 0, y = 0;
	float ip1, ip2, is1, is2; // 前景、背景像素统计
	float w0, w1, mean1, mean2, mean; //计算公式所需变量
	float deltaTmp, deltaMax; // 类间差
	is1 = 1;
	is2 = 1;   // 防止分母为0
	int pixelCount[256];
	int i, j, pixelSum = width * height, thres = 0;
	for (i = 0; i < 256; i++)
	{
		pixelCount[i] = 0;
	}
	int pixel = 0;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j <width; j++)
		{
			pixel = image.at<uchar>(i, j); //获取当前点的像素值
			pixelCount[pixel]++; //对像素进行统计
		}
	}

	//***********画出图像直方图********************************
	int pixelCount1[256] = { 0 };
	Mat image1(255, 255, CV_8UC3);
	for (int i = 0; i<255; i++)
	{
		pixelCount1[i] = pixelCount[i] % 200;
		line(image1, Point(i, 235), Point(i, 235 - pixelCount1[i]), Scalar(255, 0, 0), 1, 8, 0);
		if (i % 50 == 0)
		{
			char ch[255];
			sprintf(ch, "%d", i);
			string str = ch;
			putText(image1, str, Point(i, 250), 1, 1, Scalar(0, 0, 255));
		}
	}


	// 遍历选出最大类间差的阈值
	deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		// i 阈值
		ip1 = ip2 = is1 = is2 = w0 = w1 = mean1 = mean2 = mean = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i) 
			{
				//背景部分
				ip1 += pixelCount[j] * j;
				is1 += pixelCount[j];
			}
			else
			{
				//前景部分
				ip2 += pixelCount[j] * j;
				is2 += pixelCount[j];
			}
		}
		mean1 = ip1 / is1;     //第一类像素平均灰度值
		w0 = (float)is1 / (float)pixelSum;//第一类像素占整个图像比例
		mean2 = ip2 / is2;
		w1 = 1 - w0;
		// mean = w0*mean1 + w1*mean2; // 整幅图像的灰度平均值
		deltaTmp = w0*w1*(mean1 - mean2)*(mean1 - mean2); //灰度值为i的阈值的类间方差
		if (deltaTmp>deltaMax)
		{
			deltaMax = deltaTmp;
			thres = i;
		}
	}

	//画出以thres为阈值的分割线
	line(image1, Point(thres, 235), Point(thres, 0), Scalar(0, 0, 255), 2, 8);
	imshow("直方图", image1);
	return thres;
}

int main()
{
	Mat image = imread("test1.jpg");
	imshow("SoureImage", image);
	Mat imageOutput; // 记录函数阈值分割的结果
	Mat imageOtsu; // 记录opencv自带函数的分割结果
	int thresholdValue = ostu(image);
	cout << "类间方差为： " << thresholdValue << endl;
	cvtColor(image, image, CV_RGB2GRAY);
	threshold(image, imageOutput, thresholdValue, 255, CV_THRESH_BINARY);
	threshold(image, imageOtsu, 0, 255, CV_THRESH_OTSU); //Opencv Otsu算法
	imshow("Output Image", imageOutput);
	imshow("Opencv Otsu", imageOtsu);
	waitKey();
	return 0;
}


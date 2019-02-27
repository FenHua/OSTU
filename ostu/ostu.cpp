// ostu.cpp : �������̨Ӧ�ó������ڵ㡣
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
	float ip1, ip2, is1, is2; // ǰ������������ͳ��
	float w0, w1, mean1, mean2, mean; //���㹫ʽ�������
	float deltaTmp, deltaMax; // ����
	is1 = 1;
	is2 = 1;   // ��ֹ��ĸΪ0
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
			pixel = image.at<uchar>(i, j); //��ȡ��ǰ�������ֵ
			pixelCount[pixel]++; //�����ؽ���ͳ��
		}
	}

	//***********����ͼ��ֱ��ͼ********************************
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


	// ����ѡ������������ֵ
	deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		// i ��ֵ
		ip1 = ip2 = is1 = is2 = w0 = w1 = mean1 = mean2 = mean = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i) 
			{
				//��������
				ip1 += pixelCount[j] * j;
				is1 += pixelCount[j];
			}
			else
			{
				//ǰ������
				ip2 += pixelCount[j] * j;
				is2 += pixelCount[j];
			}
		}
		mean1 = ip1 / is1;     //��һ������ƽ���Ҷ�ֵ
		w0 = (float)is1 / (float)pixelSum;//��һ������ռ����ͼ�����
		mean2 = ip2 / is2;
		w1 = 1 - w0;
		// mean = w0*mean1 + w1*mean2; // ����ͼ��ĻҶ�ƽ��ֵ
		deltaTmp = w0*w1*(mean1 - mean2)*(mean1 - mean2); //�Ҷ�ֵΪi����ֵ����䷽��
		if (deltaTmp>deltaMax)
		{
			deltaMax = deltaTmp;
			thres = i;
		}
	}

	//������thresΪ��ֵ�ķָ���
	line(image1, Point(thres, 235), Point(thres, 0), Scalar(0, 0, 255), 2, 8);
	imshow("ֱ��ͼ", image1);
	return thres;
}

int main()
{
	Mat image = imread("test1.jpg");
	imshow("SoureImage", image);
	Mat imageOutput; // ��¼������ֵ�ָ�Ľ��
	Mat imageOtsu; // ��¼opencv�Դ������ķָ���
	int thresholdValue = ostu(image);
	cout << "��䷽��Ϊ�� " << thresholdValue << endl;
	cvtColor(image, image, CV_RGB2GRAY);
	threshold(image, imageOutput, thresholdValue, 255, CV_THRESH_BINARY);
	threshold(image, imageOtsu, 0, 255, CV_THRESH_OTSU); //Opencv Otsu�㷨
	imshow("Output Image", imageOutput);
	imshow("Opencv Otsu", imageOtsu);
	waitKey();
	return 0;
}


#pragma once

#include "stdafx.h"

class SeamDetector
{
private:
	cv::Mat *energyMatrix;
	cv::Mat *seamMatrix;
	int width;
	int height;
public:
	SeamDetector( int width, int height );
	SeamDetector( cv::Mat &originalImage );
	void findSeam();
	void iterateSeam(int row);
	void drawSeam(int col);
	cv::Mat * getImage();
	cv::Mat * getEnergyMatrix();
	~SeamDetector();
};


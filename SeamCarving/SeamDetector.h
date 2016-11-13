#pragma once

#include "stdafx.h"

class SeamDetector
{
private:
	cv::Mat originalImageMatrix;
	cv::Mat energyMatrix;
	cv::Mat seamMatrix;
	std::vector<int> verticalSeam;
	std::vector<int> horizontalSeam;
	int width;
	int height;
public:
	explicit  SeamDetector( cv::Mat &originalImage );
	void findVerticalSeam();
	void iterateEnergyMatrix(int row);
	void traceVerticalSeam();
	void findVerticalSeamStartingPoint();
	void iterateVerticalSeam(int row);
	void traceHorizontalSeam();
	void drawVerticalSeam();
	cv::Mat * getImage();
	cv::Mat * getEnergyMatrix();
	~SeamDetector();
};


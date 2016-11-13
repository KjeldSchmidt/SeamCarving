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
	void prepareEnergyMatrix();
	void findVerticalSeam();
	void iterateVerticalSeamMatrix(int row);
	void traceVerticalSeam();
	void findVerticalSeamStartingPoint();
	void iterateVerticalSeam(int row);
	void traceHorizontalSeam();
	void findHorizontalSeamStartingPoint();
	void iterateHorizontalSeam(int row);
	void drawHorizontalSeam();
	void removeHorizontalSeam();
	void drawVerticalSeam();
	void removeVerticalSeam();
	void findHorizontalSeam();
	void iterateHorizontalSeamMatrix(int row);
	cv::Mat * getImage();
	cv::Mat * getEnergyMatrix();
	cv::Mat* getSeamMatrix();
	~SeamDetector();
};


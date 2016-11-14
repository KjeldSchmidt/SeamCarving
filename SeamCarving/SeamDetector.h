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

	void iterateVerticalSeamMatrix( int row );
	void traceVerticalSeam();
	void findVerticalSeamStartingPoint();
	void iterateVerticalSeam( int row );

	void iterateHorizontalSeamMatrix( int row );
	void traceHorizontalSeam();
	void findHorizontalSeamStartingPoint();
	void iterateHorizontalSeam( int row );

public:
	explicit  SeamDetector( cv::Mat &originalImage );
	void prepareEnergyMatrix();
	
	void findVerticalSeam();
	void drawVerticalSeam();
	void removeVerticalSeam();

	void findHorizontalSeam();
	void drawHorizontalSeam();
	void removeHorizontalSeam();
	
	cv::Mat * getImage();
	cv::Mat * getEnergyMatrix();
	cv::Mat* getSeamMatrix();
	int getHeight() const;
	int getWidth() const;
	~SeamDetector();
};


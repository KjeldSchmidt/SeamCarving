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
	bool currentlyTransposed = false;
	bool energyMatrixIsSet = false;

	void prepareEnergyMatrix();

	void findSeam();
	void drawSeam();
	void removeSeam();
	void iterateSeamMatrix( int row );
	void traceSeam();
	void findSeamStartingPoint();
	void iterateSeam( int row );

	void transpose();

public:
	explicit  SeamDetector( cv::Mat &originalImage );
	
	void findVerticalSeam();
	void drawVerticalSeam();
	void removeVerticalSeam();

	void findHorizontalSeam();
	void drawHorizontalSeam();
	void removeHorizontalSeam();

	void setCorrectOrientation();
	cv::Mat * getImage();
	cv::Mat * getEnergyMatrix();
	cv::Mat* getSeamMatrix();
	int getHeight() const;
	int getWidth() const;

	~SeamDetector();
};

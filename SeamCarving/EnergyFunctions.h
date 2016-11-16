#pragma once

#include "stdafx.h"

class EnergyFunctions {
public:
	EnergyFunctions();
	~EnergyFunctions();
	static cv::Mat StupidBrightness(const cv::Mat& image);
	static cv::Mat DirectionIndependentSorbel(const cv::Mat& image);
private:
	static void lumaFromBGR( const cv::Mat & source, cv::Mat & destination );
	static void SorbelX(const cv::Mat& source, cv::Mat& destination);
	static void SorbelY(const cv::Mat& source, cv::Mat& destination);
};


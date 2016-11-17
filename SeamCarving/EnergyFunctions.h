#pragma once

#include "stdafx.h"

class EnergyFunctions {
public:
	EnergyFunctions();
	~EnergyFunctions();
	static cv::Mat DirectionIndependentSobel( const cv::Mat& image );
private:
	static void lumaFromBGR( const cv::Mat & source, cv::Mat & destination );
	static cv::Mat smoothLumaMap( const cv::Mat & source );
	static void SobelX( const cv::Mat& source, cv::Mat& destination );
	static void SobelY( const cv::Mat& source, cv::Mat& destination );
	static void combineSobelDirections( cv::Mat gx, cv::Mat gy, cv::Mat output );
};


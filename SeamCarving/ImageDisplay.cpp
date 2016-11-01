#include "stdafx.h"
#include "ImageDisplay.h"


ImageDisplay::ImageDisplay( std::string name ): name( name )
{
	cv::namedWindow(name);
}


ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::showImage(cv::Mat& image)
{
	cv::imshow( name, image );
}
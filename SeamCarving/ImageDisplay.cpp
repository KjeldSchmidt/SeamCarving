#include "stdafx.h"

ImageDisplay::ImageDisplay( std::string name ): name( name )
{
	cv::namedWindow(name);
}


ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::showImage(cv::Mat& image)
{
	openImage = image;
	cv::imshow( name, openImage );
	cv::waitKey(1);
}

void ImageDisplay::refresh() const {
	cv::imshow(name, openImage);
	cv::waitKey(1);
}
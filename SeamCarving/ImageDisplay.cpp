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
	openImage = image;
	displayedImage = openImage.clone();
	cv::imshow( name, displayedImage );
	cv::waitKey(1);
}

void ImageDisplay::refresh() {
	cv::imshow(name, displayedImage);
	cv::waitKey(1);
}

void ImageDisplay::DrawLine(int col) {
	if (col > openImage.cols) {
		throw std::invalid_argument("Input must be smaller than image width");
	}
	
	int rows = openImage.rows;
	CvScalar color = ( 255, 0, 255 );

	for (int y = 0; y < rows; y++) {
		displayedImage.at<cv::Vec3b>(CvPoint(col, y)) = cv::Vec3b(255, 0, 255);
	}

	this->refresh();
}
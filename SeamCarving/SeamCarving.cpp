// SeamCarving.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay originalImage("Original Image");
	ImageDisplay scaledImage("Scaled Image");

	cv::Mat image = ImageReader::readImage("testImage.jpg");


	if (!image.empty()) {
		std::cout << "Image loaded" << std::endl;
		originalImage.showImage(image);

		SeamDetector seamDetector( image );
		seamDetector.prepareEnergyMatrix();
		seamDetector.findVerticalSeam();
		seamDetector.drawVerticalSeam();
		seamDetector.findHorizontalSeam();
		seamDetector.drawHorizontalSeam();
		cv::Mat* seamedImage = seamDetector.getImage();
		scaledImage.showImage( *seamedImage );
	}
	else {
		std::cout << "Empty image" << std::endl;
	}

	system( "pause" );

    return 0;
}

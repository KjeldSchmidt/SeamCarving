// SeamCarving.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay originalImage("Original Image");
	ImageDisplay scaledImage("Sclaed Image");

	cv::Mat image = ImageReader::readImage("testimage.jpg");
	SeamDetector seamDetector( image );


	if (!image.empty()) {
		std::cout << "Image loaded" << std::endl;
		originalImage.showImage(image);

		seamDetector.findVerticalSeam();
		seamDetector.drawVerticalSeam();
		cv::Mat* seamedImage = seamDetector.getImage();
		scaledImage.showImage( *seamedImage );
	}
	else {
		std::cout << "Empty image" << std::endl;
	}

	system( "pause" );

    return 0;
}

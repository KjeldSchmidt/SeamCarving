// SeamCarving.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay originalImage("Original Image");
	ImageDisplay scaledImage("Scaled Image");

	cv::Mat image = ImageReader::readImage("testimage.jpg");


	if (!image.empty()) {
		std::cout << "Image loaded" << std::endl;
		
		originalImage.showImage( image );

		SeamDetector seamDetector( image );
		seamDetector.prepareEnergyMatrix();
		
		for ( int i = 0; i < 100; ++i )
		{
			seamDetector.findVerticalSeam();
			seamDetector.removeVerticalSeam();
		}		
		
		cv::Mat* scaled = seamDetector.getImage();
		scaledImage.showImage( *scaled );
	}
	else {
		std::cout << "Empty image" << std::endl;
	}

	system( "pause" );

    return 0;
}

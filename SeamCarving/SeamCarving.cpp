// SeamCarving.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int askUserForNewHeight( int oldHeight ) {
	int newHeight = 0;
	while ( newHeight < 1 || newHeight > oldHeight ) {
		std::cout << "Enter new height between 1 and " << oldHeight << " (inclusive)" << std::endl;
		scanf_s( "%i", &newHeight );
		std::cout << newHeight;

		if (  newHeight < 1 || newHeight > oldHeight ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' );
	}
	
	return newHeight;
}

int askUserForNewWidth( int oldWidth ) {
	int newWidth = 0;
	while ( newWidth < 1 || newWidth > oldWidth ) {
		std::cout << "Enter new width between 1 and " << oldWidth << " (inclusive)" << std::endl;
		scanf_s( "%i", &newWidth );
		std::cout << newWidth;

		if ( newWidth < 1 || newWidth > oldWidth ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' );
	}

	return newWidth;
}

int main() {
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay originalImage( "Original Image" );
	ImageDisplay scaledImage( "Scaled Image" );

	cv::Mat image = ImageReader::readImage( "bars.jpg" );


	if ( !image.empty() ) {
		std::cout << "Image loaded" << std::endl;

		originalImage.showImage( image );

		SeamDetector seamDetector( image );
		seamDetector.prepareEnergyMatrix();


		int oldHeight = seamDetector.getHeight();
		int oldWidth = seamDetector.getWidth();
		int newHeight = askUserForNewHeight( oldHeight );
		int newWidth = askUserForNewWidth( oldWidth );


		for ( int i = oldHeight; i > newHeight; --i ) {
			seamDetector.findHorizontalSeam();
			seamDetector.drawHorizontalSeam();
			scaledImage.showImage( *seamDetector.getImage() );
			seamDetector.removeHorizontalSeam();
		}

		for ( int i = oldWidth; i > newWidth; --i ) {
			seamDetector.findVerticalSeam();
			seamDetector.drawVerticalSeam();
			scaledImage.showImage( *seamDetector.getImage() );
			seamDetector.removeVerticalSeam();
		}



		cv::Mat* scaled = seamDetector.getImage();
		scaledImage.showImage( *scaled );
	} else {
		std::cout << "Empty image" << std::endl;
	}

	system( "pause" );

	return 0;
}
#include "stdafx.h"

int askUserForNewHeight( int oldHeight );
int askUserForNewWidth( int oldWidth );

int main() {
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay EnergyImage( "Energy Map" );
	ImageDisplay scaledImage( "Scaled Image" );

	cv::Mat image = ImageReader::readImage( "images/castle.jpg" );


	if ( !image.empty() ) {
		std::cout << "Image loaded" << std::endl;


		SeamDetector seamDetector( image );

		seamDetector.prepareEnergyMatrix();
		scaledImage.showImage( *seamDetector.getImage() );
		EnergyImage.showImage( *seamDetector.getEnergyMatrix() );

		system( "pause" );

		int oldHeight = seamDetector.getHeight();
		int oldWidth = seamDetector.getWidth();
		int rowsToRemoveCount = oldHeight - askUserForNewHeight( oldHeight );
		int colsToRemoveCount = oldWidth - askUserForNewWidth( oldWidth );


		for ( int i = 0; i < colsToRemoveCount; ++i ) {
			seamDetector.findVerticalSeam();
			seamDetector.drawVerticalSeam();
			scaledImage.showImage( *seamDetector.getImage() );
			EnergyImage.showImage( *seamDetector.getEnergyMatrix() );
			seamDetector.removeVerticalSeam();
		}

		for ( int i = 0; i < rowsToRemoveCount; ++i ) {
			seamDetector.findHorizontalSeam();
			seamDetector.drawHorizontalSeam();
			scaledImage.showImage( *seamDetector.getImage() );
			EnergyImage.showImage( *seamDetector.getEnergyMatrix() );
			seamDetector.removeHorizontalSeam();
		}


		seamDetector.setCorrectOrientation();
		scaledImage.showImage( *seamDetector.getImage() );
		EnergyImage.showImage( *seamDetector.getEnergyMatrix() );

	} else {
		std::cout << "Empty image" << std::endl;
	}

	system( "pause" );

	return 0;
}

int askUserForNewHeight( int oldHeight ) {
	int newHeight = 0;
	while ( newHeight < 1 || newHeight > oldHeight ) {
		std::cout << "Enter new height between 1 and " << oldHeight << " (inclusive)" << std::endl;
		scanf_s( "%i", &newHeight );

		if ( newHeight < 1 || newHeight > oldHeight ) {
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

		if ( newWidth < 1 || newWidth > oldWidth ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' );
	}

	return newWidth;
}
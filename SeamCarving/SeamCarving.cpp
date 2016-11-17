#include "stdafx.h"

int getNumberOfRowsToBeRemoved( int oldHeight );
int getNumberOfColumnsToBeRemoved( int oldWidth );
void removeColumns( int columnsToRemoveCount, SeamDetector &sd );
void removeRows( int rowsToRemoveCount, SeamDetector &sd );

ImageDisplay energyMap( "Energy Map" );
ImageDisplay scaledImage( "Scaled Image" );

int main() {
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;
	cv::Mat image = ImageReader::readImage( "images/castle.jpg" );

	if ( !image.empty() ) {
		std::cout << "Image loaded" << std::endl;
		SeamDetector seamDetector( image );

		seamDetector.prepareEnergyMatrix();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );

		int rowsToRemoveCount = getNumberOfRowsToBeRemoved( seamDetector.getHeight() );
		int colsToRemoveCount = getNumberOfColumnsToBeRemoved( seamDetector.getWidth() );

		removeColumns( colsToRemoveCount, seamDetector );
		removeRows( rowsToRemoveCount, seamDetector );

		seamDetector.setCorrectOrientation();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );
	} else {
		std::cout << "Unfortunately, the image could not be opened." << std::endl;
	}

	system( "pause" );

	return 0;
}

void removeColumns( int colsToRemoveCount, SeamDetector &seamDetector ) {
	for ( int i = 0; i < colsToRemoveCount; ++i ) {
		seamDetector.findVerticalSeam();
		seamDetector.drawVerticalSeam();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );
		seamDetector.removeVerticalSeam();
	}
}

void removeRows( int rowsToRemoveCount, SeamDetector &seamDetector ) {
	for ( int i = 0; i < rowsToRemoveCount; ++i ) {
		seamDetector.findHorizontalSeam();
		seamDetector.drawHorizontalSeam();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );
		seamDetector.removeHorizontalSeam();
	}
}

int getNumberOfRowsToBeRemoved( int oldHeight ) {
	int rowsToRemove = 0;
	while ( rowsToRemove < 1 || rowsToRemove > oldHeight ) {
		std::cout << "How many rows should be removed? Valid values: 0 to " << oldHeight << ", inclusive" << std::endl;
		scanf_s( "%i", &rowsToRemove );

		if ( rowsToRemove < 1 || rowsToRemove > oldHeight ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' ); // This loop removes remaining input from stdin.
	}

	return rowsToRemove;
}

int getNumberOfColumnsToBeRemoved( int oldWidth ) {
	int colsToRemove = 0;
	while ( colsToRemove < 1 || colsToRemove > oldWidth ) {
		std::cout << "How many colums should be removed? Valid values: 0 to " << oldWidth << ", inclusive" << std::endl;
		scanf_s( "%i", &colsToRemove );

		if ( colsToRemove < 1 || colsToRemove > oldWidth ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' ); // This loop removes remaining input from stdin.
	}

	return colsToRemove;
}
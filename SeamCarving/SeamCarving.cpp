#include "stdafx.h"

int getNumberOfRowsToBeRemoved( int oldHeight );
int getNumberOfColumnsToBeRemoved( int oldWidth );
SeamDetector openImageDialog();
void removeColumns( int columnsToRemoveCount, SeamDetector &sd );
void removeRows( int rowsToRemoveCount, SeamDetector &sd );

ImageDisplay energyMap( "Energy Map" );
ImageDisplay scaledImage( "Scaled Image" );

int main() {
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;
	SeamDetector seamDetector = openImageDialog();

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

	system( "pause" );

	return 0;
}

SeamDetector openImageDialog() {
	std::string file;
	cv::Mat image;

	std::cout << "Please enter a file name for an image you would like to open." << std::endl;
	while ( true ) {
		std::cin >> file;

		image = ImageReader::readImage( file );

		if ( !image.empty() ) {
			std::cout << "Image loaded" << std::endl;
			return SeamDetector( image );
		} else {
			std::cout << "File could not be opened. Check for spelling errors and try again" << std::endl;
		}
	}
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
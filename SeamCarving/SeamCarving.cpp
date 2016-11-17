#include "stdafx.h"

void interactiveMode();
void demoMode();
void cliMode( std::string inputFile, std::string outputFile, int rows, int cols );

int getNumberOfRowsToBeRemoved( int oldHeight );
int getNumberOfColumnsToBeRemoved( int oldWidth );
SeamDetector openImageDialog();
void saveImageDialog( cv::Mat image );
void removeColumns( int colsToRemoveCount, SeamDetector &sd );
void removeRows( int rowsToRemoveCount, SeamDetector &sd );
void removeColumnsBlind( int colsToRemoveCount, SeamDetector &sd );
void removeRowsBlind( int rowsToRemoveCount, SeamDetector &sd );


ImageDisplay energyMap( "Energy Map" );
ImageDisplay scaledImage( "Scaled Image" );





int main( int argc, char *argv[] ) {
	std::vector<std::string> args( argv, argv + argc );

	if ( argc == 1 ) {
		interactiveMode();
	} else if ( argc == 2 && !args.at( 1 ).compare( "demo" ) ) {
		demoMode();
	} else if ( argc == 5 ) {
		std::string input = args.at( 1 );
		std::string output = args.at( 2 );
		int rows = std::stoi( args.at( 3 ) );
		int cols = std::stoi( args.at( 4 ) );

		cliMode( input, output, rows, cols );
	}

	return 0;
}

void interactiveMode() {
	std::cout << "Welcome to SeamCarving, using OpenCV version : " << CV_VERSION << std::endl;
	SeamDetector seamDetector = openImageDialog();

	seamDetector.prepareEnergyMatrix();
	scaledImage.showImage( *seamDetector.getImage() );
	energyMap.showImage( *seamDetector.getEnergyMatrix() );

	int colsToRemoveCount = getNumberOfColumnsToBeRemoved( seamDetector.getWidth() );
	int rowsToRemoveCount = getNumberOfRowsToBeRemoved( seamDetector.getHeight() );

	removeColumns( colsToRemoveCount, seamDetector );
	removeRows( rowsToRemoveCount, seamDetector );

	seamDetector.finalize();
	scaledImage.showImage( *seamDetector.getImage() );
	energyMap.showImage( *seamDetector.getEnergyMatrix() );

	saveImageDialog( *seamDetector.getImage() );

	system( "pause" );
}


void demoMode() {
	std::cout << "Entering demo mode." << std::endl;

	cv::Mat image = ImageReader::readImage( "cake.jpg" );

	if ( !image.empty() ) {
		SeamDetector seamDetector( image );

		std::cout << "Preparing enery matrix..." << std::endl;

		seamDetector.prepareEnergyMatrix();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );

		std::cout << "Removing 100 columns and 50 rows..." << std::endl;

		removeColumns( 100, seamDetector );
		removeRows( 50, seamDetector );

		std::cout << "Done!" << std::endl;

		seamDetector.finalize();
		scaledImage.showImage( *seamDetector.getImage() );
		energyMap.showImage( *seamDetector.getEnergyMatrix() );

		std::cout << "Demo Mode does not save resulting images" << std::endl;

		system( "pause" );
	} else {
		std::cout << "The example image could not be opened. Please place a file named cake.jpg in this directory." << std::endl;
		system( "pause" );
	}

}

void cliMode( std::string inputFile, std::string outputFile, int rows, int cols ) {
	std::cout << "Processing..." << std::endl;

	cv::destroyAllWindows();

	cv::Mat image = ImageReader::readImage( inputFile );

	if ( !image.empty() && image.rows > rows && image.cols > cols && cols >= 0 && rows >= 0 ) {
		SeamDetector seamDetector( image );
		removeColumnsBlind( cols, seamDetector );
		removeRowsBlind( rows, seamDetector );

		seamDetector.finalize();

		outputFile.append( ".jpg" );
		cv::imwrite( outputFile, *seamDetector.getImage() );
		std::cout << "File has been saved as " << outputFile << std::endl;

		system( "pause" );
	} else {
		std::cout << "The input file could not be opened, image resolution is too small or an argument could not be parsed." << std::endl;
		system( "pause" );
	}
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
			std::cout << "Or try some example images - cake.jpg, castle.jpg, lena.jpg, shoes.jpg." << std::endl;
		}
	}
}

void saveImageDialog( cv::Mat image ) {
	std::string file;
	std::cout << "Please enter a name for your image, such as 'friends', 'canoe', or 'muffins'." << std::endl;
	std::cin >> file;
	if ( file.size() == 0 ) {
		file.append( "default" );
	}
	file.append( ".jpg" );

	cv::imwrite( file, image );
	std::cout << "File has been saved as " << file << std::endl;
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

void removeColumnsBlind( int colsToRemoveCount, SeamDetector &seamDetector ) {
	for ( int i = 0; i < colsToRemoveCount; ++i ) {
		seamDetector.findVerticalSeam();
		seamDetector.drawVerticalSeam();
		seamDetector.removeVerticalSeam();
	}
}

void removeRowsBlind( int rowsToRemoveCount, SeamDetector &seamDetector ) {
	for ( int i = 0; i < rowsToRemoveCount; ++i ) {
		seamDetector.findHorizontalSeam();
		seamDetector.drawHorizontalSeam();
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
		std::cout << "How many columns should be removed? Valid values: 0 to " << oldWidth << ", inclusive" << std::endl;
		scanf_s( "%i", &colsToRemove );

		if ( colsToRemove < 1 || colsToRemove > oldWidth ) {
			std::cout << "Sorry, that doesn't seem to be right." << std::endl;
		}
		while ( getchar() != '\n' ); // This loop removes remaining input from stdin.
	}

	return colsToRemove;
}
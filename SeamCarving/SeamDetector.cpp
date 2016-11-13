#include "stdafx.h"

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	originalImageMatrix = *new cv::Mat( originalImage );
	width = originalImageMatrix.cols;
	height = originalImageMatrix.rows;

	energyMatrix = *new cv::Mat( height, width, CV_32SC1 );
	seamMatrix = *new cv::Mat( height, width, CV_32SC1 );
}

/*
 * Dummy function - works by stupidity, not any actual energy.
 */
void SeamDetector::prepareEnergyMatrix() {
	for ( int row = 0; row < height; ++row ) {
		for ( int col = 0; col < width; ++col ) {
			auto color = originalImageMatrix.at<cv::Vec3b>( CvPoint( col, row ) );
			int sumColor = color[ 0 ] + color[ 1 ] + color[ 2 ];
			energyMatrix.at<int>( CvPoint( col, row ) ) = sumColor;
		}
	}

	seamMatrix = energyMatrix.clone();
}


void SeamDetector::findVerticalSeam() {
	for ( auto i = 1; i < seamMatrix.rows; ++i ) {
		iterateVerticalSeamMatrix( i );
	}

	traceVerticalSeam();
}


void SeamDetector::iterateVerticalSeamMatrix( int row ) {
	for ( auto col = 0; col < width; ++col ) {
		int lowestNeighbourAbove = 0;
		int left = std::numeric_limits<int>::max();
		int right = std::numeric_limits<int>::max();
		int top = seamMatrix.at<int>( CvPoint( col, row - 1 ) );
		if ( col > 0 ) {
			left = seamMatrix.at<int>( CvPoint( col - 1, row - 1 ) );
		}
		if ( col < width - 1 ) {
			right = seamMatrix.at<int>( CvPoint( col + 1, row - 1 ) );
		}

		lowestNeighbourAbove = ( left < top ) ? left : top;
		lowestNeighbourAbove = ( right < lowestNeighbourAbove ) ? right : lowestNeighbourAbove;

		seamMatrix.at<int>( CvPoint( col, row ) ) += lowestNeighbourAbove;
	}
}


void SeamDetector::traceVerticalSeam() {

	verticalSeam.clear();
	findVerticalSeamStartingPoint();


	for ( int row = seamMatrix.rows - 2; row > 0; --row ) {
		iterateVerticalSeam( row );
	}
}

void SeamDetector::findVerticalSeamStartingPoint() {
	int startingPoint = 0;
	int minimumEnergy = std::numeric_limits<int>::max();
	for ( int col = 0; col < seamMatrix.cols; ++col ) {
		int energyAtIndex = seamMatrix.at<int>( CvPoint( col, height - 1 ) );
		if ( energyAtIndex < minimumEnergy ) {
			startingPoint = col;
			minimumEnergy = energyAtIndex;
		}
	}

	verticalSeam.push_back( startingPoint );
}

void SeamDetector::iterateVerticalSeam( int row ) {
	// Initialize default values.
	int previousIndex = verticalSeam.back();    // The index of the last point of the seam. Only check top left, top top and top right from here.
	char indexShift = 0;                        // Will indicate where to move next. Should only ever be -1, 0 or 1.

	int leftEnergy;
	int rightEnergy;
	int topEnergy = seamMatrix.at<int>( CvPoint( previousIndex, row - 1 ) );
	int lowestEnergyAbove = topEnergy;

	if ( previousIndex > 0 ) {
		leftEnergy = seamMatrix.at<int>( CvPoint( previousIndex - 1, row - 1 ) );
		if ( leftEnergy < lowestEnergyAbove ) {
			lowestEnergyAbove = leftEnergy;
			indexShift = -1;
		}
	}

	if ( previousIndex < width - 1 ) {
		rightEnergy = seamMatrix.at<int>( CvPoint( previousIndex + 1, row - 1 ) );
		if ( rightEnergy < lowestEnergyAbove ) {
			indexShift = 1;
		}
	}

	verticalSeam.push_back( previousIndex + indexShift );
}

void SeamDetector::drawVerticalSeam() {
	auto p = verticalSeam.begin();
	int rowIndex = height - 1;
	for ( ; p != verticalSeam.end(); ++p, --rowIndex ) {
		originalImageMatrix.at<cv::Vec3b>( CvPoint( *p, rowIndex ) ) = cv::Vec3i( 255, 0, 0 );
	}
}


/*
 * Horizontal seams begin here!
 */

void SeamDetector::findHorizontalSeam() {
	for ( int i = 1; i < seamMatrix.rows; ++i ) {
		iterateHorizontalSeamMatrix( i );
	}

	traceHorizontalSeam();
}


void SeamDetector::iterateHorizontalSeamMatrix( int col ) {
	for ( auto row = 0; row < height; ++row ) {
		int lowestNeighbourLeft = 0;
		int top = std::numeric_limits<int>::max();
		int bottom = std::numeric_limits<int>::max();
		int left = seamMatrix.at<int>( CvPoint( col - 1, row ) );
		if ( row > 0 ) {
			top = seamMatrix.at<int>( CvPoint( col - 1, row - 1 ) );
		}
		if ( row < height - 1 ) {
			bottom = seamMatrix.at<int>( CvPoint( col - 1, row + 1 ) );
		}

		lowestNeighbourLeft = ( top < left ) ? top : left;
		lowestNeighbourLeft = ( bottom < lowestNeighbourLeft ) ? bottom : lowestNeighbourLeft;

		seamMatrix.at<int>( CvPoint( col, row ) ) += lowestNeighbourLeft;
	}
}


void SeamDetector::traceHorizontalSeam() {

	horizontalSeam.clear();
	findHorizontalSeamStartingPoint();


	for ( int col = seamMatrix.cols - 2; col > 0; --col ) {
		iterateHorizontalSeam( col );
	}
}

void SeamDetector::findHorizontalSeamStartingPoint() {
	int startingPoint = 0;
	int minimumEnergy = std::numeric_limits<int>::max();
	for ( int row = 0; row < seamMatrix.rows; ++row ) {
		int energyAtIndex = seamMatrix.at<int>( CvPoint( width - 1, row ) );
		if ( energyAtIndex < minimumEnergy ) {
			startingPoint = row;
			minimumEnergy = energyAtIndex;
		}
	}

	horizontalSeam.push_back( startingPoint );
}

void SeamDetector::iterateHorizontalSeam( int col ) {
	int previousIndex = horizontalSeam.back();    // The index of the last point of the seam. Only check top left, top top and top right from here.
	char indexShift = 0;                          // Will indicate where to move next. Should only ever be -1, 0 or 1.

	int topEnergy;
	int bottomEnergy;
	int leftEnergy = seamMatrix.at<int>( CvPoint( col - 1, previousIndex ) );
	int lowestEnergyLeft = leftEnergy;

	if ( previousIndex > 0 ) {
		topEnergy = seamMatrix.at<int>( CvPoint( col - 1, previousIndex - 1 ) );
		if ( topEnergy < lowestEnergyLeft ) {
			lowestEnergyLeft = topEnergy;
			indexShift = -1;
		}
	}

	if ( previousIndex < height - 1 ) {
		bottomEnergy = seamMatrix.at<int>( CvPoint( col - 1, previousIndex + 1 ) );
		if ( bottomEnergy < lowestEnergyLeft ) {
			indexShift = 1;
		}
	}

	horizontalSeam.push_back( previousIndex + indexShift );
}

void SeamDetector::drawHorizontalSeam() {
	auto p = horizontalSeam.begin();
	int colIndex = width - 1;
	for ( ; p != horizontalSeam.end(); ++p, --colIndex ) {
		originalImageMatrix.at<cv::Vec3b>( CvPoint( colIndex, *p ) ) = cv::Vec3i( 255, 0, 0 );
	}
}

cv::Mat* SeamDetector::getImage() {
	return &originalImageMatrix;
}

cv::Mat* SeamDetector::getEnergyMatrix() {
	return &seamMatrix;
}

SeamDetector::~SeamDetector() {}

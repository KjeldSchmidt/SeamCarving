#include "stdafx.h"

void printVector( std::vector<int> v ) {
	for ( auto i = v.begin(); i != v.end(); ++i )
	{
		std::cout << *i << std::endl;
	}
}

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	originalImageMatrix = *new cv::Mat( originalImage );
	energyMatrix = originalImageMatrix.clone();
	seamMatrix = originalImageMatrix.clone();
	width = originalImageMatrix.cols;
	height = originalImageMatrix.rows;
}

void SeamDetector::findVerticalSeam() {
	for ( auto i = 1; i < seamMatrix.rows; ++i ) {
		iterateEnergyMatrix( i );
	}

	traceVerticalSeam();

	printVector( verticalSeam );
}


void SeamDetector::iterateEnergyMatrix( int row ) {
	for ( auto col = 0; col < originalImageMatrix.cols; ++col ) {
		int lowestNeighbourAbove = 0;
		auto left = cv::Vec3i( 255, 255, 255 );
		auto right = cv::Vec3i( 255, 255, 255 );
		auto top = originalImageMatrix.at<cv::Vec3b>( CvPoint( col, row - 1 ) );
		if ( col > 0 ) {
			left = originalImageMatrix.at<cv::Vec3b>( CvPoint( col - 1, row - 1 ) );
		}
		if ( col < originalImageMatrix.cols - 1 ) {
			right = originalImageMatrix.at<cv::Vec3b>( CvPoint( col + 1, row - 1 ) );
		}

		lowestNeighbourAbove = ( left[ 0 ] < top[ 0 ] ) ? left[ 0 ] : top[ 0 ];
		lowestNeighbourAbove = ( right[ 0 ] < lowestNeighbourAbove ) ? right[ 0 ] : lowestNeighbourAbove;

		seamMatrix.at<cv::Vec3b>( CvPoint( col, row ) ) += cv::Vec3i( lowestNeighbourAbove, 0, 0 );
	}
}


void SeamDetector::traceVerticalSeam() {

	verticalSeam.clear();
	findVerticalSeamStartingPoint();


	for ( int row = seamMatrix.rows - 2; row > 0; --row )
	{
		iterateVerticalSeam( row );
	}
}

void SeamDetector::findVerticalSeamStartingPoint() {
	int startingPoint = 0;
	int minimumEnergy = std::numeric_limits<int>::max();
	for ( int col = 0; col >= seamMatrix.cols; --col )
	{
		int energyAtIndex = seamMatrix.at<cv::Vec3i>( CvPoint( col, height - 1 ) )[ 0 ];

		if ( energyAtIndex < minimumEnergy )
		{
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

	int leftEnergy = std::numeric_limits<int>::max();
	int rightEnergy = std::numeric_limits<int>::max();
	int topEnergy = originalImageMatrix.at<cv::Vec3b>( CvPoint( previousIndex, row - 1 ) )[ 0 ];
	int lowestEnergyAbove = topEnergy;

	if ( previousIndex > 0 )
	{
		leftEnergy = seamMatrix.at<cv::Vec3b>( CvPoint( previousIndex - 1, row - 1 ) )[ 0 ];
		if ( leftEnergy < lowestEnergyAbove )
		{
			lowestEnergyAbove = leftEnergy;
			indexShift = -1;
		}
	}

	if ( previousIndex < width - 1 )
	{
		rightEnergy = seamMatrix.at<cv::Vec3b>( CvPoint( previousIndex + 1, row - 1 ) )[ 0 ];
		if ( rightEnergy < lowestEnergyAbove )
		{
			indexShift = 1;
		}
	}

	verticalSeam.push_back( previousIndex + indexShift );
}

void SeamDetector::traceHorizontalSeam() {

}


void SeamDetector::drawVerticalSeam() {
	auto p = verticalSeam.begin();
	int rowIndex = height - 1;
	for ( ; p != verticalSeam.end(); ++p, --rowIndex ) {
		originalImageMatrix.at<cv::Vec3b>( CvPoint( *p, rowIndex ) ) = cv::Vec3i( 255, 0, 0 );
	}
}

cv::Mat* SeamDetector::getImage() {
	return &originalImageMatrix;
}

cv::Mat* SeamDetector::getEnergyMatrix() {
	return &seamMatrix;
}

SeamDetector::~SeamDetector() {}

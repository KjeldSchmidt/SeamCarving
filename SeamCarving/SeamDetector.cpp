#include "stdafx.h"


SeamDetector::SeamDetector( int inputWidth, int inputHeight ) : width( inputWidth ), height( inputHeight ) {
	energyMatrix = *new cv::Mat( height, width, CV_8SC3 );
	seamMatrix = energyMatrix;
}

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	energyMatrix = *new cv::Mat( originalImage );
	seamMatrix = energyMatrix;
	width = energyMatrix.cols;
	height = energyMatrix.rows;
}

void SeamDetector::findVerticalSeam() {
	for ( auto i = 1; i < seamMatrix.rows; ++i ) {
		iterateSeam( i );
	}


}


void SeamDetector::iterateSeam( int row ) {
	for ( auto col = 0; col < energyMatrix.cols; ++col ) {
		int lowestNeighbourAbove = 0;
		auto left = cv::Vec3i( 255, 255, 255 );
		auto right = cv::Vec3i( 255, 255, 255 );
		auto top = energyMatrix.at<cv::Vec3b>( CvPoint( col, row - 1 ) );
		if ( col > 0 ) {
			left = energyMatrix.at<cv::Vec3b>( CvPoint( col - 1, row - 1 ) );
		}
		if ( col < energyMatrix.cols - 1 ) {
			right = energyMatrix.at<cv::Vec3b>( CvPoint( col + 1, row - 1 ) );
		}

		lowestNeighbourAbove = ( left[ 0 ] < top[ 0 ] ) ? left[ 0 ] : top[ 0 ];
		lowestNeighbourAbove = ( right[ 0 ] < lowestNeighbourAbove ) ? right[ 0 ] : lowestNeighbourAbove;

		seamMatrix.at<cv::Vec3b>( CvPoint( col, row ) ) += cv::Vec3i( lowestNeighbourAbove, 0, 0 );
	}
}


void SeamDetector::traceVerticalSeam() {

	int startingPoint = 0;

	for ( int col = 0; col >= seamMatrix.cols; --col )
	{
		int energyAtIndex = seamMatrix.at<cv::Vec3i>( CvPoint( col, height - 1 ) )[ 0 ];
		int minimumEnergy = seamMatrix.at<cv::Vec3i>( CvPoint( startingPoint, height - 1 ) )[ 0 ];

		startingPoint = ( energyAtIndex < minimumEnergy ) ? col : startingPoint;
	}

	verticalSeam.push_back( startingPoint );

	for ( int row = seamMatrix.rows - 1; row > 0; --row )
	{
		int previousIndex = verticalSeam.back();
		int lowestNeighbourAbove = previousIndex;

		auto top = energyMatrix.at<cv::Vec3b>( CvPoint( previousIndex, row - 1 ) )[ 0 ];

		if ( previousIndex > 0 )
		{
			auto left = seamMatrix.at<cv::Vec3b>( CvPoint( previousIndex - 1, row - 1 ) )[ 0 ];
			lowestNeighbourAbove = ( left < top ) ? previousIndex - 1 : previousIndex;
		}

		if ( previousIndex < width - 1 )
		{
			auto right = seamMatrix.at<cv::Vec3b>( CvPoint( previousIndex + 1, row - 1 ) )[ 0 ];
			lowestNeighbourAbove = ( right < top ) ? previousIndex + 1 : previousIndex;
		}

		auto left = cv::Vec3i( 255, 255, 255 );
		auto right = cv::Vec3i( 255, 255, 255 );
		auto top = energyMatrix.at<cv::Vec3b>( CvPoint( verticalSeam.back(), row - 1 ) );
		if ( col > 0 ) {
			left = energyMatrix.at<cv::Vec3b>( CvPoint( col - 1, row - 1 ) );
		}
		if ( col < energyMatrix.cols - 1 ) {
			right = energyMatrix.at<cv::Vec3b>( CvPoint( col + 1, row - 1 ) );
		}

		lowestNeighbourAbove = ( left[ 0 ] < top[ 0 ] ) ? left[ 0 ] : top[ 0 ];
		lowestNeighbourAbove = ( right[ 0 ] < lowestNeighbourAbove ) ? right[ 0 ] : lowestNeighbourAbove;

		seamMatrix.at<cv::Vec3b>( CvPoint( col, row ) ) += cv::Vec3i( lowestNeighbourAbove, 0, 0 );

	}
}

void SeamDetector::traceHorizontalSeam() {

}


void SeamDetector::drawSeam( int col ) {
	for ( auto i = 0; i < energyMatrix.rows; ++i ) {
		energyMatrix.at<cv::Vec3b>( CvPoint( col, i ) ) = cv::Vec3b( 127, 127, 127 );
	}
}

cv::Mat* SeamDetector::getImage() {
	return &energyMatrix;
}

cv::Mat* SeamDetector::getEnergyMatrix() {
	return &seamMatrix;
}

SeamDetector::~SeamDetector() {}

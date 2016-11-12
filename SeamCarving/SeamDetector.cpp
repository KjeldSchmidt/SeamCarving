#include "stdafx.h"


SeamDetector::SeamDetector( int inputWidth, int inputHeight ) : width( inputWidth ), height( inputHeight ) {
	energyMatrix = *new cv::Mat( height, width, CV_8SC3 );
	seamMatrix = energyMatrix;
	verticalSeam.reserve( inputHeight );
	horizontalSeam.reserve( inputWidth );
}

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	energyMatrix = *new cv::Mat( originalImage );
	seamMatrix = energyMatrix;
	width = energyMatrix.cols;
	height = energyMatrix.rows;
	verticalSeam.reserve( height );
	horizontalSeam.reserve( width );
}

void SeamDetector::findSeam() {
	for ( auto i = 1; i < seamMatrix.rows; ++i ) {
		iterateSeam( i );
	}
}


void SeamDetector::iterateSeam( int row ) {
	for ( auto col = 0; col < energyMatrix.cols; ++col ) {
		auto lowestNeighbourAbove = 0;
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

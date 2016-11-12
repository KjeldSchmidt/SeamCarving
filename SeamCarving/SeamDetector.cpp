#include "stdafx.h"


SeamDetector::SeamDetector( int inputWidth, int inputHeight ) : width( inputWidth ), height( inputHeight ) {
	energyMatrix = new cv::Mat( height, width, CV_8SC3 );
}

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	energyMatrix = new cv::Mat( originalImage );
}

void SeamDetector::findSeam() {
	seamMatrix = &((*energyMatrix).clone());
	for ( int i = 1; i < ( *seamMatrix ).rows; ++i ) {
		iterateSeam( i );
	}
}


/*
 * There is a good chance this function is broken,
 * because we just try to assign Vec3i
 * and I'm not sure the Mat is initialized properly for that.
 */
void SeamDetector::iterateSeam( int row ) {
	for ( int col = 0; col < ( *energyMatrix ).cols; ++col ) {
		int direction = 0;
		int lowestNeighbourAbove = 0;
		cv::Vec3i left = cv::Vec3i( 255, 255, 255 );
		cv::Vec3i right = cv::Vec3i( 255, 255, 255 );
		cv::Vec3i top = ( *energyMatrix ).at<cv::Vec3b>( CvPoint( col, row - 1 ) );
		if ( col > 0 ) {
			left = ( *energyMatrix ).at<cv::Vec3b>( CvPoint( col - 1, row - 1 ) );
		}
		if ( col < ( *energyMatrix ).cols - 1 ) {
			right = ( *energyMatrix ).at<cv::Vec3b>( CvPoint( col + 1, row - 1 ) );
		}

		lowestNeighbourAbove = ( left[ 0 ] < top[ 0 ] ) ? left[ 0 ] : top[ 0 ];
		lowestNeighbourAbove = ( right[ 0 ] < lowestNeighbourAbove ) ? right[0] : lowestNeighbourAbove;

		( *seamMatrix ).at<cv::Vec3b>( CvPoint( col, row ) ) += cv::Vec3i( lowestNeighbourAbove, 0, 0 );
	}
}

void SeamDetector::drawSeam( int col ) {
	for ( int i = 0; i < ( *energyMatrix ).rows; ++i ) {
		( *energyMatrix ).at<cv::Vec3b>( CvPoint( col, i ) ) = cv::Vec3b( 127, 127, 127 );
	}
}

cv::Mat* SeamDetector::getImage() {
	return energyMatrix;
}

cv::Mat* SeamDetector::getEnergyMatrix() {
	return seamMatrix;
}

SeamDetector::~SeamDetector() {}

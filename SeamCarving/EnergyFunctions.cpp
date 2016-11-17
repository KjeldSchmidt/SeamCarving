#include "stdafx.h"
#include "EnergyFunctions.h"


EnergyFunctions::EnergyFunctions() {}
EnergyFunctions::~EnergyFunctions() {}

cv::Mat EnergyFunctions::DirectionIndependentSobel( const cv::Mat& image ) {
	int height = image.rows;
	int width = image.cols;

	cv::Mat lumaMap = cv::Mat( height, width, CV_8UC1 );
	cv::Mat gx = cv::Mat( height, width, CV_32SC1 );
	cv::Mat gy = cv::Mat( height, width, CV_32SC1 );
	cv::Mat combined = cv::Mat( height, width, CV_8UC1 );

	lumaFromBGR( image, lumaMap );
	SobelX( lumaMap, gx );
	SobelY( lumaMap, gy );
	combineSobelDirections( gx, gy, combined );

	return combined;
}

void EnergyFunctions::lumaFromBGR( cv::Mat const &source, cv::Mat &destination ) {
	const cv::Vec3b *sourceRowPointer;
	unsigned char *destRowPointer;

	int cols = source.cols;
	int rows = source.rows;

	cv::Vec3b coloredPixel;
	for ( int row = 0; row < rows; ++row ) {
		sourceRowPointer = source.ptr<cv::Vec3b>( row );
		destRowPointer = destination.ptr< unsigned char>( row );
		for ( int col = 0; col < cols; ++col ) {
			coloredPixel = sourceRowPointer[ col ];
			// According to http://stackoverflow.com/a/596241
			destRowPointer[ col ] = ( coloredPixel[ 0 ] + coloredPixel[ 1 ] + +coloredPixel[ 1 ] + +coloredPixel[ 1 ] + coloredPixel[ 2 ] + coloredPixel[ 2 ] ) / 6;
		}
	}
}

/*
 * NOTE: smoothLumaMap() is not currently used, but I left it in code because it sometimes produced interesting results.
 */

cv::Mat EnergyFunctions::smoothLumaMap( const cv::Mat & source ) {

	int height = source.rows;
	int width = source.cols;

	cv::Mat smoothLuma( height, width, CV_8UC1 );

	const unsigned char *top, *mid, *bot;
	unsigned char *rowSmooth;
	int sum;

	for ( int row = 1; row < source.rows - 1; ++row ) {
		top = source.ptr<unsigned char>( row - 1 );
		mid = source.ptr<unsigned char>( row );
		bot = source.ptr<unsigned char>( row + 1 );
		rowSmooth = smoothLuma.ptr<unsigned char>( row );

		for ( int col = 1; col < source.cols - 1; ++col ) {

			sum =
				top[ col - 1 ] + top[ col ] + top[ col + 1 ] +
				mid[ col - 1 ] + mid[ col ] + mid[ col + 1 ] +
				bot[ col - 1 ] + bot[ col ] + bot[ col + 1 ];
			sum = sum / 9;

			rowSmooth[ col ] = sum;
		}
	}

	return smoothLuma;
}

void EnergyFunctions::SobelX( cv::Mat const &source, cv::Mat &destination ) {
	const unsigned char *rowTop, *rowMid, *rowBot;
	unsigned char tl, ml, bl, tr, mr, br;
	int sum;

	for ( int row = 1; row < source.rows - 1; ++row ) {
		rowTop = source.ptr<unsigned char>( row - 1 );
		rowMid = source.ptr<unsigned char>( row );
		rowBot = source.ptr<unsigned char>( row + 1 );
		for ( int col = 1; col < source.cols - 1; ++col ) {
			tl = rowTop[ col - 1 ];
			ml = rowMid[ col - 1 ];
			bl = rowBot[ col - 1 ];
			tr = rowTop[ col + 1 ];
			mr = rowMid[ col + 1 ];
			br = rowBot[ col + 1 ];

			sum = tl + ml * 2 + bl - tr - mr * 2 - br;
			sum = sum;

			destination.at<int>( CvPoint( col, row ) ) = sum;
		}
	}
}

void EnergyFunctions::SobelY( cv::Mat const &source, cv::Mat &destination ) {
	const unsigned char *rowTop, *rowBot;
	unsigned char lt, mt, rt, lb, mb, rb;
	int sum;

	for ( int row = 1; row < source.rows - 1; ++row ) {
		rowTop = source.ptr<unsigned char>( row - 1 );
		rowBot = source.ptr<unsigned char>( row + 1 );
		for ( int col = 1; col < source.cols - 1; ++col ) {
			lt = rowTop[ col - 1 ];
			mt = rowTop[ col ];
			rt = rowTop[ col + 1 ];
			lb = rowBot[ col - 1 ];
			mb = rowBot[ col ];
			rb = rowBot[ col + 1 ];

			sum = lt + mt * 2 + rt - lb - mb * 2 - rb;

			destination.at<int>( CvPoint( col, row ) ) = sum;
		}
	}
}

void EnergyFunctions::combineSobelDirections( cv::Mat const gx, cv::Mat const gy, cv::Mat output ) {
	const int *xRow, *yRow;
	unsigned char *outRow;
	int sum;


	for ( int row = 0; row < output.rows; ++row ) {
		xRow = gx.ptr<int>( row );
		yRow = gy.ptr<int>( row );
		outRow = output.ptr<unsigned char>( row );

		for ( int col = 0; col < output.cols; ++col ) {
			sum = std::abs( xRow[ col ] ) + std::abs( yRow[ col ] );
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;

			outRow[ col ] = sum;
		}
	}
}
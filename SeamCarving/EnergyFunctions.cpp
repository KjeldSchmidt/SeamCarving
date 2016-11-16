#include "stdafx.h"
#include "EnergyFunctions.h"


EnergyFunctions::EnergyFunctions() {}


EnergyFunctions::~EnergyFunctions() {}

cv::Mat EnergyFunctions::StupidBrightness( const cv::Mat& image ) {
	int height = image.rows;
	int width = image.cols;
	cv::Mat lumaMap = cv::Mat( height, width, CV_8UC1 );

	const cv::Vec3b *imageRow;
	unsigned char *mapRow;

	for ( int row = 0; row < height; ++row ) {
		imageRow = image.ptr<cv::Vec3b>( row );
		mapRow = lumaMap.ptr<unsigned char>( row );
		for ( int col = 0; col < width; ++col ) {
			mapRow[ col ] = ( imageRow[col][ 0 ] + imageRow[col][ 1 ] + imageRow[col][ 2 ] ) / 3;
		}
	}

	return lumaMap;
}

cv::Mat EnergyFunctions::DirectionIndependentSorbel( const cv::Mat& image ) {
	int height = image.rows;
	int width = image.cols;
	cv::Mat lumaMap = cv::Mat( height, width, CV_8UC1 );
	cv::Mat gx = cv::Mat( height, width, CV_8SC1 );
	cv::Mat gy = cv::Mat( height, width, CV_8SC1 );
	lumaFromBGR( image, lumaMap );
	SorbelX( lumaMap, gx );
	SorbelY( lumaMap, gy );

	return gx;
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

void EnergyFunctions::SorbelX( cv::Mat const &source, cv::Mat &destination ) {
	const unsigned char *rowTop;
	const unsigned char *rowMid;
	const unsigned char *rowBot;

	unsigned char tl;
	unsigned char ml;
	unsigned char bl;
	unsigned char tr;
	unsigned char mr;
	unsigned char br;

	int kernelPixel;

	for ( int row = 1; row < source.rows - 1; ++row ) {
		rowTop = source.ptr<unsigned char>( row - 1 );
		rowMid = source.ptr<unsigned char>( row );
		rowBot = source.ptr<unsigned char>( row + 1 );
		for ( int col = 1; col < source.cols - 1; ++col ) {
			tl = rowTop[ col - 1 ] * 1;
			ml = rowMid[ col - 1 ] * 2;
			bl = rowBot[ col - 1 ] * 1;
			tr = rowTop[ col + 1 ] * -1;
			mr = rowMid[ col + 1 ] * -2;
			br = rowBot[ col + 1 ] * -1;

			kernelPixel = ( tl + ml + bl + tr + mr + br ) / 8;

			destination.at<char>( CvPoint( col, row ) ) = kernelPixel;
		}
	}
}

void EnergyFunctions::SorbelY( cv::Mat const &source, cv::Mat &destination ) {
	const unsigned char *rowTop;
	const unsigned char *rowBot;

	unsigned char lt;
	unsigned char mt;
	unsigned char rt;
	unsigned char lb;
	unsigned char mb;
	unsigned char rb;

	int kernelPixel;

	for ( int row = 1; row < source.rows - 1; ++row ) {
		rowTop = source.ptr<unsigned char>( row - 1 );
		rowBot = source.ptr<unsigned char>( row + 1 );
		for ( int col = 1; col < source.cols - 1; ++col ) {
			lt = rowTop[ col - 1 ] * 1;
			mt = rowTop[ col ] * 2;
			rt = rowTop[ col + 1 ] * 1;
			lb = rowBot[ col - 1 ] * -1;
			mb = rowBot[ col ] * -2;
			rb = rowBot[ col + 1 ] * -1;

			kernelPixel = ( lt + mt + rt + lb + mb + rb ) / 8;

			destination.at<char>( CvPoint( col, row ) ) = kernelPixel;
		}
	}
}

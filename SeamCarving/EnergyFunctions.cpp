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
			mapRow[ col ] = ( imageRow[ col ][ 0 ] + imageRow[ col ][ 1 ] + imageRow[ col ][ 2 ] ) / 3;
		}
	}

	return lumaMap;
}

cv::Mat EnergyFunctions::DirectionIndependentSobel( const cv::Mat& image ) {
	int height = image.rows;
	int width = image.cols;
	cv::Mat lumaMap = cv::Mat( height, width, CV_8UC1 );
	cv::Mat gx = cv::Mat( height, width, CV_32SC1 );
	cv::Mat gy = cv::Mat( height, width, CV_32SC1 );
	cv::Mat directionIndependentMat = cv::Mat( height, width, CV_8UC1 );

	lumaFromBGR( image, lumaMap );
	SobelX( lumaMap, gx );
	SobelY( lumaMap, gy );
	combineSobelDirections( gx, gy, directionIndependentMat );

	return directionIndependentMat;
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

void EnergyFunctions::SobelX( cv::Mat const &source, cv::Mat &destination ) {
	const unsigned char *rowTop, *rowMid, *rowBot;
	unsigned char tl, ml, bl, tr, mr, br;
	int sum;

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

			sum = tl + ml + bl + tr + mr + br;
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
			lt = rowTop[ col - 1 ] * 1;
			mt = rowTop[ col ] * 2;
			rt = rowTop[ col + 1 ] * 1;
			lb = rowBot[ col - 1 ] * -1;
			mb = rowBot[ col ] * -2;
			rb = rowBot[ col + 1 ] * -1;

			sum = lt + mt + rt + lb + mb + rb;
			sum = sum;

			destination.at<int>( CvPoint( col, row ) ) = sum;
		}
	}
}

void EnergyFunctions::combineSobelDirections( cv::Mat const gx, cv::Mat const gy, cv::Mat output ) {
	const int *xRow, *yRow;
	unsigned char *outRow;
	int sum;
	char truncatedSum;


	for ( int row = 0; row < output.rows; ++row ) {
		xRow = gx.ptr<int>( row );
		yRow = gy.ptr<int>( row );
		outRow = output.ptr<unsigned char>( row );

		for ( int col = 0; col < output.cols; ++col ) {
			sum = std::abs( xRow[ col ] ) + std::abs( yRow[ col ] );
			if ( sum > 255 ) {
				truncatedSum = 255;
			} else if ( sum < 0 ) {
				truncatedSum = 0;
			} else {
				truncatedSum = static_cast< unsigned char >( sum );
			}

			outRow[ col ] = truncatedSum;
		}
	}
}

// Computes the x component of the gradient vector
// at a given point in a image.
// returns gradient in the x direction
int xGradient( cv::Mat image, int x, int y ) {
	return image.at<uchar>( y - 1, x - 1 ) +
		2 * image.at<uchar>( y, x - 1 ) +
		image.at<uchar>( y + 1, x - 1 ) -
		image.at<uchar>( y - 1, x + 1 ) -
		2 * image.at<uchar>( y, x + 1 ) -
		image.at<uchar>( y + 1, x + 1 );
}

// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction

int yGradient( cv::Mat image, int x, int y ) {
	return image.at<uchar>( y - 1, x - 1 ) +
		2 * image.at<uchar>( y - 1, x ) +
		image.at<uchar>( y - 1, x + 1 ) -
		image.at<uchar>( y + 1, x - 1 ) -
		2 * image.at<uchar>( y + 1, x ) -
		image.at<uchar>( y + 1, x + 1 );
}

cv::Mat EnergyFunctions::sample( const cv::Mat &image ) {

	cv::Mat src, dst;
	int gx, gy, sum;

	// Load an image
	cv::Mat lumaMap = cv::Mat( image.rows, image.cols, CV_8UC1 );
	lumaFromBGR( image, lumaMap );
	dst = lumaMap.clone();
	
	for ( int y = 0; y < lumaMap.rows; y++ )
		for ( int x = 0; x < lumaMap.cols; x++ )
			dst.at<uchar>( y, x ) = 0.0;

	for ( int y = 1; y < lumaMap.rows - 1; y++ ) {
		for ( int x = 1; x < lumaMap.cols - 1; x++ ) {
			gx = xGradient( lumaMap, x, y );
			gy = yGradient( lumaMap, x, y );
			sum = abs( gx ) + abs( gy );
			sum = sum / 4;
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;
			dst.at<uchar>( y, x ) = sum;
		}
	}

	return dst;
}
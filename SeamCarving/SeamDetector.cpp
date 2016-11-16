#include "stdafx.h"
#include "EnergyFunctions.h"

SeamDetector::SeamDetector( cv::Mat &originalImage ) {
	originalImageMatrix = *new cv::Mat( originalImage );
	width = originalImageMatrix.cols;
	height = originalImageMatrix.rows;

	energyMatrix = *new cv::Mat( height, width, CV_8SC1 );
	seamMatrix = *new cv::Mat( height, width, CV_32SC1 );
}

/*
 * Dummy function - works by stupidity, not any actual energy.
 * TODO: IMPLEMENT ACTUAL ENERGY FUNCTION
 */
void SeamDetector::prepareEnergyMatrix() {

	energyMatrix = EnergyFunctions::DirectionIndependentSorbel( originalImageMatrix );
	energyMatrixIsSet = true;
}

void SeamDetector::prepareSeamMatrix()
{
	char *energyRow;
	int *seamRow;

	for ( int row = 0; row < height; ++row )
	{
		energyRow = energyMatrix.ptr<char>( row );
		seamRow = seamMatrix.ptr<int>( row );
		for ( int col = 0; col < width; ++col ) {
			seamRow[ col ] = energyRow[ col ];
		}
	}
}

void SeamDetector::findSeam()
{

	if ( !energyMatrixIsSet )
	{
		prepareEnergyMatrix();
	}

	verticalSeam.clear();

	prepareSeamMatrix();

	for ( auto i = 1; i < height; ++i ) {
		iterateSeamMatrix( i );
	}

	traceSeam();
}

void SeamDetector::iterateSeamMatrix( int row ) {
	for ( auto col = 0; col < width; ++col ) {
		int lowestNeighbourAbove = 0;
		int left = MAX_INT;
		int right = MAX_INT;
		int *rowData = seamMatrix.ptr<int>( row - 1 );

		int top = rowData[ col ];
		if ( col > 0 ) {
			left = rowData[ col - 1 ];
		}
		if ( col < width - 1 ) {
			right = rowData[ col + 1 ];
		}

		lowestNeighbourAbove = ( left < top ) ? left : top;
		lowestNeighbourAbove = ( right < lowestNeighbourAbove ) ? right : lowestNeighbourAbove;

		seamMatrix.ptr<int>( row )[ col ] += lowestNeighbourAbove;
	}
}

void SeamDetector::traceSeam() {
	findSeamStartingPoint();


	for ( int row = height - 2; row > 0; --row ) {
		iterateSeam( row );
	}
}

void SeamDetector::findSeamStartingPoint() {
	int startingPoint = 0;
	int minimumEnergy = MAX_INT;
	for ( int col = 0; col < seamMatrix.cols; ++col ) {
		int energyAtIndex = seamMatrix.at<int>( CvPoint( col, height - 1 ) );
		if ( energyAtIndex < minimumEnergy ) {
			startingPoint = col;
			minimumEnergy = energyAtIndex;
		}
	}

	verticalSeam.push_back( startingPoint );
}

void SeamDetector::iterateSeam( int row ) {
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

void SeamDetector::drawSeam()
{
	auto p = verticalSeam.begin();
	int rowIndex = height - 1;
	for ( ; p != verticalSeam.end(); ++p, --rowIndex ) {
		originalImageMatrix.at<cv::Vec3b>( CvPoint( *p, rowIndex ) ) = cv::Vec3b( 255, 0, 0 );
	}
}

void SeamDetector::removeSeam()
{
	int rowIndex = height - 1;

	std::cout << "Height: " << height << " image.rows" << originalImageMatrix.rows << " vector length: " << verticalSeam.size() << std::endl;

	for ( auto p = verticalSeam.begin(); p != verticalSeam.end(); ++p ) {
		auto *imagePointer = originalImageMatrix.ptr<cv::Vec3b>( rowIndex );
		int *energyPointer = energyMatrix.ptr<int>( rowIndex );

		for ( int colIndex = *p; colIndex < width - 1; ++colIndex ) {
			imagePointer[ colIndex ] = imagePointer[ colIndex + 1 ];
			energyPointer[ colIndex ] = energyPointer[ colIndex + 1 ];
		}

		imagePointer[ width ] = cv::Vec3b( 0, 0, 0 );
		energyPointer[ width ] = MAX_INT;

		--rowIndex;
	}

	--width;
}


void SeamDetector::findVerticalSeam() {
	if ( currentlyTransposed ) {
		transpose();
	}

	findSeam();
}

void SeamDetector::drawVerticalSeam() {
	if ( currentlyTransposed ) {
		transpose();
	}

	drawSeam();
}

void SeamDetector::removeVerticalSeam() {
	if ( currentlyTransposed ) {
		transpose();
	}

	removeSeam();
}


void SeamDetector::findHorizontalSeam() {
	if ( !currentlyTransposed ) {
		transpose();
	}

	findSeam();
}

void SeamDetector::drawHorizontalSeam() {
	if ( !currentlyTransposed ) {
		transpose();
	}

	drawSeam();
}

void SeamDetector::removeHorizontalSeam() {
	if ( !currentlyTransposed ) {
		transpose();
	}

	removeSeam();
}


void SeamDetector::transpose() {
	energyMatrix = energyMatrix.t();
	originalImageMatrix = originalImageMatrix.t();
	seamMatrix = seamMatrix.t();

	int temp = width;
	width = height;
	height = temp;

	currentlyTransposed = !currentlyTransposed;
}

cv::Mat* SeamDetector::getImage() {
	return &originalImageMatrix;
}

void SeamDetector::setCorrectOrientation() {
	if ( currentlyTransposed ) {
		transpose();
	}
}

cv::Mat* SeamDetector::getEnergyMatrix() {
	return &energyMatrix;
}

cv::Mat* SeamDetector::getSeamMatrix() {
	return &seamMatrix;
}

int SeamDetector::getHeight() const {
	return height;
}

int SeamDetector::getWidth() const {
	return width;
}

SeamDetector::~SeamDetector() {}

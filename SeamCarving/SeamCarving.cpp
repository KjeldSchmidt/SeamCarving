// SeamCarving.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::cout << "OpenCV version : " << CV_VERSION << std::endl;

	ImageDisplay window("Test Window");

	cv::Mat image = ImageReader::readImage("testimage.jpg");


	if (!image.empty()) {
		std::cout << "Image loaded";
		window.showImage(image);
		cv::waitKey(1);
	}
	else {
		std::cout << "Empty image";
	}

	system("pause");

    return 0;
}

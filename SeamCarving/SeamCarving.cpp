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
	}
	else {
		std::cout << "Empty image";
	}

	window.DrawLine(100);

	system("pause");

    return 0;
}

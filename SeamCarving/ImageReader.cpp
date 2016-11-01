#include "stdafx.h"

cv::Mat ImageReader::readImage(const std::string& filePath)
{
	return cv::imread(filePath);
}

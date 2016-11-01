#pragma once

class ImageDisplay
{
private:
	cv::Mat openImage;
	cv::Mat displayedImage;
	std::string name;
public:
	void showImage( cv::Mat& );
	ImageDisplay(std::string name);
	~ImageDisplay();
};


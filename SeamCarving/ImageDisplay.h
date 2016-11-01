#pragma once

class ImageDisplay
{
private:
	cv::Mat openImage;
	cv::Mat displayedImage;
	std::string name;
public:
	void showImage( cv::Mat& );
	void refresh();
	void DrawLine(int col);
	ImageDisplay(std::string name);
	~ImageDisplay();
};


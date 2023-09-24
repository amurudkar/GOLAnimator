#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

class GOLAnimator {
public:
	GOLAnimator(cv::Size resolution = cv::Size(25, 25)) : mResolution(resolution) {};

	void setResolution(cv::Size resolution);

	bool update();

	void reset();

	const cv::Mat& getFrame();

private:
	int getNeighborCount(const cv::Mat& frame, int row, int col);

	bool matIsEqual(const cv::Mat& mat1, const cv::Mat& mat2);

	cv::Size mResolution;

	cv::Mat mMask;
	cv::Mat mFrame;
	cv::Mat mWhite;
	std::deque<cv::Mat> mOldMasks;

};
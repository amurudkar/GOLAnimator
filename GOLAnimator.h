// GOLAnimator.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

#include "opencv2/opencv.hpp"

class GOLAnimator {
public:
	GOLAnimator(cv::Size resolution = cv::Size(25, 25)) : mResolution(resolution) {};

	void setResolution(cv::Size resolution);

	bool run();

private:
	int getNeighborCount(const cv::Mat& frame, int row, int col);

	bool matIsEqual(const cv::Mat& mat1, const cv::Mat& mat2);

	cv::Size mResolution;
};
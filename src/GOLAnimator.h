#pragma once

#include <iostream>
#include "opencv2/opencv.hpp"

class GOLAnimator {
public:
	GOLAnimator(const cv::Size &resolution = cv::Size(25, 25));

	void setResolution(const cv::Size &resolution);

	bool update();

	void setInvertColors(bool invert);

	bool getInvertColors();

	void reset();

	const cv::Mat& getFrame();

private:
	bool matIsEqual(const cv::Mat& mat1, const cv::Mat& mat2);

	unsigned char getColorBg();
	unsigned char getColorFg();

	cv::Size mResolution;

	cv::Mat mMask;
	cv::Mat mTempMask;
	cv::Mat mFrame, mSum;
	cv::Mat mLifeColor;
	cv::Mat mSummationKernel;

	std::deque<cv::Mat> mOldMasks;

	bool mInvertColors;
	bool mIsFinished;

	int mMaxPeriodicity;
};
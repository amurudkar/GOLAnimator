// GOLAnimator.cpp : Defines the entry point for the application.

#include "GOLAnimator.h"
#include <deque>

using namespace std;
using namespace cv;

void GOLAnimator::setResolution(cv::Size resolution)
{
	mResolution = resolution;
	reset();
}

bool GOLAnimator::update()
{
	Mat newMask;
	
	if (mOldMasks.size() == 2)
		mOldMasks.pop_front();

	mOldMasks.push_back(mMask.clone());

	mFrame.setTo(0);
	newMask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	for (int i = 0; i < mMask.rows; ++i)
		for (int j = 0; j < mMask.cols; ++j) {
			auto sum = getNeighborCount(mMask, i, j);
			const auto cElem = mMask.at<unsigned char>(i, j);
			auto& nElem = newMask.at<unsigned char>(i, j);
			
			if (sum <= 1 || sum >= 4)
				nElem = 0;
			else if (sum == 3)
				nElem = 1;
			else if (sum == 2)
				nElem = cElem;
		}

	swap(mMask, newMask);

	mWhite.copyTo(mFrame, mMask);

	auto matchF = matIsEqual(mMask, mOldMasks.front());
	auto matchB = matIsEqual(mMask, mOldMasks.back());

	auto hasBlinkers = (mOldMasks.size() == 2) && matchF;
	auto isStable = ((mOldMasks.size() == 1) && matchF) || matchB;

	if (hasBlinkers || isStable) {
		cout << (hasBlinkers ? "Blinkers!" : "") << (isStable ? "Still life!" : "") << endl;
		return false;
	}

	return true;
}

void GOLAnimator::reset()
{
	mMask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	mFrame = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	mWhite = Mat::ones(mResolution.width, mResolution.width, CV_8UC1) * 255;

	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < mMask.rows; ++i)
		for (int j = 0; j < mMask.cols; ++j)
			mMask.at<unsigned char>(i, j) = static_cast<unsigned char>(rand() % 2);

	mWhite.copyTo(mFrame, mMask);
	mOldMasks.clear();
}

const cv::Mat &GOLAnimator::getFrame()
{
	return mFrame;
}

int GOLAnimator::getNeighborCount(const cv::Mat & frame, int row, int col)
{
	int sum = 0;
	int starti = max(0, row - 1);
	int endi = min(frame.rows - 1, row  + 1);
	int startj = max(0, col - 1);
	int endj = min(frame.cols - 1, col + 1);

	for (int i = starti; i <= endi; ++i)
		for (int j = startj; j <= endj; ++j)
			sum += static_cast<int>(frame.at<unsigned char>(i, j));

	sum -= static_cast<int>(frame.at<unsigned char>(row, col));
	return sum;
}

bool GOLAnimator::matIsEqual(const cv::Mat & mat1, const cv::Mat & mat2)
{
	cv::Mat diff = mat1 != mat2;
	auto d = countNonZero(diff);
	return d == 0;
}

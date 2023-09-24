// GOLAnimator.cpp : Defines the entry point for the application.

#include "GOLAnimator.h"
#include <deque>

using namespace std;
using namespace cv;

GOLAnimator::GOLAnimator(const Size &resolution) :
mMaxPeriodicity(5)
{
	setResolution(resolution);
	setInvertColors(false);
	mSummationKernel = Mat::ones(3, 3, CV_8UC1);
};


void GOLAnimator::setResolution(const Size &resolution)
{
	mResolution = resolution;
	reset();
}

void GOLAnimator::setInvertColors(bool invert) {
	invertColors = invert;
	reset();
}

bool GOLAnimator::getInvertColors()
{
    return invertColors;
}

bool GOLAnimator::update()
{	
	if (mOldMasks.size() == mMaxPeriodicity)
		mOldMasks.pop_front();

	mOldMasks.emplace_back(mMask.clone());

	mFrame.setTo(invertColors ? 255 : 0);
	mTempMask.setTo(0);

	filter2D(mMask, mSum, -1, mSummationKernel);
	mSum -= mMask;
	mTempMask.setTo(0, (mSum <= 1) + (mSum >= 4));
	mTempMask.setTo(1, mSum == 3);
	mMask.copyTo(mTempMask, mSum == 2);

	swap(mMask, mTempMask);
	mLifeColor.copyTo(mFrame, mMask);

	auto match = false;
	auto periodicity = 0;

	for(int i = mOldMasks.size() - 1; i >= 0; --i) {
		match |= matIsEqual(mMask, mOldMasks[i]);
		if(match) {
			periodicity = mOldMasks.size() - i;
			break;
		}
 	}

	auto hasBlinkers = match && (periodicity > 1);
	auto isStable = match && (periodicity == 1);

	if (hasBlinkers || isStable) {
		cout << (hasBlinkers ? "Blinkers, max periodicity = " + to_string(periodicity) + ". ": "");
		cout << (isStable ? "Still life!" : "") << endl;
		return false;
	}

	return true;
}

void GOLAnimator::reset()
{
	mMask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	mTempMask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	mFrame = Mat::ones(mResolution.width, mResolution.width, CV_8UC1) * (invertColors ? 255 : 0);
	mLifeColor = Mat::ones(mResolution.width, mResolution.width, CV_8UC1) * (invertColors ? 0 : 255);

	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < mMask.rows; ++i)
		for (int j = 0; j < mMask.cols; ++j)
			mMask.at<unsigned char>(i, j) = static_cast<unsigned char>(rand() % 2);

	mLifeColor.copyTo(mFrame, mMask);
	mOldMasks.clear();
}

const Mat &GOLAnimator::getFrame()
{
	return mFrame;
}

bool GOLAnimator::matIsEqual(const Mat & mat1, const Mat & mat2)
{
	return countNonZero(mat1 != mat2) == 0;
}

// GOLAnimator.cpp : Defines the entry point for the application.
//

#include "GOLAnimator.h"
#include <deque>
#include <random>

using namespace std;
using namespace cv;

int main()
{
	cout << "Hello GOL." << endl;

	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(25, 200); // define the range

	auto g = GOLAnimator();

	while (true) {
		auto a = distr(eng);
		g.setResolution(Size(a, a));
		g.run();
	}
	return 0;
}

void GOLAnimator::setResolution(cv::Size resolution)
{
	mResolution = resolution;
}

bool GOLAnimator::run()
{
	Mat mask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	Mat frame = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
	Mat white = Mat::ones(mResolution.width, mResolution.width, CV_8UC1) * 255;
	
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < mask.rows; ++i)
		for (int j = 0; j < mask.cols; ++j)
			mask.at<unsigned char>(i, j) = static_cast<unsigned char>(rand() % 2);

	white.copyTo(frame, mask);

	string winName = "GOL";
	namedWindow(winName, WINDOW_KEEPRATIO);
	resizeWindow(winName, 600, 600);
	
	deque<Mat> oldMasks;
	Mat newMask;
	while (true) {
		if (oldMasks.size() == 2)
			oldMasks.pop_front();

		oldMasks.push_back(mask.clone());

		frame = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
		newMask = Mat::zeros(mResolution.width, mResolution.width, CV_8UC1);
		for (int i = 0; i < mask.rows; ++i)
			for (int j = 0; j < mask.cols; ++j) {
				auto sum = getNeighborCount(mask, i, j);
				const auto cElem = mask.at<unsigned char>(i, j);
				auto& nElem = newMask.at<unsigned char>(i, j);
				
				if (sum <= 1 || sum >= 4)
					nElem = 0;
				else if (sum == 3)
					nElem = 1;
				else if (sum == 2)
					nElem = cElem;
			}

		swap(mask, newMask);

		white.copyTo(frame, mask);
		imshow(winName, frame);

		auto k = waitKey(33);
		if (k == 'q')
			return false;

		auto matchF = matIsEqual(mask, oldMasks.front());
		auto matchB = matIsEqual(mask, oldMasks.back());

		auto hasBlinkers = (oldMasks.size() == 2) && matchF;
		auto isStable = ((oldMasks.size() == 1) && matchF) || matchB;

		if (hasBlinkers || isStable) {
			cout << (hasBlinkers ? "Blinkers!" : "") << (isStable ? "Stable!" : "") << endl;
			waitKey(2000);
			return true;
		}
	}
	return false;
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

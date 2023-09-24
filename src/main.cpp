#include "GOLAnimator.h"
#include <random>

using namespace std;
using namespace cv;

int main()
{
	cout << "Hello GOL." << endl;

	// From https://stackoverflow.com/a/19728404/22523947
	random_device rd; // obtain a random number from hardware
	mt19937 eng(rd()); // seed the generator
	uniform_int_distribution<> distr(25, 200); // define the range

	int dispWidth = 600;
	int dispHeight = 600;
	string winName = "GOL";
	namedWindow(winName, WINDOW_KEEPRATIO);
	resizeWindow(winName, dispWidth, dispHeight);
	auto g = GOLAnimator();

	auto res = false;
	auto counter = 0;
	auto countingDown = false;

	cv::TickMeter tm;
	size_t frameCnt = 0;

	while (true) {
		if(!res && (!countingDown || (countingDown && counter == 0))) {
			g.reset();
			countingDown = false;
			cout << "Update avg.: " << tm.getAvgTimeMilli() << " ms" << endl;
		}
		else if(countingDown && counter) {
			--counter;
			cout << "Countdown " << counter << endl;
		}

		tm.start();
		res = g.update();
		tm.stop();

		if(!res && !countingDown) {
			counter = 4; // 2 frames
			countingDown = true;
		}

		auto frame = g.getFrame();
		auto waitTime = res ? 33 : 500;

		imshow(winName, frame);
		auto k = waitKey(waitTime);
		if(k == 'q')
			break;
		else if (k == 'r')
			res = false;
		else if (k == 'c') {
			auto s = distr(eng);
			g.setResolution(Size(s, s));
		}
		else if (k == 'i') {
			g.setInvertColors(!g.getInvertColors());
		}
	}

	cout << "Update avg.: " << tm.getAvgTimeMilli() << " ms" << endl;
	return 0;
}

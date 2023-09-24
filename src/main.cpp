#include "GOLAnimator.h"
#include <random>

using namespace std;
using namespace cv;

int main()
{
	cout << "Hello GOL." << endl;

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
	while (true) {
		if(!res) {
			g.reset();
		}

		res = g.update();
		auto frame = g.getFrame();
		auto waitTime = res ? 33 : 2000;

		imshow(winName, frame);
		auto k = waitKey(waitTime);
		if(k == 'q')
			break;
		else if (k == 'r')
			res = false;
		else if (k == 'c') {
			auto a = distr(eng);
			g.setResolution(Size(a, a));
		}
	}
	return 0;
}

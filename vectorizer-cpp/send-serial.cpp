#include <cstdlib>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

static int smooth_val = 3;
static int apeture = 0;
static int high_th = 70;
static int low_th = 30;

static int poly_acc = 300;

static Mat res_smooth;
static Mat resized;

void smooth_val_callback(int pos, void* data) {
}


void setup_gui() {
	namedWindow("Canny", CV_WINDOW_NORMAL);
	namedWindow("Contours", CV_WINDOW_NORMAL);

	createTrackbar("smooth", "Canny", &smooth_val, 10, &smooth_val_callback);
	/*
	createTrackbar("apeture", "Canny", &apeture, 2, &smooth_val_callback);
	createTrackbar("lowThresh", "Canny", &high_th, 100, &smooth_val_callback);
	createTrackbar("highThresh", "Canny", &low_th, 100, &smooth_val_callback);
	*/

	createTrackbar("poly acc", "Contours", &poly_acc, 500, &smooth_val_callback);
}

void init_mats(int width, int height) {
	Size size(width, height);
	res_smooth.create(size, CV_8UC1);
	resized.create(size, CV_8UC1);
}


int main(int argc, char* argv[]) {
	setup_gui();

	if (argc < 3) {
		cout << "Not enough arguments\n";
		return 0;
	}

	int width = atoi(argv[1]);
	string filename(argv[2]);

	Mat source = imread(filename, 0);
	int height = (double)width / source.cols * source.rows;

	init_mats(width, height);

	Size newsize(width, height);
	resize(source, resized, newsize);


	while(waitKey() != 'e');
}

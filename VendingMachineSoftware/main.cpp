#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <concepts>

#include "utils.h"
#include "IFeatureExtractor.h"
#include "parameters_config.h"
#include "FeatureExtractor.h"
#include "Classifier.h"

using namespace cv;

void classify() {
	cv::Mat mask = cv::imread(R"(C:\Users\zgstv\OneDrive\���������\CSF\programming\programs\Mag Project\VendingMachineSoftware\Mask Bottle.png)");
	if (mask.empty())
		throw exception("����������� ����� �� ���������.");
	namedWindow("Mask", WINDOW_AUTOSIZE);
	cv::imshow("Mask", mask);
	cv::moveWindow("Mask", 0, 45);
	cv::cvtColor(mask, mask, cv::COLOR_RGB2GRAY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	if (contours.empty())
		throw exception("������ �������� ����.");
	vmsoftware::IFeatureExtractor* feature_extractor = new vmsoftware::FeatureExtractor(SLICES_COUNT, SLICE_AXIS, REL_INDENT_X);
	vmsoftware::Classifier* classifier = new vmsoftware::Classifier(feature_extractor, BOTTLE_CLASS_THRESH);
	vmsoftware::BottleType bottle_type = classifier->classify(mask, contours.front());
	string btype = !bottle_type ? "�����" : "�������";
	cout << btype << endl;

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void test_rot_rect() {
	Mat test_image(200, 200, CV_8UC3, Scalar(0));
	RotatedRect rRect = RotatedRect(Point2f(100, 100), Size2f(100, 50), -10);
	Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(test_image, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2);
	Rect brect = rRect.boundingRect();
	rectangle(test_image, brect, Scalar(255, 0, 0), 2);
	imshow("rectangles", test_image);
	cout << rRect.angle << endl;
	waitKey(0);
}
int main()
{
	setlocale(LC_ALL, "");

	classify();
	//test_rot_rect();
	return 0;
}
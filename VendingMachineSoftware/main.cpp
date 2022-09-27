#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <concepts>
#include "utils.h"
#include "IFeatureExtractor.h"
#include "FeatureExtractor.h"
#include "parameters_config.h"
#include "Classifier.h"
using namespace cv;

int main()
{
	setlocale(LC_ALL, "");

	cv::Mat mask = cv::imread(R"(C:\Users\zgstv\OneDrive\Документы\CSF\programming\programs\Mag Project\VendingMachineSoftware\Mask.png)");
	if (!mask.data)
		throw exception("Изображение маски не прочитано.");
	namedWindow("Mask", WINDOW_AUTOSIZE);
	cv::imshow("Mask", mask);
	cv::moveWindow("Mask", 0, 45);
	cv::cvtColor(mask, mask, cv::COLOR_RGB2GRAY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	vmsoftware::IFeatureExtractor *feature_extractor = new vmsoftware::FeatureExtractor(SLICES_COUNT, SLICE_AXIS, REL_INDENT_X);
	vmsoftware::Classifier *classifier = new vmsoftware::Classifier(feature_extractor, BOTTLE_CLASS_THRESH);
	vmsoftware::BottleType bottle_type = classifier->classify(mask, contours.front());
	string btype = bottle_type ? "Банка" : "Бутылка";
	cout << btype << endl;

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
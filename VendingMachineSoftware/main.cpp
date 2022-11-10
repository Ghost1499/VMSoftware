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

using std::cout, std::cin,std::endl, std::vector, std::string;

classification::BottleType classify(string mask_path) {
	using classification::Classifier, classification::IFeatureExtractor, classification::BottleType;
	using features::FeatureExtractor;

	cv::Mat mask = cv::imread(mask_path);
	if (mask.empty())
		throw std::exception("Изображение маски не прочитано.");
#ifdef VALIDATE
	cv::namedWindow("Mask", cv::WINDOW_AUTOSIZE);
	//cv::moveWindow("Mask", 0, 45);
	cv::imshow("Mask", mask);
#endif // VALIDATE
	cv::cvtColor(mask, mask, cv::COLOR_RGB2GRAY);

	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	if (contours.empty())
		throw std::exception("Вектор контуров пуст.");
	IFeatureExtractor* feature_extractor = new FeatureExtractor(config::SLICES_COUNT, config::SLICE_AXIS, config::REL_INDENT_X);
	Classifier* classifier = new Classifier(feature_extractor, config::BOTTLE_CLASS_THRESH);
	BottleType bottle_type = classifier->classify(mask, contours.front());
#ifdef VALIDATE
	std::string btype = !bottle_type ? "Банка" : "Бутылка";
	cout << btype << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();
#endif // VALIDATE
	return bottle_type;
}

void test_rot_rect() {
	using namespace cv;
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
	string mask_path = R"(C:\Users\zgstv\OneDrive\Документы\CSF\programming\programs\Mag Project\VendingMachineSoftware\Mask bottle 2.png)";
	classify(mask_path);
	//test_rot_rect();
	return 0;
}
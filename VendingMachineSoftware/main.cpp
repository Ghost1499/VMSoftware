#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <concepts>
#include <filesystem>
#include <map>
#include <fstream>

#include "utils.h"
#include "IFeatureExtractor.h"
#include "parameters_config.h"
#include "FeatureExtractor.h"
#include "Classifier.h"

using std::cout, std::cin, std::cerr, std::endl, std::vector, std::string;
namespace fs = std::filesystem;

classification::BottleType classify(string);

void classify_all(fs::path data_dir, fs::path result_dir, string sample_filename = "Mask.png") {
	using fs::path;
	using classification::BottleType;
	std::map<string, BottleType> dir_classes{ {"Aluminium",BottleType::Can},{"Glass",BottleType::Bottle},{"PET",BottleType::Bottle} };
	for (auto const& data_dir_entry : fs::directory_iterator(data_dir))
	{
		if (!data_dir_entry.exists())
			throw exceptions::VMSoftException((std::stringstream() << "Файла для хранения изображений, подлежащих классификации, по пути " << data_dir_entry.path() << "  не существует.").str());
		if (!data_dir_entry.is_directory())
			throw exceptions::VMSoftException((std::stringstream() << "Файл по пути " << data_dir_entry.path() << " для хранения изображений, подлежащих классификации не является каталогом.").str());
		BottleType dir_class = dir_classes[data_dir_entry.path().stem().string()];
		int count = 0;
		int errors_count = 0;
		vector<path> errors_paths;
		for (auto const& class_dir_entry : fs::directory_iterator(data_dir_entry)) {
			path sample_path = class_dir_entry.path() / sample_filename;
			if (!fs::exists(sample_path))
				cerr << "Файл для классификации " << sample_path << " не существует";
			else {
				BottleType btype = classify(sample_path.string());
				count++;
				if (btype != dir_class) {
					errors_count++;
					errors_paths.push_back(class_dir_entry);
				}
			}
		}

		path errors_file_path = result_dir / (data_dir_entry.path().filename().string() + "_errors.txt");
		std::ofstream errors_ofstream(errors_file_path);
		errors_ofstream.precision(3);
		if (!errors_ofstream.is_open())
			throw exceptions::VMSoftException((std::stringstream() << "Ошибка открытия файла по пути " << errors_file_path << " для записи ошибок классификации.").str());
		errors_ofstream << "Папка: " << data_dir_entry.path().filename() << endl
			<< "Корректный класс: " << (!dir_class ? "Банка" : "Бутылка") << endl
			<< "Всего: " << count << endl
			<< "Ошибок: " << errors_count << endl
			<< "Правильность: " << (count - errors_count) / float(count) << endl
			<< "Ошибочные образцы:" << endl;
		for (size_t epath_n = 0; epath_n < errors_paths.size(); epath_n++)
		{
			errors_ofstream << epath_n + 1 << ". " << errors_paths[epath_n] << endl;
		}
	}
}

classification::BottleType classify(string mask_path) {
	using classification::Classifier, classification::IFeatureExtractor, classification::BottleType;
	using features::FeatureExtractor;

	cv::Mat mask = cv::imread(mask_path);
	if (mask.empty())
		throw std::exception("Изображение маски не прочитано.");
#ifdef VALIDATE
	cv::imshow("Mask", mask);
#endif // VALIDATE
	cv::cvtColor(mask, mask, cv::COLOR_RGB2GRAY);


	IFeatureExtractor* feature_extractor = new FeatureExtractor(config::SLICES_COUNT, config::SLICE_AXIS, config::FE_IMG_ORIENT, config::REL_INDENT_X);
	Classifier* classifier = new Classifier(feature_extractor, config::BOTTLE_CLASS_THRESH);
	BottleType bottle_type = classifier->classify(mask);
#ifdef VALIDATE
	std::string btype = !bottle_type ? "Банка" : "Бутылка";
	cout << btype << endl;
	cv::waitKey(0);
	cv::destroyAllWindows();
#endif // VALIDATE
	return bottle_type;
}

//void test_rot_rect() {
//	using namespace cv;
//	Mat test_image(200, 200, CV_8UC3, Scalar(0));
//	RotatedRect rRect = RotatedRect(Point2f(100, 100), Size2f(100, 50), -10);
//	Point2f vertices[4];
//	rRect.points(vertices);
//	for (int i = 0; i < 4; i++)
//		line(test_image, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2);
//	Rect brect = rRect.boundingRect();
//	rectangle(test_image, brect, Scalar(255, 0, 0), 2);
//	imshow("rectangles", test_image);
//	cout << rRect.angle << endl;
//	waitKey(0);
//}
int main()
{
	setlocale(LC_ALL, "");

	//string mask_path = R"(C:\Users\zgstv\JupyterLab Notebooks\Mag-Project\Barcode_detection\data\classification\cropped\PET\20220712_161111\Mask.png)";
	//classify(mask_path);

	fs::path data_folder = LR"(C:\Users\zgstv\JupyterLab Notebooks\Mag-Project\Barcode_detection\data\classification\cropped)";
	classify_all(data_folder,"../results");

	//cv::Mat mask = cv::imread(mask_path);
	//cv::imshow("source", mask);
	//cv::Mat rotated = features::orient_image(mask, features::Horizontal);
	//cv::imshow("rotated",rotated);
	//cv::Mat rotated_back = features::orient_image(rotated, features::Vertical);
	//cv::imshow("rotated back", rotated_back);
	//cv::waitKey(0);
	//cv::destroyAllWindows();
	return 0;
}
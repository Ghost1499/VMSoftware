#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "FeatureExtractor.h"
#include "IFeatureExtractor.h"


namespace classification {
	using cv::Mat, cv::Point;
	using std::vector;
	using features::IFeatureExtractor;
	enum BottleType
	{
		Can,
		Bottle
	};

	class Classifier
	{
		IFeatureExtractor *feature_extractor;
		float thresh;
		~Classifier();
		
	public:
		float get_thresh();
		void set_thresh(float value);
		Classifier(IFeatureExtractor *feature_extractor, float thresh);
		BottleType classify(Mat mask, vector<Point> object_contour);
	};

	
}


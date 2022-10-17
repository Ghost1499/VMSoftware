#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "FeatureExtractor.h"
#include "IFeatureExtractor.h"
#include "VMSoftException.h"


namespace classification {
	using cv::Mat, cv::Point;
	using std::vector;
	using features::IFeatureExtractor;
	using exceptions::VMSoftException;

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
		BottleType classify(const Mat mask,const vector<Point>& object_contour);
	};

	
}


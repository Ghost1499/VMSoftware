#pragma once
#include <concepts>
#include <opencv2/core.hpp>

namespace features {
	using cv::Mat,cv::Point;
	using std::vector;
	class IFeatureExtractor
	{
	public:
		virtual float extract(Mat mask, std::vector<Point> object_contour) = 0;
	};
}


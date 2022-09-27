#pragma once
#include <concepts>
#include <opencv2/core.hpp>

using cv::Mat,cv::Point;

namespace vmsoftware {
	class IFeatureExtractor
	{
	public:
		virtual float extract(Mat mask, std::vector<Point> object_contour) = 0;
	};
}


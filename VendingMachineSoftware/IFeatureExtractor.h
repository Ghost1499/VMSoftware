#pragma once
#include <concepts>
#include <opencv2/core.hpp>

namespace features {
	using cv::Mat;
	class IFeatureExtractor
	{
	public:
		virtual float extract(Mat mask) = 0;
	};
}


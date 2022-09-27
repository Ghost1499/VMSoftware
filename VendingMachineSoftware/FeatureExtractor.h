#pragma once
#include <concepts>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "IFeatureExtractor.h"
#include "utils.h"

using namespace std;
using namespace cv;

namespace vmsoftware {
	enum Axis
	{
		Vertical = 0,
		Horizontal = 1
	};

	class FeatureExtractor : public IFeatureExtractor
	{
		vector<float> rel_indents{ 0, 0 };
		int slices_count;
		Axis slice_axis;

		void set_rel_indent(Axis axis, float value);
		Mat crop_object(Mat mask, vector<Point> contour);
		Mat make_indents(Mat mask);
		vector<Mat> slice(Mat mask, Axis axis, int slices_count);
		float calculate_features(vector<Mat> slices);
	
	public:
		void set_rel_indent_x(float value);
		void set_rel_indent_y(float value);

		FeatureExtractor(int slices_count, Axis slice_axis, float rel_indent_x = 0, float rel_indent_y = 0);

		float extract(Mat mask, vector<Point> object_contour) override;
	};
}


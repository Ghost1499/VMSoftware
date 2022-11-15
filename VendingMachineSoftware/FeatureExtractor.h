#pragma once
#include <concepts>
#include <vector>
#include <cassert>
#include <format>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "IFeatureExtractor.h"
#include "utils.h"

namespace features {
	

	enum Axis
	{
		Vertical = 0,
		Horizontal = 1
	};

	Axis find_orientation(cv::Size size);
	Mat orient_image(Mat image, Axis orientation);
	vector<vector<Point>> find_contours(Mat mask);

	class FeatureExtractor : public IFeatureExtractor
	{
		vector<float> rel_indents{ 0, 0 };
		int slices_count;
		Axis slice_axis;
		Axis image_orientation;

		void set_rel_indent(Axis axis,float value);
		Mat crop_object(Mat mask,const vector<Point>& contour);
		Mat make_indents(Mat mask);
		vector<Mat> slice(Mat mask, Axis axis, int slices_count);
		float calculate_features(const vector<Mat>& slices);
	
	public:
		void set_rel_indent_x(float value);
		void set_rel_indent_y(float value);
		float get_rel_indent_x() const;
		float get_rel_indent_y() const;
		
		void set_image_orientation(Axis orientation);
		Axis get_image_orientation() const;

		FeatureExtractor(int slices_count, Axis slice_axis,Axis image_orientation, float rel_indent_x = 0, float rel_indent_y = 0);

		float extract(Mat mask) override;
	};
}


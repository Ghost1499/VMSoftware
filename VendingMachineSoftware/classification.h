#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "utils.h"

using namespace cv;
using namespace std;

namespace vmsoftware
{
	enum Axis;
	
	void classify(Mat mask, vector<Point> object_contour, int slices_count, float y_indent, float x_indent);
	
	float _get_features(Mat mask, vector<Point> object_countour, int slices_count, float y_indent, float x_indent);

	Mat _crop_object(Mat mask, vector<Point> contour);

	Mat _make_indent(Mat mask, vmsoftware::Axis axis, float indent);

	vector<Mat> _slice(Mat mask, vmsoftware::Axis axis, int slices_count);

}


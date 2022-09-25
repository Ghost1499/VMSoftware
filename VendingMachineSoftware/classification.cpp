#include "classification.h"

enum vmsoftware::Axis
{
	Vertical = 0,
	Horizontal = 1
};

void vmsoftware::classify(Mat mask, vector<Point> object_contour, int slices_count, float y_indent, float x_indent) {
	float score = vmsoftware::_get_features(mask, object_contour, slices_count, y_indent, x_indent);
}

float vmsoftware::_get_features(Mat mask, vector<Point> object_contour, int slices_count, float y_indent, float x_indent) {
	mask = mask.clone();
	Mat cropped_mask = vmsoftware::_crop_object(mask, object_contour);
	Mat indeted = vmsoftware::_make_indent(cropped_mask, vmsoftware::Horizontal, x_indent);
	vector<Mat> slices = vmsoftware::_slice(indeted, Horizontal, 5);
	float first_slice_ratio = countNonZero(slices.front()) / float(slices[0].total());
	float last_slice_ratio = countNonZero(slices.back()) / float(slices[0].total());
	float ratio = first_slice_ratio > last_slice_ratio ? last_slice_ratio : first_slice_ratio;
	return ratio;
}

Mat vmsoftware::_crop_object(Mat mask, vector<Point> contour) {
	/*vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);*/

	vector<Point> contour_poly;
	approxPolyDP(contour, contour_poly, 10, true);
	//Rect bound_rect = boundingRect(contour_poly);
	RotatedRect min_rect = minAreaRect(contour_poly);
	return vmsoftware::crop_rotated_rect(mask, min_rect);
}

Mat vmsoftware::_make_indent(Mat mask, vmsoftware::Axis axis, float rel_indent) {
	vector<Range> ranges{ Range::all(),Range::all() };
	int indent = roundf(mask.size[axis] * rel_indent);
	ranges[axis] = Range(indent, mask.size[axis] - indent);
	return mask(ranges);
}

vector<Mat> vmsoftware::_slice(Mat mask, vmsoftware::Axis axis, int slices_count) {
	vector<Mat> slices(slices_count);
	int length = mask.size[axis];
	auto round2int = [](float elem) { return static_cast<int>(roundf(elem)); };
	vector<float> slices_steps = vmsoftware::linspace(0, length - 1, slices_count + 1);
	vector<int> slices_steps_rounded(slices_steps.size());
	std::transform(slices_steps.begin(), slices_steps.end(), slices_steps_rounded.begin(), round2int);
	
	vector<Range> ranges{Range::all(),Range::all() };
	for (size_t i = 1; i < slices_steps_rounded.size(); i++)
	{
		ranges[axis] = Range(slices_steps_rounded[i - 1], slices_steps_rounded[i]);
		slices[i] = mask(ranges);
	}
	return slices;
}

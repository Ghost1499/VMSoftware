#include "FeatureExtractor.h"

void vmsoftware::FeatureExtractor::set_rel_indent(Axis axis, float value) {
	if (value < 0 || value >= 1)
		throw invalid_argument("Ќедопустимое значение параметра \"value\"");
	this->rel_indents[axis] = value;
}

Mat vmsoftware::FeatureExtractor::crop_object(Mat mask, vector<Point> contour) {

	/*vector<Point> contour_poly;
	approxPolyDP(contour, contour_poly, 10, true);*/
	//Rect bound_rect = boundingRect(contour_poly);
	RotatedRect min_rect = minAreaRect(contour);
	Mat drawing = vmsoftware::draw_rotated_rect(mask, min_rect);
	cv::imshow("Min rect", drawing);

	return vmsoftware::crop_rotated_rect(mask, min_rect);
}

Mat vmsoftware::FeatureExtractor::make_indents(Mat mask) {
	vector<Range> ranges{ Range::all(),Range::all() };
	for (int axis = 0; axis < ranges.size(); axis++)
	{
		//необ€зательна€ строчка, но лучше(?) дл€ производительности
		if (rel_indents[axis] == 0)
			continue;
		int indent = static_cast<int>(roundf(mask.size[axis] * this->rel_indents[axis]));
		ranges[axis] = Range(indent, mask.size[axis] - indent);
	}
	return mask(ranges);
}

vector<Mat> vmsoftware::FeatureExtractor::slice(Mat mask, Axis axis, int slices_count) {
	vector<Mat> slices(slices_count);
	int length = mask.size[axis];
	auto round2int = [](float elem) { return static_cast<int>(roundf(elem)); };
	vector<float> slices_steps = vmsoftware::linspace(0, length - 1, slices_count + 1);
	vector<int> slices_steps_rounded(slices_steps.size());
	std::transform(slices_steps.begin(), slices_steps.end(), slices_steps_rounded.begin(), round2int);

	vector<Range> ranges{ Range::all(),Range::all() };
	for (size_t i = 1; i < slices_steps_rounded.size(); i++)
	{
		ranges[axis] = Range(slices_steps_rounded[i - 1], slices_steps_rounded[i]);
		slices[i-1] = mask(ranges);
	}
	return slices;
}

float vmsoftware::FeatureExtractor::calculate_features(vector<Mat> slices) {
	float first_slice_ratio = countNonZero(slices.front()) / float(slices[0].total());
	float last_slice_ratio = countNonZero(slices.back()) / float(slices[0].total());
	float ratio = cv::abs(first_slice_ratio - last_slice_ratio);
	return ratio;
}

void vmsoftware::FeatureExtractor::set_rel_indent_x(float value) {
	this->set_rel_indent(vmsoftware::Axis::Horizontal, value);
}

void vmsoftware::FeatureExtractor::set_rel_indent_y(float value) {
	this->set_rel_indent(vmsoftware::Axis::Vertical, value);
}

vmsoftware::FeatureExtractor::FeatureExtractor(int slices_count, Axis slice_axis, float rel_indent_x, float rel_indent_y) {
	this->set_rel_indent_x(rel_indent_x);
	this->set_rel_indent_y(rel_indent_y);
	this->slices_count = slices_count;
	this->slice_axis = slice_axis;
}

float vmsoftware::FeatureExtractor::extract(Mat mask, vector<Point> object_contour) {
	mask = mask.clone();
	Mat cropped_mask = this->crop_object(mask, object_contour);
	cv::imshow("Cropped mask", cropped_mask);
	Mat indeted = this->make_indents(cropped_mask);
	vector<Mat> slices = slice(indeted, this->slice_axis, this->slices_count);
	float feature = this->calculate_features(slices);
	return feature;
}

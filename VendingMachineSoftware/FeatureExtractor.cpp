#include "FeatureExtractor.h"
#include <cassert>

void features::FeatureExtractor::set_rel_indent(Axis axis, float value) {
	if (value < 0 || value >= 1)
		throw std::invalid_argument("Ќедопустимое значение параметра \"value\"");
	this->rel_indents[axis] = value;
}

cv::Mat features::FeatureExtractor::crop_object(Mat mask, vector<Point> contour) {
	
	/*vector<Point> contour_poly;
	approxPolyDP(contour, contour_poly, 10, true);*/
	//Rect bound_rect = boundingRect(contour_poly);
	cv::RotatedRect min_rect = minAreaRect(contour);
#ifdef VALIDATE
	std::cout << min_rect.angle << std::endl;
	Mat drawing = utils::draw_rotated_rect(mask, min_rect);
	cv::imshow("Min rect", drawing);
#endif // VALIDATE
	return utils::crop_rotated_rect(mask, min_rect);
}

cv::Mat features::FeatureExtractor::make_indents(Mat mask) {
	using cv::Range;
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

std::vector<cv::Mat> features::FeatureExtractor::slice(Mat mask, Axis axis, int slices_count) {
	using cv::Range;
	vector<Mat> slices(slices_count);
	int length = mask.size[axis];
	auto round2int = [](float elem) { return static_cast<int>(roundf(elem)); };
	vector<float> slices_steps = utils::linspace(0, length - 1, slices_count + 1);
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

float features::FeatureExtractor::calculate_features(vector<Mat> slices) {
	float first_slice_ratio = countNonZero(slices.front()) / float(slices[0].total());
	float last_slice_ratio = countNonZero(slices.back()) / float(slices[0].total());
	float ratio = cv::abs(first_slice_ratio - last_slice_ratio);
	return ratio;
}

void features::FeatureExtractor::set_rel_indent_x(float value) {
	this->set_rel_indent(Axis::Horizontal, value);
}

void features::FeatureExtractor::set_rel_indent_y(float value) {
	this->set_rel_indent(Axis::Vertical, value);
}

features::FeatureExtractor::FeatureExtractor(int slices_count, Axis slice_axis, float rel_indent_x, float rel_indent_y) {
	this->set_rel_indent_x(rel_indent_x);
	this->set_rel_indent_y(rel_indent_y);
	this->slices_count = slices_count;
	this->slice_axis = slice_axis;
}

float features::FeatureExtractor::extract(Mat mask, vector<Point> object_contour) {
	assert(!mask.empty() && "Input mask is empty.");
	mask = mask.clone();
	Mat cropped_mask = this->crop_object(mask, object_contour);
	Mat indeted = this->make_indents(cropped_mask);
#ifdef VALIDATE
	cv::imshow("Cropped mask", cropped_mask);
	cv::imshow("Indented mask", indeted);
#endif // VALIDATE
	vector<Mat> slices = slice(indeted, this->slice_axis, this->slices_count);
	float feature = this->calculate_features(slices);
	return feature;
}

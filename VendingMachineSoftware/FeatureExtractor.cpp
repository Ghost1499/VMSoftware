#include "FeatureExtractor.h"
#include "VMSoftException.h"
#include "SlicesCountException.h"
#include "RelIndentException.h"


void features::FeatureExtractor::set_rel_indent(Axis axis, float value) {
	if (value < 0 || value >= 1)
		throw exceptions::RelIndentException(value, axis);
	this->rel_indents[axis] = value;
}

cv::Mat features::FeatureExtractor::crop_object(Mat mask, const vector<Point>& contour) {
	
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
	vector<Range> ranges(mask.dims);
	for (int axis = 0; axis < ranges.size(); axis++)
	{
		//необязательная строчка, но лучше(?) для производительности
		if (rel_indents[axis] == 0) {
			ranges[axis] = Range::all();
			continue;
		}
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

float features::FeatureExtractor::calculate_features(const vector<Mat>& slices) {
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

float features::FeatureExtractor::get_rel_indent_x() const
{
	return this->rel_indents[Axis::Horizontal];
}

float features::FeatureExtractor::get_rel_indent_y() const
{
	return this->rel_indents[Axis::Vertical];
}

void features::FeatureExtractor::set_image_orientation(Axis orientation)
{
	image_orientation = orientation;
}

features::Axis features::FeatureExtractor::get_image_orientation() const
{
	return image_orientation;
}

features::FeatureExtractor::FeatureExtractor(int slices_count, Axis slice_axis,Axis image_orientation, float rel_indent_x, float rel_indent_y) {
	if (slices_count < 1)
		throw exceptions::SlicesCountException("Параметр slices_count<1.",slices_count);
	this->slices_count = slices_count;
	this->slice_axis = slice_axis;
	set_image_orientation(image_orientation);
	this->set_rel_indent_x(rel_indent_x);
	this->set_rel_indent_y(rel_indent_y);
}

float features::FeatureExtractor::extract(Mat mask) {
	assert(!mask.empty() && "Входная маска пустая.");
	//assert(!object_contour.empty() && "Входной контур пустой.");
	mask = mask.clone();
	Mat rotated = orient_image(mask, image_orientation);
	vector<Point> object_contour = find_contours(rotated).front();
	Mat cropped_mask = this->crop_object(rotated, object_contour);
	cv::rotate(cropped_mask, cropped_mask, cv::ROTATE_180);
	Mat indeted = this->make_indents(cropped_mask);
#ifdef VALIDATE
	cv::imshow("Match orient", rotated);
	cv::imshow("Cropped mask", cropped_mask);
	cv::imshow("Indented mask", indeted);
#endif // VALIDATE
	vector<Mat> slices = slice(indeted, this->slice_axis, this->slices_count);
	float feature = this->calculate_features(slices);
	return feature;
}

features::Axis features::find_orientation(cv::Size size)
{
	return  size.aspectRatio() > 1 ? Axis::Horizontal : Axis::Vertical;
}

cv::Mat features::orient_image(Mat image, Axis dest_orient)
{
	Axis curr_orient = find_orientation(image.size());
	if (curr_orient == dest_orient)
		return image;
	Mat rotated(image.cols, image.rows, image.type());
	cv::RotateFlags rotate_flag;
	if (curr_orient == Horizontal)
		rotate_flag = cv::ROTATE_90_CLOCKWISE;
	else
		rotate_flag = cv::ROTATE_90_COUNTERCLOCKWISE;
	cv::rotate(image, rotated, rotate_flag);
	return rotated;
}

std::vector<std::vector<cv::Point>> features::find_contours(Mat mask)
{
	using std::vector;
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	if (contours.empty())
		throw exceptions::VMSoftException("Вектор контуров пуст.");
	return contours;
}

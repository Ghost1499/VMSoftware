#include "utils.h"


Mat vmsoftware::crop_rotated_rect(Mat m, RotatedRect rot_rect)
{
	Mat rot_mat = getRotationMatrix2D(rot_rect.center, 90+rot_rect.angle, 1);
	Mat rotated_m;
	warpAffine(m, rotated_m, rot_mat, m.size());
	cv::imshow("Rotated", rotated_m);
	Mat cropped_m;
	getRectSubPix(rotated_m, Size(rot_rect.size.height,rot_rect.size.width), rot_rect.center, cropped_m);
	return cropped_m;
}

Mat vmsoftware::draw_rotated_rect(Mat mat, RotatedRect rot_rect, Scalar color)
{
	cv::RNG rng(12345);
	if(color == Scalar::zeros())
		color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	Mat drawing = mat.clone();
	Point2f rect_points[4];
	rot_rect.points(rect_points);
	for (int j = 0; j < 4; j++)
	{
		line(drawing, rect_points[j], rect_points[(j + 1) % 4], Scalar(255, 255, 0));
	}
	return drawing;
}


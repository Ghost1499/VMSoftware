#include "utils.h"


Mat vmsoftware::crop_rotated_rect(Mat m, RotatedRect rot_rect)
{
	/*Point2f pts[4];
	rot_rect.points(pts);
	vector<Point2f> box(pts,std::end(pts));
	vector<Point> box_rounded(box.size());
	std::transform(box.begin(), box.end(), box_rounded.begin(), [](Point2f p) {return Point(p); })
	vector<Point*/
	float rot_angle = rot_rect.angle;
	int rot_width = rot_rect.size.width;
	int rot_height = rot_rect.size.height;
	if (rot_rect.size.width < rot_rect.size.height) {
		rot_angle = 90 + rot_angle;
		int tmp = rot_width;
		rot_width = rot_height;
		rot_height = tmp;
	}
	Mat rot_mat = getRotationMatrix2D(rot_rect.center, rot_angle, 1);
	Mat rotated_m;
	warpAffine(m, rotated_m, rot_mat, m.size());
#ifdef VALIDATE
	cv::imshow("Rotated", rotated_m);
#endif // VALIDATE
	Mat cropped_m;
	getRectSubPix(rotated_m, Size(rot_width,rot_height), rot_rect.center, cropped_m);
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


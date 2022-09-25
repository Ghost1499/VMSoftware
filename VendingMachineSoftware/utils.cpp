#include "utils.h"


Mat vmsoftware::crop_rotated_rect(Mat m, RotatedRect rot_rect)
{
	Mat rot_mat = getRotationMatrix2D(rot_rect.center, rot_rect.angle, 1);
	Mat rotated_m;
	warpAffine(m, rotated_m, rot_mat, m.size());
	Mat cropped_m;
	getRectSubPix(rotated_m, rot_rect.size, rot_rect.center, cropped_m);
	return cropped_m;
}


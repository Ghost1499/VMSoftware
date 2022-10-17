#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <concepts>

namespace utils
{
	using cv::Mat, cv::Point,cv::Scalar,cv::RotatedRect;
	using std::vector;

	Mat crop_rotated_rect(Mat m, RotatedRect rot_rect);
	Mat draw_rotated_rect(Mat m, RotatedRect rot_rect, Scalar color= Scalar());

	template<typename T>
		requires std::integral<T> || std::floating_point<T>
	vector<float> linspace(T start, T stop, int count)
	{
		if (count < 2)
			throw std::invalid_argument("Параметр count<2");
		float diff = static_cast<float>(stop - start);
		float step = diff / (count - 1);

		vector<float> slices(count);
		for (size_t i = 0; i < count; i++)
		{
			float res = start + step * i;
			slices[i] = res;
		}
		return slices;
	}
}
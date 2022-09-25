#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <concepts>
#include "classification.h"
#include "utils.h"
using namespace cv;

int main()
{
    /*cv::Mat img = cv::imread(R"(C:\Users\zgstv\OneDrive\Изображения\Saved Pictures\young_moon_pixel.png)");
    namedWindow("First OpenCV Application", WINDOW_AUTOSIZE);
    cv::imshow("First OpenCV Application", img);
    cv::moveWindow("First OpenCV Application", 0, 45);
    cv::waitKey(0);
    cv::destroyAllWindows();
    vmsoftware::classify(img);*/

    auto slices = vmsoftware::linspace(-6, 0, 7);
    for (auto number : slices)
    {
        cout << number<<" ";
    }

    return 0;
}
// Project a uniform grey pattern
// written by Shizuo Sakamoto as example in the book, titled as "Basics and Practices of 3D measurement"
// released by Ohmsha, Ltd.
//
// - version 1.00 on 6th, June, 2022.
//   developed on Mouse Computer H5-CML, Ubuntu 20.04LTS.


#include <opencv2/opencv.hpp>

int
main(int argc, char* argv[])
{
  cv::Mat grey_pattern = cv::Mat(1080, 1920, CV_8UC1);
  grey_pattern = 180;
  cv::namedWindow("grey_pattern", cv::WINDOW_NORMAL);
  cv::setWindowProperty("grey_pattern", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  cv::moveWindow("grey_pattern", 1920, 0);
  cv::imshow("grey_pattern", grey_pattern);
  cv::waitKey(0);
  cv::destroyAllWindows();
}
// end of program

// Project a uniform grey pattern
// written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
// to be released by Ohmsha, Ltd.
//
// - version 0.91 on 29th, June, 2021.
//   developed in Python 3.8.5 on ubuntu 20.04LTS,
//   on mouse computer H5-CML.

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
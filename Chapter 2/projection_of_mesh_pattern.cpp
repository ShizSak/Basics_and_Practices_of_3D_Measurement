// Project a uniform mesh pattern
// written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
// to be released by Ohmsha, Ltd.
//
// - version 0.91 on 8th, July, 2021.
//   developed in Python 3.8.5 on ubuntu 20.04LTS,
//   on mouse computer H5-CML.

#include <opencv2/opencv.hpp>

int
main(int argc, char* argv[])
{
  cv::Mat mesh_pattern = cv::Mat::zeros(1080, 1920, CV_8UC1);
  unsigned char line_brightness = 180;
  for (int row = 0; row < mesh_pattern.rows; row++) {
    for (int col = 0; col < mesh_pattern.cols; col += 40) {
      mesh_pattern.at<unsigned char>(row, col) = line_brightness;
    }
  }
  for (int col = 0; col < mesh_pattern.cols; col++) {
    for (int row = 0; row < mesh_pattern.rows; row += 40) {
      mesh_pattern.at<unsigned char>(row, col) = line_brightness;
    }
  }
  cv::namedWindow("mesh_pattern", cv::WINDOW_NORMAL);
  cv::setWindowProperty("mesh_pattern", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  cv::moveWindow("mesh_pattern", 1920, 0);
  cv::imshow("mesh_pattern", mesh_pattern);
  cv::waitKey(0);
  cv::destroyAllWindows();
}
// end of program
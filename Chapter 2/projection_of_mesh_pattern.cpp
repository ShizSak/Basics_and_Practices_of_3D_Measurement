// Project a uniform mesh pattern
// written by Shizuo Sakamoto as example in the book, titled as "Basics and Practices of 3D measurement"
// released by Ohmsha, Ltd.
//
// - version 1.00 on 6th, June, 2022.
//   developed on Mouse Computer H5-CML, Ubuntu 20.04LTS.


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

// Measure 3D shape by projecting sinusoidal patterns
// written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
// to be released by Ohmsha, Ltd.
//
// - version 0.95 on 14th, October, 2021.
//   developed in Python 3.8.5 on ubuntu 20.04LTS,
//   on mouse computer H5-CML.

#include <fstream>
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>
#include <matplotlib-cpp/matplotlibcpp.h>


void
setCameraProperties(cv::VideoCapture &cap)
{
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  std::cout << "camera properties are changed as below:" << std::endl;
  std::cout << "\tsharpness\t" << cap.get(cv::CAP_PROP_SHARPNESS) << " -> ";
  cap.set(cv::CAP_PROP_SHARPNESS, 0.0f);
  std::cout << cap.get(cv::CAP_PROP_SHARPNESS) << std::endl;
  std::cout << "\tautofocus\t" << cap.get(cv::CAP_PROP_AUTOFOCUS) << " -> ";
  cap.set(cv::CAP_PROP_AUTOFOCUS, 0.0f);
  std::cout << cap.get(cv::CAP_PROP_AUTOFOCUS) << std::endl;
  std::cout << "\tfocus\t\t" << cap.get(cv::CAP_PROP_FOCUS) << " -> ";
  cap.set(cv::CAP_PROP_FOCUS, 60.0f);
  std::cout << cap.get(cv::CAP_PROP_FOCUS) << std::endl;
  std::cout << "\tauto exposure\t" << cap.get(cv::CAP_PROP_AUTO_EXPOSURE) << " -> ";
  cap.set(cv::CAP_PROP_AUTO_EXPOSURE, 1.0f);
  std::cout << cap.get(cv::CAP_PROP_AUTO_EXPOSURE) << std::endl;
  std::cout << "\texposure\t" << cap.get(cv::CAP_PROP_EXPOSURE) << " -> ";
  cap.set(cv::CAP_PROP_EXPOSURE, 250.0f);
  std::cout << cap.get(cv::CAP_PROP_EXPOSURE) << std::endl;
  std::cout << "\tgain\t\t" << cap.get(cv::CAP_PROP_GAIN) << " -> ";
  cap.set(cv::CAP_PROP_GAIN, 0.0f);
  std::cout << cap.get(cv::CAP_PROP_GAIN) << std::endl;
  std::cout << "\tiris\t\t" << cap.get(cv::CAP_PROP_IRIS) << " -> ";
  cap.set(cv::CAP_PROP_IRIS, -1.0f);
  std::cout << cap.get(cv::CAP_PROP_IRIS) << std::endl;
  std::cout << "\tgamma\t\t" << cap.get(cv::CAP_PROP_GAMMA) << " -> ";
  cap.set(cv::CAP_PROP_GAMMA, -1.0f);
  std::cout << cap.get(cv::CAP_PROP_GAMMA) << std::endl;
  std::cout << "\tauto wb\t\t" << cap.get(cv::CAP_PROP_AUTO_WB) << " -> ";
  cap.set(cv::CAP_PROP_AUTO_WB, 0.0f);
  std::cout << cap.get(cv::CAP_PROP_AUTO_WB) << std::endl;
  std::cout << "\twb temperature\t" << cap.get(cv::CAP_PROP_WB_TEMPERATURE) << " -> ";
  cap.set(cv::CAP_PROP_WB_TEMPERATURE, 4000.0f);
  std::cout << cap.get(cv::CAP_PROP_WB_TEMPERATURE) << std::endl;
  std::cout << "\tbuffer size\t" << cap.get(cv::CAP_PROP_BUFFERSIZE) << " -> ";
  cap.set(cv::CAP_PROP_BUFFERSIZE, 1.0f);
  std::cout << cap.get(cv::CAP_PROP_BUFFERSIZE) << std::endl;
}

bool
loadIntrinsicParams(cv::Mat& mtx, cv::Mat& dist)
{
  std::ifstream f("./intrinsicParams.dat", std::ios::in | std::ios::binary);
  if (!f) {
    return false;
  }
  for (int row = 0; row < mtx.rows; row++) {
    for (int col = 0; col < mtx.cols; col++) {
      f.read(reinterpret_cast<char *>(&(mtx.at<double>(row, col))), sizeof(double));
    }
  }
  for (int row = 0; row < dist.rows; row++) {
    for (int col = 0; col < dist.cols; col++) {
      f.read(reinterpret_cast<char *>(&(dist.at<double>(row, col))), sizeof(double));
    }
  }
  f.close();
  return true;
}

void
createSinusoidalPattern(cv::Mat& sinusoidal_pattern, const float intensity, const float coeff,
			const float lambda, const int phase, const int num)
{
  for (int col = 0; col < sinusoidal_pattern.cols; col++) {
    unsigned char brightness =
      static_cast<unsigned char>(0.5f * intensity
				 * cosf(coeff * (col / lambda + static_cast<float>(phase)
						 / static_cast<float>(num))) + 127.5f);
    for (int row = 0; row < sinusoidal_pattern.rows; row++) {
      sinusoidal_pattern.at<unsigned char>(row, col) = brightness;
    }
  }
}

void
calculateRphases3(float* in, float* rphase, float* amplitude, float* bias)
{
  *bias = (in[0] + in[1] + in[2]) / 3.0f;
  *amplitude = sqrtf((2.0f * in[0] - in[1] - in[2]) * (2.0f * in[0] - in[1] - in[2])
		     + 3.0f * (in[1] - in[2]) * (in[1] - in[2])) / 3.0f;
  *rphase = atan2f(sqrtf(3.0f) * (in[2] - in[1]), 2.0f * in[0] - in[1] - in[2]);
}

void
calculateRphases4(float* in, float* rphase, float* amplitude, float* bias)
{
  *bias = (in[0] + in[1] + in[2] + in[3]) / 4.0f;
  *amplitude = sqrtf((in[0] - in[2])*(in[0] - in[2]) + (in[1] - in[3])*(in[1] - in[3])) / 2.0f;
  *rphase = atan2f(in[0] - in[2], in[1] - in[3]);
}

void
calculateRphasesAny(float* in, int num, float* rphase, float* amplitude, float* bias)
{
  const float coeff = 2.0f * static_cast<float>(M_PI);
  static bool flag = true;
  static float *sinarray, *cosarray;
  if (flag) {
    sinarray = new float[num];
    cosarray = new float[num];
    for (int i = 0; i < num; i++) {
      sinarray[i] = sinf(coeff * i  / static_cast<float>(num));
      cosarray[i] = cosf(coeff * i  / static_cast<float>(num));
    }
    flag = false;
  }
  float corSin = 0.0f, corCos = 0.0f;
  *bias = 0.0f;
  for (int i = 0; i < num; i++) {
    *bias += in[i] / static_cast<float>(num);
    corSin += in[i] * sinarray[i];
    corCos += in[i] * cosarray[i];
  }
  *amplitude = sqrtf(corSin * corSin + corCos * corCos) / 2.0f;
  *rphase = atan2f(corCos, corSin);
}

void
plotSinusoidalWaves(cv::Mat sinusoidalwaves[], cv::Mat& amplitude, cv::Mat& bias,
		    const int num)
{
  int row = sinusoidalwaves[0].rows / 2;
  int cols = sinusoidalwaves[0].cols;
  std::vector<float> u, v[num + 2];
  std::ostringstream ss;

  for (int col = 0; col < cols; col++) {
    u.push_back(col);
    for (int i = 0; i < num; i++) {
      v[i].push_back(sinusoidalwaves[i].at<float>(row, col));
    }
    v[num].push_back(amplitude.at<float>(row, col));
    v[num + 1].push_back(bias.at<float>(row, col));
  }
  matplotlibcpp::title("Pixel Values");
  matplotlibcpp::xlabel("x (pixel)");
  matplotlibcpp::xlim(0, cols);
  matplotlibcpp::ylim(0, 255);
    for (int i = 0; i < num; i++) {
      std::string title = "pixel values (phase ";
      ss << i;
      title.append(ss.str());
      title.append(")");
      matplotlibcpp::named_plot(title, u, v[i]);
      ss.str("");
    }
  matplotlibcpp::named_plot("amplitudes", u, v[num]);
  matplotlibcpp::named_plot("biases", u, v[num + 1]);
  matplotlibcpp::legend();
  matplotlibcpp::show();
  u.clear();
  for (int i = 0; i < num + 2; i++) {
    v[i].clear();
  }
}

void
plotRelativePhases(cv::Mat& rphase, cv::Mat& amplitude, cv::Mat& bias)
{
  int row = rphase.rows / 2;
  int cols = rphase.cols;
  float scaleparam = 3.0f / 255.0f;
  std::vector<float> u(cols), v0(cols), v1(cols), v2(cols);
  for (int col = 0; col < cols; col++) {
    u[col] = col;
    v0[col] = rphase.at<float>(row, col);
    v1[col] = scaleparam * amplitude.at<float>(row, col);
    v2[col] = scaleparam * bias.at<float>(row, col);
  }
  matplotlibcpp::title("Relative Phases");
  matplotlibcpp::xlabel("x (pixel)");
  matplotlibcpp::xlim(0, cols);
  matplotlibcpp::ylim(-M_PI, M_PI);
  matplotlibcpp::named_plot("relative phases", u, v0);
  matplotlibcpp::named_plot("amplitudes normalized between [0, 3]", u, v1);
  matplotlibcpp::named_plot("biases normalized between [0, 3]", u, v2);
  matplotlibcpp::legend();
  matplotlibcpp::show();
}

void
thresholdAmplitudes(cv::Mat& rphase, cv::Mat& amplitude, float threshold)
{
  cv::Mat amp = cv::Mat::zeros(amplitude.rows, amplitude.cols, CV_32FC1);
  for (int row = 0; row < amp.rows; row++) {
    for (int col = 0; col < amp.cols; col++) {
      if (amplitude.at<float>(row, col) < threshold) {
	rphase.at<float>(row, col) = -FLT_MAX;
      } else {
	amp.at<float>(row, col) = 255.0f;
      }
    }
  }
  matplotlibcpp::imshow(&(amp.at<float>(0, 0)), amp.rows, amp.cols, 1);
  matplotlibcpp::axis("off");
  matplotlibcpp::show();
}

bool
loadLUTs(cv::Mat LUTrphase[2], cv::Mat LUTcoordv[2])
{
  cv::FileStorage fs("LUTs.xml", cv::FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["LUTatNearEnd"] >> LUTrphase[0];
  fs["CoordinateValuesatNearEnd"] >> LUTcoordv[0];
  fs["LUTatFarEnd"] >> LUTrphase[1];
  fs["CoordinateValuesatFarEnd"] >> LUTcoordv[1];
  fs.release();
  return true;
}

void
convertRphasetoCoordv(cv::Mat& rphase, std::vector<double>& x, std::vector<double>& y, std::vector<double>& z,
		      cv::Mat LUTrphase[2], cv::Mat LUTcoordv[2], int steprow, int stepcol)
{
  const float znear = 220.0f, zfar = 270.0f;
  for (int row = 0; row < rphase.rows; row += steprow) {
    for (int col = 0; col < rphase.cols; col += stepcol) {
      if (LUTrphase[0].at<float>(row, col) == -FLT_MAX || rphase.at<float>(row, col) == -FLT_MAX) {
      	continue;
      }
      if (rphase.at<float>(row, col) < LUTrphase[0].at<float>(row, col)) {
	rphase.at<float>(row, col) += 2.0f * static_cast<float>(M_PI);
      }
      if (rphase.at<float>(row, col) > LUTrphase[1].at<float>(row, col) ||
	  rphase.at<float>(row, col) < LUTrphase[0].at<float>(row, col)) {
	continue;
      }
      float ratio = (rphase.at<float>(row, col) - LUTrphase[0].at<float>(row, col))
       	/ (LUTrphase[1].at<float>(row, col) - LUTrphase[0].at<float>(row, col));
      float X = (LUTcoordv[1].at<cv::Vec3f>(row, col)[0] - LUTcoordv[0].at<cv::Vec3f>(row, col)[0])
	* ratio + LUTcoordv[0].at<cv::Vec3f>(row, col)[0];
      float Y = (LUTcoordv[1].at<cv::Vec3f>(row, col)[1] - LUTcoordv[0].at<cv::Vec3f>(row, col)[1])
	* ratio + LUTcoordv[0].at<cv::Vec3f>(row, col)[1];
      float Z = (zfar - znear) * ratio + znear;
      x.push_back(X);
      y.push_back(Y);
      z.push_back(Z);
    }
  }
}

void
saveCoordv(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z)
{
  std::ofstream f("./3D.asc", std::ios::out);
  for (int i = 0; i < x.size(); i++) {
    f << x[i] << "," << y[i] << "," << z[i] << std::endl;
  }
  f.close();
}

void
plotCoordv(std::vector<double>& x, std::vector<double>& y, std::vector<double>& z)
{
  matplotlibcpp::scatter3(x, y, z);
  matplotlibcpp::xlabel("X (mm)");
  matplotlibcpp::ylabel("Y (mm)");
  matplotlibcpp::set_zlabel("Z (radian)");
  matplotlibcpp::set_zlim(220.0, 270.0);
  matplotlibcpp::show();
}


int
main(int argc, char* argv[])
{
  boost::program_options::options_description options("options");
  options.add_options()
    ("help,h", "describe options")
    ("intensity,i", boost::program_options::value<float>()->default_value(200.0f),
     "intensity of projection (0-255)")
    ("threshold,t", boost::program_options::value<float>()->default_value(10.0f),
     "threshold on amplitude (0-255)")
    ("waitingTime,w", boost::program_options::value<int>()->default_value(100),
     "waiting time [msec] of projection (0-)")
    ("skippingFrames,s", boost::program_options::value<int>()->default_value(1),
     "number of skipping frames (1-)")
    ("lambdaofSinusoidalPattern,l", boost::program_options::value<float>()->default_value(240.0f),
     "wave length of sinusoidal pattern (0<)");
  boost::program_options::variables_map vm;
  boost::program_options::store(parse_command_line(argc, argv, options), vm);
  boost::program_options::notify(vm);
  if (vm.count("help")) {
    std::cout << options << std::endl;
    return -1;
  }
  auto intensity = vm["intensity"].as<float>();
  auto threshold = vm["threshold"].as<float>();
  auto waitingTime = vm["waitingTime"].as<int>();
  auto skippingFrames = vm["skippingFrames"].as<int>();
  auto lambda = vm["lambdaofSinusoidalPattern"].as<float>();

  if (intensity < 0.0f || intensity > 255.0f || threshold < 0.0f || threshold > 255.0f ||
      waitingTime < 1 || skippingFrames < 1 || lambda <= 0.0f) {
    std::cout << options << std::endl;
    return -1;
  }
  cv::VideoCapture cap(4);
  if (!cap.isOpened()) {
    std::cerr << "camera open error" << std::endl;
    return -1;
  }
  setCameraProperties(cap);
  cv::Mat sinusoidal_pattern = cv::Mat(1080, 1920, CV_8UC1);
  const float coeff = 2.0f * static_cast<float>(M_PI);
  const int num = 8;
  cv::Mat frame[num];
  cv::namedWindow("sinusoidal_pattern", cv::WINDOW_NORMAL);
  cv::setWindowProperty("sinusoidal_pattern", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  cv::moveWindow("sinusoidal_pattern", 1920, 0);
  std::cout << "\nmeasure the 3D shape: if you're ready, please press Enter." << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  for (int phase = 0; phase < num; phase++) {
    createSinusoidalPattern(sinusoidal_pattern, intensity, coeff, lambda, phase, num);
    cv::imshow("sinusoidal_pattern", sinusoidal_pattern);
    cv::waitKey(waitingTime);
    int count = skippingFrames;
    while (count >= 0) {
    count--;
      if (!cap.read(frame[phase])) {
	std::cerr << "frame read error" << std::endl;
	return -1;
	}
    }
  }

  cv::Mat mtx = cv::Mat(3, 3, CV_64FC1), dist = cv::Mat(1, 5, CV_64FC1);
  if(!loadIntrinsicParams(mtx, dist)) {
    std::cerr << "file (intrinsic parameters) open error" << std::endl;
    return -1;
  }
  cv::Mat undistortedframe[num];
  for (int i = 0; i < num; i++) {
    cv::undistort(frame[i], undistortedframe[i], mtx, dist);
  }

  cv::Mat sinusoidalwaves[num];
  for (int j = 0; j < num; j++)
    sinusoidalwaves[j] = cv::Mat(480, 640, CV_32FC1);
  cv::Mat amplitude = cv::Mat(480, 640, CV_32FC1);
  cv::Mat bias = cv::Mat(480, 640, CV_32FC1);
  cv::Mat rphase = cv::Mat(480, 640, CV_32FC1);
  cv::Mat coord = cv::Mat(480, 640, CV_32FC3);
  float gamma = 2.2f;
  for (int row = 0; row < undistortedframe[0].rows; row++) {
    for (int col = 0; col < undistortedframe[0].cols; col++) {
      float in[num];
      for (int i = 0; i < num; i++) {
	in[i] = 0.0f;
	for (int bgr = 0; bgr < 3; bgr++) {
	  in[i] += 255.0f
	    * powf(static_cast<float>(undistortedframe[i].at<cv::Vec3b>(row, col)[bgr]) / 255.0f,
		   gamma)
	    / 3.0f;
	}
	sinusoidalwaves[i].at<float>(row, col) = in[i];
      }
      // *** in case of num == 3
      // calculateRphases3(in, &rphase.at<float>(row, col),
      // 		  &amplitude.at<float>(row, col),
      // 		  &(bias.at<float>(row, col)));
      // *** in case of num == 4
      // calculateRphases4(in, &rphase.at<float>(row, col),
      // 		  &amplitude.at<float>(row, col),
      // 		  &(bias.at<float>(row, col)));
      // *** in case of any other values
      calculateRphasesAny(in, num, &rphase.at<float>(row, col),
			  &amplitude.at<float>(row, col),
			  &(bias.at<float>(row, col)));
    }
  }
  plotSinusoidalWaves(sinusoidalwaves, amplitude, bias, num);
  plotRelativePhases(rphase, amplitude, bias);
  thresholdAmplitudes(rphase, amplitude, threshold);

  cv::Mat LUTrphase[2], LUTcoordv[2];
  if (!loadLUTs(LUTrphase, LUTcoordv)) {
    std::cerr << "file (LUTs) open error" << std::endl;
    return -1;
  }
  std::vector<double> x, y, z;
  int steprow = 5, stepcol = 5;
  convertRphasetoCoordv(rphase, x, y, z, LUTrphase, LUTcoordv, steprow, stepcol);
  saveCoordv(x, y, z);
  plotCoordv(x, y, z);

  cv::destroyAllWindows();
  return 0;
}
// end of program

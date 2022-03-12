// Generate LUT by projecting sinusoidal patterns
// written by Shizuo Sakamoto as example in the book, titled as "Basics of 3D measurement",
// to be released by Ohmsha, Ltd.
//
// - version 1.00 on 1st, April, 2022.
//   developed on Mouse Computer H5-CML, Ubuntu 20.04LTS.


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
plotSinusoidalWaves(cv::Mat sinusoidalwaves[], cv::Mat amplitude[], cv::Mat bias[],
		    const int num)
{
  int row = sinusoidalwaves[0].rows / 2;
  int cols = sinusoidalwaves[0].cols;
  std::vector<float> u, v[num + 2];
  std::ostringstream ss;

  for (int times = 0; times < 2; times++) {
    for (int col = 0; col < cols; col++) {
      u.push_back(col);
      for (int i = 0; i < num; i++) {
	v[i].push_back(sinusoidalwaves[i + num * times].at<float>(row, col));
      }
      v[num].push_back(amplitude[times].at<float>(row, col));
      v[num + 1].push_back(bias[times].at<float>(row, col));
    }
    if (times == 0)
      matplotlibcpp::title("Pixel Values at near end");
    else
      matplotlibcpp::title("Pixel Values at far end");
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
}

void
plotRelativePhases(cv::Mat rphase[], cv::Mat amplitude[], cv::Mat bias[])
{
  int row = rphase[0].rows / 2;
  int cols = rphase[0].cols;
  float scaleparam = 3.0f / 255.0f;
  std::vector<float> u(cols), v0(cols), v1(cols), v2(cols), v3(cols), v4(cols), v5(cols);
  for (int col = 0; col < cols; col++) {
    u[col] = col;
    v0[col] = rphase[0].at<float>(row, col);
    v1[col] = scaleparam * amplitude[0].at<float>(row, col);
    v2[col] = scaleparam * bias[0].at<float>(row, col);
    v3[col] = rphase[1].at<float>(row, col);
    v4[col] = scaleparam * amplitude[1].at<float>(row, col);
    v5[col] = scaleparam * bias[1].at<float>(row, col);
  }
  matplotlibcpp::title("Relative Phases");
  matplotlibcpp::xlabel("x (pixel)");
  matplotlibcpp::xlim(0, cols);
  matplotlibcpp::ylim(-M_PI, M_PI);
  matplotlibcpp::named_plot("relative phases (near end)", u, v0);
  matplotlibcpp::named_plot("amplitudes normalized between [0, 3] (near end)", u, v1);
  matplotlibcpp::named_plot("biases normalized between [0, 3] (near end)", u, v2);
  matplotlibcpp::named_plot("relative phases (far end)", u, v3);
  matplotlibcpp::named_plot("amplitudes normalized between [0, 3] (far end)", u, v4);
  matplotlibcpp::named_plot("biases normalized between [0, 3] (far end)", u, v5);
  matplotlibcpp::legend();
  matplotlibcpp::show();
}

void
thresholdAmplitudes(cv::Mat rphase[], cv::Mat amplitude[], float threshold)
{
  cv::Mat amp = cv::Mat::zeros(amplitude[0].rows, amplitude[0].cols, CV_32FC1);
  for (int row = 0; row < amp.rows; row++) {
    for (int col = 0; col < amp.cols; col++) {
      if (amplitude[0].at<float>(row, col) < threshold || amplitude[1].at<float>(row, col) < threshold) {
	for (int times = 0; times < 2; times++)
	  rphase[times].at<float>(row, col) = -FLT_MAX;
      } else {
	amp.at<float>(row, col) = 255.0f;
	if (rphase[0].at<float>(row, col) > rphase[1].at<float>(row, col)) {
	  rphase[1].at<float>(row, col) += 2.0f * static_cast<float>(M_PI);
	}
      }
    }
  }
  matplotlibcpp::imshow(&(amp.at<float>(0, 0)), amp.rows, amp.cols, 1);
  matplotlibcpp::axis("off");
  matplotlibcpp::show();
}

void
calc3DcoordinateValues(cv::Mat coordv[], cv::Mat& mtx, float distance[])
{
  float fx = static_cast<float>(mtx.at<double>(0, 0));
  float fy = static_cast<float>(mtx.at<double>(1, 1));
  float cx = static_cast<float>(mtx.at<double>(0, 2));
  float cy = static_cast<float>(mtx.at<double>(1, 2));
  for (int times = 0; times < 2; times++) {
    for (int row = 0; row < coordv[0].rows; row++) {
      double Y = (row - cy) * distance[times] / fy;
      for (int col = 0; col < coordv[0].cols; col++) {
	double X = (col - cx) * distance[times] / fx;
	coordv[times].at<cv::Vec3f>(row, col)[0] = X;
	coordv[times].at<cv::Vec3f>(row, col)[1] = Y;
	coordv[times].at<cv::Vec3f>(row, col)[2] = distance[times];
      }
    }
  }
}

int
main(int argc, char* argv[])
{
  boost::program_options::options_description options("options");
  options.add_options()
    ("help,h", "describe options")
    ("intensityatNearEnd,n", boost::program_options::value<float>()->default_value(200.0f),
     "intensity of projection at the near end (0-255)")
    ("intensityatFarEnd,f", boost::program_options::value<float>()->default_value(200.0f),
     "intensity of projection at the far end (0-255)")
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
  auto intensityatNearEnd = vm["intensityatNearEnd"].as<float>();
  auto intensityatFarEnd = vm["intensityatFarEnd"].as<float>();
  auto threshold = vm["threshold"].as<float>();
  auto waitingTime = vm["waitingTime"].as<int>();
  auto skippingFrames = vm["skippingFrames"].as<int>();
  auto lambda = vm["lambdaofSinusoidalPattern"].as<float>();

  if (intensityatNearEnd < 0.0f || intensityatNearEnd > 255.0f ||
      intensityatFarEnd < 0.0f || intensityatFarEnd > 255.0f || threshold < 0.0f || threshold > 255.0f ||
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
  cv::Mat frame[num * 2];
  cv::namedWindow("sinusoidal_pattern", cv::WINDOW_NORMAL);
  cv::setWindowProperty("sinusoidal_pattern", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
  cv::moveWindow("sinusoidal_pattern", 1920, 0);
  for (int times = 0; times < 2; times++) {
    float intensity;
    if (times == 0) {
      intensity = intensityatNearEnd;
      std::cout << "\ncreate the LUT at the near end: if you're ready, please press Enter." << std::endl;
    } else {
      intensity = intensityatFarEnd;
      std::cout << "create the LUT at the far end: if you're ready, please press Enter." << std::endl;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int phase = 0; phase < num; phase++) {
      createSinusoidalPattern(sinusoidal_pattern, intensity, coeff, lambda, phase, num);
      cv::imshow("sinusoidal_pattern", sinusoidal_pattern);
      cv::waitKey(waitingTime);
      int count = skippingFrames;
      while (count >= 0) {
	count--;
	if (!cap.read(frame[phase + num * times])) {
	  std::cerr << "frame read error" << std::endl;
	  return -1;
	}
      }
    }
  }

  cv::Mat mtx = cv::Mat(3, 3, CV_64FC1), dist = cv::Mat(1, 5, CV_64FC1);
  if(!loadIntrinsicParams(mtx, dist)) {
    std::cerr << "file (./intrinsicParam.dat) open error" << std::endl;
    return -1;
  }
  cv::Mat undistortedframe[num * 2];
  for (int i = 0; i < num * 2; i++) {
    cv::undistort(frame[i], undistortedframe[i], mtx, dist);
  }

  cv::Mat sinusoidalwaves[num * 2], amplitude[2], bias[2], rphase[2], coord[2];
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < num; j++)
      sinusoidalwaves[j + num * i] = cv::Mat(480, 640, CV_32FC1);
    amplitude[i] = cv::Mat(480, 640, CV_32FC1);
    bias[i] = cv::Mat::zeros(480, 640, CV_32FC1);
    rphase[i] = cv::Mat::zeros(480, 640, CV_32FC1);
    coord[i] = cv::Mat::zeros(480, 640, CV_32FC3);
  }
  float gamma = 2.2f;
  for (int times = 0; times < 2; times++) {
    for (int row = 0; row < undistortedframe[0].rows; row++) {
      for (int col = 0; col < undistortedframe[0].cols; col++) {
	float in[num];
	for (int i = 0; i < num; i++) {
	  in[i] = 0.0f;
	  for (int bgr = 0; bgr < 3; bgr++) {
	    in[i] += 255.0f
	      * powf(static_cast<float>(undistortedframe[i + num * times].at<cv::Vec3b>(row, col)[bgr]) / 255.0f,
		     gamma) / 3.0f;
	  }
	  sinusoidalwaves[i + num * times].at<float>(row, col) = in[i];
	}
	// *** in case of num == 3
	// calculateRphases3(in, &rphase[times].at<float>(row, col),
	// 		  &amplitude[times].at<float>(row, col),
	// 		  &(bias[times].at<float>(row, col)));
	// *** in case of num == 4
	// calculateRphases4(in, &rphase[times].at<float>(row, col),
	// 		  &amplitude[times].at<float>(row, col),
	// 		  &(bias[times].at<float>(row, col)));
	// *** in case of any other values
	calculateRphasesAny(in, num, &rphase[times].at<float>(row, col),
			    &amplitude[times].at<float>(row, col),
			    &(bias[times].at<float>(row, col)));
      }
    }
  }
  plotSinusoidalWaves(sinusoidalwaves, amplitude, bias, num);
  plotRelativePhases(rphase, amplitude, bias);

  std::cout << "created the two LUTs and saved them." << std::endl;
  thresholdAmplitudes(rphase, amplitude, threshold);
  float distance[2] = {220.0f, 270.0f};
  calc3DcoordinateValues(coord, mtx, distance);
  cv::FileStorage fs("LUTs.xml", cv::FileStorage::WRITE);
  fs << "LUTatNearEnd" << rphase[0];
  fs << "CoordinateValuesatNearEnd" << coord[0];
  fs << "LUTatFarEnd" << rphase[1];
  fs << "CoordinateValuesatFarEnd" << coord[1];
  fs.release();

  cv::destroyAllWindows();
  return 0;
}
// end of program

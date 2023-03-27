#include "color_curve_lib.hpp"
#include <chrono>
#include <execution>
#include <opencv2/opencv.hpp>

bool ColorCurveLib::loadImage(const std::filesystem::path& inputPath,
                              cv::Mat& img) {
  img = cv::imread(inputPath.c_str());
  if (img.data == nullptr) {
    throw std::runtime_error{"Input image contains no data:" +
                             inputPath.string()};
  }

  return true;
}

void ColorCurveLib::ImageTransformer::computeFunctionLUT() {
  const float c_1 = std::pow(parameters.t / 255, parameters.g - 1);
  const auto f_1 = [c_1](float pixelValue) { return c_1 * pixelValue; };
  const int tIndex = std::min(std::max(int(parameters.t), 0), 256);
  std::for_each(std::execution::par_unseq, LUT.begin(), LUT.begin() + tIndex,
                [this, f_1](float& value) {
                  const size_t valueIndex = &value - &LUT[0];
                  value = f_1(valueIndex);
                });
  const float c_2 = std::pow(255, 1 - parameters.g);
  const auto f_2 = [c_2, this](float pixelValue) {
    return c_2 * std::pow(pixelValue, parameters.g);
  };
  std::for_each(std::execution::par_unseq, LUT.begin() + tIndex, LUT.end(),
                [this, f_2](float& value) {
                  const size_t valueIndex = &value - &LUT[0];
                  value = f_2(valueIndex);
                });
}

void ColorCurveLib::normalizeLUT(std::array<float, 256>& lut) {
  const auto [itMinInLUT, itMaxInLUT] =
      std::minmax_element(lut.begin(), lut.end());
  const float minInLUT = *itMinInLUT;
  const float maxInLUT = *itMaxInLUT;
  std::for_each(std::execution::par_unseq, lut.begin(), lut.end(),
                [minInLUT, maxInLUT](float& transformedValue) {
                  transformedValue = std::round((transformedValue - minInLUT) /
                                                (maxInLUT - minInLUT) * 255);
                });
}

ColorCurveLib::ImageTransformer::ImageTransformer(const Parameters& parameters)
    : parameters(parameters) {
  if (parameters.loadImageFromPath.empty()) {
    throw std::runtime_error{"Input image path is empty."};
  }
  if (!std::filesystem::exists(parameters.loadImageFromPath)) {
    throw std::runtime_error{"Input file path does not exist: " +
                             parameters.loadImageFromPath.string()};
  }
  if (parameters.loadImageFromPath.extension() != ".png") {
    throw std::runtime_error{"A png image is expected as input."};
  }
  if (parameters.g < 0 || parameters.t < 0) {
    throw std::runtime_error{"g and t must be non-negative."};
  }
  if (parameters.saveImageToPath.extension() != ".png") {
    throw std::runtime_error{
        "Output image file path must have a .png extension."};
  }

  try {
    loadImage(parameters.loadImageFromPath, img);
  } catch (const std::exception& e) {
    throw;
  }

  auto executionTimeStart = std::chrono::high_resolution_clock::now();
  computeFunctionLUT();
  applyLUT(LUT, img);
  auto executionTimeEnd = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      executionTimeEnd - executionTimeStart);
  std::cout << "Processing time: " << duration.count() / 1000.0 << "ms"
            << std::endl;

  saveImage(parameters.saveImageToPath, img);
}

void ColorCurveLib::applyLUT(const std::array<float, 256>& lut, cv::Mat& I) {
  int channels = I.channels();
  int nRows = I.rows;
  int nCols = I.cols * channels;
  if (I.isContinuous()) {
    nCols *= nRows;
    nRows = 1;
  }
  tbb::parallel_for(0, nRows, 1, [&I, lut, nCols](int i) {
    uchar* p = I.ptr<uchar>(i);
    for (int j = 0; j < nCols; ++j) {
      p[j] = lut[p[j]];
    }
  });
}

bool ColorCurveLib::saveImage(const std::filesystem::path& saveToPath,
                              const cv::Mat& img) {
  if (img.empty()) {
    std::cerr << "The output image is empty." << std::endl;
    return false;
  }
  const bool savingWasSuccessful = cv::imwrite(saveToPath, img);

  if (!savingWasSuccessful) {
    std::cerr << "Saving the output image failed." << std::endl;
    return false;
  }

  return true;
}

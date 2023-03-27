#include "ImageProcessingLib.hpp"
#include <chrono>
#include <execution>
#include <opencv2/opencv.hpp>

bool ImageProcessingLib::loadImage(const std::filesystem::path& inputPath,
                                   cv::Mat& img) {
  img = cv::imread(inputPath.c_str(), cv::IMREAD_UNCHANGED);
  if (img.data == nullptr) {
    throw std::runtime_error{"Input image contains no data:" +
                             inputPath.string()};
  }

  return true;
}

void ImageProcessingLib::computeFunctionLUT(
    const std::function<void(std::array<float, 256>&)> transformationFunction,
    std::array<float, 256>& lut) {
  transformationFunction(lut);
}

void ImageProcessingLib::normalizeLUT(std::array<float, 256>& lut) {
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

ImageProcessingLib::ImageTransformer::ImageTransformer(
    const Parameters& parameters)
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
  computeFunctionLUT(parameters.function, lut);
  applyLUT(lut, img);
  auto executionTimeEnd = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      executionTimeEnd - executionTimeStart);
  std::cout << "Processing time: " << duration.count() / 1000.0 << "ms"
            << std::endl;

  saveImage(parameters.saveImageToPath, img);
}

void ImageProcessingLib::applyLUT(const std::array<float, 256>& lut,
                                  cv::Mat& I) {
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

bool ImageProcessingLib::saveImage(const std::filesystem::path& saveToPath,
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

#include <execution>
#include <iostream>
#include "color_curve_lib.hpp"

ImageProcessingLib::ImageTransformer::Parameters handleCommandLineArguments(
    int argc,
    char* argv[]) {
  if (argc != 5) {
    throw std::runtime_error{"Wrong number of arguments provided."};
  }
  ImageProcessingLib::ImageTransformer::Parameters inputParameters;
  inputParameters.loadImageFromPath = argv[1];
  const float t = std::atof(argv[2]);
  const float g = std::atof(argv[3]);
  inputParameters.saveImageToPath = argv[4];

  inputParameters.function = [g, t](std::array<float, 256>& lut) {
    const float c_1 = std::pow(t / 255, g - 1);
    const auto f_1 = [c_1](float pixelValue) { return c_1 * pixelValue; };
    const int tIndex = std::min(std::max(int(t), 0), 256);
    std::for_each(std::execution::par_unseq, lut.begin(), lut.begin() + tIndex,
                  [f_1, lut](float& value) {
                    const size_t valueIndex = &value - &lut[0];
                    value = f_1(valueIndex);
                  });
    const float c_2 = std::pow(255, 1 - g);
    const auto f_2 = [c_2, g](float pixelValue) {
      return c_2 * std::pow(pixelValue, g);
    };
    std::for_each(std::execution::par_unseq, lut.begin() + tIndex, lut.end(),
                  [f_2, lut](float& value) {
                    const size_t valueIndex = &value - &lut[0];
                    value = f_2(valueIndex);
                  });
  };
  return inputParameters;
}

int main(int argc, char* argv[]) {
  const std::string usageInfo{
      "Command line arguments:\n 1.input.png\n 2.t value\n 3.g value\n "
      "4.output.png\n"};
  ImageProcessingLib::ImageTransformer::Parameters inputParameters;
  try {
    inputParameters = handleCommandLineArguments(argc, argv);
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    std::cout << usageInfo;
    return 1;
  }

  try {
    ImageProcessingLib::ImageTransformer imageProcessor(inputParameters);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  return 0;
}

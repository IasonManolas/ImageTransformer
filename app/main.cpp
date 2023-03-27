#include <iostream>
#include "color_curve_lib.hpp"

ColorCurveLib::ImageTransformer::Parameters handleCommandLineArguments(
    int argc,
    char* argv[]) {
  if (argc != 5) {
    throw std::runtime_error{"Wrong number of arguments provided."};
  }
  ColorCurveLib::ImageTransformer::Parameters inputParameters;
  inputParameters.loadImageFromPath = argv[1];
  inputParameters.t = std::atof(argv[2]);
  inputParameters.g = std::atof(argv[3]);
  inputParameters.saveImageToPath = argv[4];

  return inputParameters;
}

int main(int argc, char* argv[]) {
  const std::string usageInfo{
      "Command line arguments:\n 1.input.png\n 2.t value\n 3.g value\n "
      "4.output.png\n"};
  ColorCurveLib::ImageTransformer::Parameters inputParameters;
  try {
    inputParameters = handleCommandLineArguments(argc, argv);
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
    std::cout << usageInfo;
    return 1;
  }

  try {
    ColorCurveLib::ImageTransformer imageProcessor(inputParameters);
  } catch (const std::exception& e) {
    std::cout << e.what();
    return 1;
  }

  return 0;
}

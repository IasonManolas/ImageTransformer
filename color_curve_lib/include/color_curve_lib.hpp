#include <filesystem>
#include <opencv2/opencv.hpp>

namespace ColorCurveLib {
class ImageTransformer {
  cv::Mat img;
  std::array<float, 256> LUT;

 public:
  struct Parameters {
    std::filesystem::path loadImageFromPath;
    std::filesystem::path saveImageToPath;
    double g;
    double t;
  };
  const Parameters parameters;
  ImageTransformer(const Parameters& parameters);

 private:
  void computeFunctionLUT();
};

bool loadImage(const std::filesystem::path& inputPath, cv::Mat& img);
bool saveImage(const std::filesystem::path& saveToPath, const cv::Mat& img);
void applyLUT(const std::array<float, 256>& lut, cv::Mat& img);
void normalizeLUT(std::array<float, 256>& lut);
}  // namespace ColorCurveLib

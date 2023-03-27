#include <filesystem>
#include <opencv2/opencv.hpp>

/*! \namespace ImageProcessingLib
   \brief containing image processing related
 * functions and classes
 */
namespace ImageProcessingLib {
/*! \class ImageTransformer
   \brief Applies a function to each pixel of an input image and
* exports the result to a provided path.
*/

class ImageTransformer {
  cv::Mat img;  //!< Data of the input and output images (transformation is done
                //!< in-place)
  std::array<float, 256> lut;  //!< A look-up-table holding the function values
                               //!< for each pixel value in the range [0,255]

 public:
  /*! \struct TransformationParameters
     \brief Holds the parameters of the transform
  */
  struct Parameters {
    std::filesystem::path
        loadImageFromPath;  //!< The path from which the input image is loaded
    std::filesystem::path
        saveImageToPath;  //!< The path to which the output image is saved
    std::function<void(std::array<float, 256>&)> function;
  };
  const Parameters parameters;
  ImageTransformer(const Parameters& parameters);

 private:
};
/*! \fn loadImage
 *  \param inputPath The path from which the image is loaded
 *  \param img The image data holder
   \brief loads an image from a given file path into a cv::Mat
   */
bool loadImage(const std::filesystem::path& inputPath, cv::Mat& img);
/*! \fn saveImage
 *  \param saveToPath The path to which the data of img is saved
 *  \param img The image data holder
   \brief Saves the data from an cv::Mat class to a given file path
 inputPath
*/
bool saveImage(const std::filesystem::path& saveToPath, const cv::Mat& img);
/*! \fn computeFunctionLUT
 *  \param transformationFunction the function which shall be applied to the
 * image
 * \param the look-up-table computed from the given function
 */
void computeFunctionLUT(
    const std::function<void(std::array<float, 256>&)> transformationFunction,
    std::array<float, 256>& lut);
/*! \fn applyLUT
 *  \param lut an array holding the values of a function for each possible pixel
 value in the range [0,255]
 *  \param img The image to which the lut is applied
   \brief Applies a function via a look-up-table(lut) to given image data
 contained in img.
*/
void applyLUT(const std::array<float, 256>& lut, cv::Mat& img);
/*! \fn normalizeLUT
 *  \param lut the array to be normalized
   \brief Normalizes the values of lut such that they are in the range [0,255]
   */
void normalizeLUT(std::array<float, 256>& lut);
}  // namespace ImageProcessingLib

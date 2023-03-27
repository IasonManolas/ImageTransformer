#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include "ImageProcessingLib.hpp"

TEST_CASE("Load an image") {
  cv::Mat img;
  REQUIRE(
      ImageProcessingLib::loadImage(
          std::filesystem::path(__FILE__).parent_path().append("monoLena.png"),
          img) == true);
}

TEST_CASE("Save an image") {
  cv::Mat savedImg(5, 5, 0);
  const std::filesystem::path imgFilePath =
      std::filesystem::path(__FILE__).parent_path().append("black.png");
  ImageProcessingLib::saveImage(imgFilePath, savedImg);
  cv::Mat loadedImg;
  ImageProcessingLib::loadImage(imgFilePath, loadedImg);
  REQUIRE(savedImg.rows == loadedImg.rows);
  REQUIRE(savedImg.cols == loadedImg.cols);
  REQUIRE(savedImg.channels() == loadedImg.channels());
  cv::Mat diff;
  cv::compare(savedImg, loadedImg, diff, cv::CMP_NE);

  int nz = cv::countNonZero(diff);

  REQUIRE(nz == 0);
}

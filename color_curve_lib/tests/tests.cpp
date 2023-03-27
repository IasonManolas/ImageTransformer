#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include "color_curve_lib.hpp"

TEST_CASE("Read an input image") {
  cv::Mat img;
  REQUIRE(
      ColorCurveLib::loadImage(
          std::filesystem::path(__FILE__).parent_path().append("monoLena.png"),
          img) == true);
}

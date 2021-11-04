#include "HololuxScreenshot.h"
#include <iostream>
#include <imgpp/loadersext.hpp>

bool CheckImage(const char *image_file_name, float angle_in_radian, HololuxScreenshot &app) {
  imgpp::Img image;
  if (!imgpp::Load(image_file_name, image, true)) {
    std::cerr << "Failed to load ground truth image" << std::endl;
    return false;
  }

  uint32_t width = image.ROI().Width();
  uint32_t height = image.ROI().Height();
  std::vector<uint8_t> data(width * height * 4);

  auto result = app.getImageByAngle(
    data.data(), (int)width, (int)height, angle_in_radian, {0.0f, 0.0f, 0.0f, 0.0f}
  );
  if (result != 0) {
    return false;
  }

  auto expected = image.Data().GetBuffer();
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i] != expected[i]) {
      std::cerr << "Wrong pixel at " << i / 4 << " for " << image_file_name << std::endl;

      imgpp::ImgROI roi(data.data(), width, height, 4, 8, width * 4, false, false);
      if (!imgpp::Write("rendered.png", roi, true)) {
        std::cerr << "Failed to write rendered image" << std::endl;
      } else {
        std::cerr << "Rendered image written to rendered.png" << std::endl;
      }

      return false;
    }
  }

  return true;
}

int main() {
  HololuxScreenshot app;

  auto result = app.init();
  if (result != 0) {
    return result;
  }

  result = app.loadModel("data/Adidas_shoe_43287072668.hlf");
  if (result != 0) {
    return result;
  }

  result = app.setCamera(0.0f, 0.0f, 0.6f, glm::radians(40.0f));
  if (result != 0) {
    return result;
  }

  if (!CheckImage("data/expected30.png", glm::radians(30.0f), app)) {
    return -1;
  }

  if (!CheckImage("data/expected60.png", glm::radians(60.0f), app)) {
    return -1;
  }

  result = app.loadModel("data/single_test.hlf");
  if (result != 0) {
    return result;
  }

  result = app.setCamera(0.0f, 0.0f, 0.6f, glm::radians(40.0f));
  if (result != 0) {
    return result;
  }
  
  if (!CheckImage("data/expected30_single.png", glm::radians(30.0f), app)) {
    return -1;
  }

  if (!CheckImage("data/expected60_single.png", glm::radians(60.0f), app)) {
    return -1;
  }

  return 0;
}

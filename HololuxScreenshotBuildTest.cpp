#include "IHololuxScreenshot.h"
#include <vector>
#include <memory>

int main() {
  auto app = std::unique_ptr<IHololuxScreenshot>(IHololuxScreenshot::create());
  auto result = app->init();
  if (result != 0) {
    return result;
  }
  result = app->loadModel("data/Adidas_shoe_43287072668.hlf");
  if (result != 0) {
    return result;
  }
  std::vector<uint8_t> data(4);
  result = app->getImageByAngle(data.data(), 1, 1, 0.0f, {0.0f, 0.0f, 0.0f, 0.0f});
  if (result != 0) {
    return result;
  }
  return 0;
}

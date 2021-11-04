#include "IHololuxScreenshot.h"
#include <vector>
#include <memory>

int main() {
  auto app = std::unique_ptr<IHololuxScreenshot>(IHololuxScreenshot::create());
  app->init();
  app->loadModel("data/Adidas_shoe_43287072668.hlf");
  std::vector<uint8_t> data(4);
  app->getImageByAngle(data.data(), 1, 1, 0.0f, {0.0f, 0.0f, 0.0f, 0.0f});
}

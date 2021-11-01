#pragma once

#include <HololuxRenderer.h>
#include "MineolaHelper.h"

class HololuxScreenshot {
public:
  HololuxScreenshot() = default;

  // Initialize the render engine. Return 0 upon success.
  int init();

  // Load Hololux LightField model. Return 0 upon success.
  int loadModel(const char *modelFile);

  // Set camera position and fov. Return 0 upon success.
  int setCamera(float x, float y, float z, float fovYInRadian);

  // Rotate the model by [angleInRadian] around world Y axis from the initial pose,
  // set the clear color (RGBA, [0.0 - 1.0]),
  // and write resulting image to [buffer].
  // Image will always be width x height x 4, in scanline order.
  // Buffer size is assumed to be at least width*height*4.
  // Return 0 upon success.
  int getImageByAngle(
    uint8_t *buffer,
    int width,
    int height,
    float angleInRadian,
    std::array<float, 4> clear_color
  );

  // Destroy all resources.
  ~HololuxScreenshot();

private:
  friend void HololuxScreenshotOnDownloadSuccess(void *app_ctx);
  friend void HololuxScreenshotOnDownloadError(void *app_ctx);

private:
  MineolaHelper helper_;
  PHOLOLUXRENDERER renderer_{nullptr};
  bool download_successful_{false};
  std::shared_ptr<mineola::SceneNode> model_node_;
  mineola::math::Rbt initial_pose_;
  uint32_t current_width_{0};
  uint32_t current_height_{0};
};

#pragma once

#include <cstdint>
#include <array>

class IHololuxScreenshot {
public:
  // Initialize the render engine. Return 0 upon success.
  virtual int init() = 0;

  // Load Hololux LightField model. Return 0 upon success.
  virtual int loadModel(const char *modelFile) = 0;

  // Set camera position and fov. Return 0 upon success.
  virtual int setCamera(float x, float y, float z, float fovYInRadian) = 0;

  // Rotate the model by [angleInRadian] around world Y axis from the initial pose,
  // set the clear color (RGBA, [0.0 - 1.0]),
  // and write resulting image to [buffer].
  // Image will always be width x height x 4, in scanline order.
  // Buffer size is assumed to be at least width*height*4.
  // Return 0 upon success.
  virtual int getImageByAngle(
    uint8_t *buffer,
    int width,
    int height,
    float angleInRadian,
    std::array<float, 4> clear_color
  ) = 0;

  // Destroy all resources.
  virtual ~IHololuxScreenshot() = default;

  static IHololuxScreenshot *create();
};



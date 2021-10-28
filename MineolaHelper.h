#pragma once

#include "EGLHelper.h"
#include <mineola/glutility.h>
#include <mineola/Engine.h>
#include <mineola/Framebuffer.h>
#include <mineola/GLEffect.h>
#include <mineola/RenderPass.h>
#include <mineola/TextureHelper.h>
#include <mineola/GraphicsBuffer.h>
#include <mineola/SceneNode.h>

class MineolaHelper {
public:
  MineolaHelper() = default;
  bool Initialize();
  ~MineolaHelper();

private:
  EGLHelper egl_;
};

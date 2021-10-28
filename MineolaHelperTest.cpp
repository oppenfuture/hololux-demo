#include "MineolaHelper.h"
#include <iostream>

using namespace mineola;

auto vs = R"(#version 320 es
void main() {
  gl_Position = vec4(0.0);
}
)";

auto fs = R"(#version 320 es
void main() {}
)";

bool Test(glm::vec4 clear_color, std::array<uint8_t, 4> expected) {
  MineolaHelper helper;
  if (!helper.Initialize()) {
    return false;
  }

  uint32_t width = 256, height = 256;
  if (!texture_helper::CreateRenderTarget(
    "rt", 256, 256, 24, false, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE
  )) {
    std::cerr << "CreateRenderTarget failed" << std::endl;
    return false;
  }

  auto &en = Engine::Instance();
  auto fb = bd_cast<InternalFramebuffer>(en.ResrcMgr().Find("rt"));
  if (!fb) {
    std::cerr << "Failed to find framebuffer" << std::endl;
    return false;
  }
  en.SetFramebuffer("rt", true);

  if (!CreateEffectFromMemHelper("effect", vs, fs, nullptr, {})) {
    std::cerr << "CreateEffectFromMemHelper failed" << std::endl;
    return false;
  }

  RenderPass pass;
  en.RenderPasses().push_back(pass);

  en.RenderStateMgr().SetClearColor(clear_color);
  en.FrameMove();
  en.Render();

  auto &pbo = fb->GetReadPBO();
  fb->ReadBack(Framebuffer::AT_COLOR0);
  uint8_t *data = (uint8_t*)pbo.Map();
  for (uint32_t i = 0; i < width * height; ++i) {
    if (data[0] != expected[0] || data[1] != expected[1] ||
        data[2] != expected[2] || data[3] != expected[3]) {
      std::cerr << "Wrong pixel at " << i << std::endl;
      return false;
    }
    data += 4;
  }
  pbo.Unmap();

  return true;
}

int main() {
  if (!Test({1.0f, 0.0f, 0.0f, 1.0f}, {255, 0, 0, 255})) {
    return 1;
  }

  if (!Test({0.0f, 1.0f, 1.0f, 0.0f}, {0, 255, 255, 0})) {
    return 1;
  }

  return 0;
}

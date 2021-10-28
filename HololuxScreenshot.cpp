#include "HololuxScreenshot.h"
#include <iostream>
#include <fstream>

using namespace mineola;

namespace {

void GetFileSize(const HLHttpRequest *request, void *app_ctx,
  HLHttpResponseProc on_success, HLHttpResponseProc on_error) {
  HLHttpResponse response;
  std::ifstream in(request->url, std::ifstream::ate | std::ifstream::binary);
  response.length = in.tellg();
  response.error = HLNET_ERR_NONE;
  response.hl_ctx = request->hl_ctx;
  on_success(&response);
}

void GetFileRange(const HLHttpRequest *request, void *app_ctx,
  HLHttpResponseProc on_success, HLHttpResponseProc on_error) {
  HLHttpResponse response;
  std::ifstream in(request->url, std::ifstream::binary);
  in.seekg(0, in.end);
  uint64_t file_size = in.tellg();
  response.hl_ctx = request->hl_ctx;
  response.offset = request->offset;
  response.length = request->length;
  if (file_size < request->offset + request->length) {
    // For exmaple, request range out of file size error
    on_error(&response);
  } else {
    // TODO: Implementatio with http range
    std::vector<char> buffer(request->length);
    in.seekg(request->offset);
    in.read(buffer.data(), request->length);
    in.close();
    response.data = buffer.data();
    response.error = HLNET_ERR_NONE;
    on_success(&response);
  }
}

void CancelAllRequests(void *app_ctx) {

}

}

void HololuxScreenshotOnDownloadSuccess(void *app_ctx) {
  HololuxScreenshot *app = reinterpret_cast<HololuxScreenshot*>(app_ctx);
  app->download_successful_ = true;
}

void HololuxScreenshotOnDownloadError(void *app_ctx) {
  HololuxScreenshot *app = reinterpret_cast<HololuxScreenshot*>(app_ctx);
  app->download_successful_ = false;
}

int HololuxScreenshot::init() {
  if (!helper_.Initialize()) {
    return -1;
  }

  auto result = HLCreateRenderer(&renderer_);
  if (result != 0) {
    std::cerr << "HLCreateRenderer failed" << std::endl;
    return result;
  }

  result = HLCreateScene(renderer_);
  if (result != 0) {
    std::cerr << "HLCreateScene failed" << std::endl;
    return result;
  }

  return 0;
}

int HololuxScreenshot::loadModel(const char *modelFile) {
  HLNetworkInterfaces netapis;
  netapis.get_file_size = GetFileSize;
  netapis.get_file_range = GetFileRange;
  netapis.cancel_all_requests = CancelAllRequests;
  HLDownloadLightField(
    renderer_, modelFile, this, &netapis,
    HololuxScreenshotOnDownloadSuccess, HololuxScreenshotOnDownloadError
  );

  if (!download_successful_) {
    std::cerr << "HLDownloadLightField failed" << std::endl;
    return -1;
  }

  auto result = HLCreateLightField(renderer_);
  if (result != 0) {
    std::cerr << "HLCreateLightField failed" << std::endl;
    return -1;
  }

  return 0;
}

std::ostream &operator<<(std::ostream &os, const math::Rbt &rbt) {
  os << "{ t {" << rbt.Translation().x << ", " << rbt.Translation().y << ", "
    << rbt.Translation().z << "}, r {" << rbt.Rotation().x << ", " << rbt.Rotation().y
    << ", " << rbt.Rotation().z << ", " << rbt.Rotation().w << "} }";
  return os;
}

int HololuxScreenshot::setCamera(float x, float y, float z, float fovYInRadian) {
  auto &en = Engine::Instance();

  auto node = SceneNode::FindNodeByName(HLGetCameraNodeName(renderer_), en.Scene().get());
  if (!node) {
    std::cerr << "Failed to find camera node" << std::endl;
    return -1;
  }

  auto rbt = math::Rbt::LookAt({x, y, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
  node->SetWorldRbt(rbt);

  auto camera = bd_cast<Camera>(en.CameraMgr().Find(HLGetCameraName(renderer_)));
  if (!camera) {
    std::cerr << "Failed to find camera" << std::endl;
    return -1;
  }

  camera->SetFov(fovYInRadian);

  return 0;
}

int HololuxScreenshot::getImageByAngle(
  uint8_t *buffer,
  int width,
  int height,
  float angleInRadian,
  std::array<float, 4> clear_color
) {
  auto &en = Engine::Instance();
  auto node = SceneNode::FindNodeByName(HLGetModelNodeName(renderer_), en.Scene().get());
  if (!node) {
    std::cerr << "Failed to find model node" << std::endl;
    return -1;
  }

  auto halfAngle = angleInRadian * 0.5f;
  auto rotation = math::Rbt(glm::quat(std::cos(halfAngle), 0.0f, std::sin(halfAngle), 0.0f));
  node->SetWorldRbt(math::Rbt::DoMToOWrtA(rotation, node->WorldRbt(), math::Rbt()));

  const char *rt_name = "rt:hololux:screenshot";
  if (current_width_ != (uint32_t)width || current_height_ != (uint32_t)height) {
    if (!texture_helper::CreateRenderTarget(
      rt_name, (uint32_t)width, (uint32_t)height, 24, false,
      GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE
    )) {
      std::cerr << "CreateRenderTarget failed" << std::endl;
      return -1;
    }

    current_width_ = (uint32_t)width;
    current_height_ = (uint32_t)height;
  }

  auto fb = bd_cast<InternalFramebuffer>(en.ResrcMgr().Find(rt_name));
  if (!fb) {
    std::cerr << "Failed to find render target " << rt_name << std::endl;
    return -1;
  }

  en.SetFramebuffer(rt_name, true);
  en.RenderStateMgr().SetClearColor(
    {clear_color[0], clear_color[1], clear_color[2], clear_color[3]}
  );
  en.FrameMove();
  HLOnPreRender(renderer_);
  en.Render();

  auto &pbo = fb->GetReadPBO();
  fb->ReadBack(Framebuffer::AT_COLOR0);
  void *data = pbo.Map();
  std::memcpy(buffer, data, current_width_ * current_height_ * 4);
  pbo.Unmap();

  return 0;
}

HololuxScreenshot::~HololuxScreenshot() {
  HLDestroyRenderer(renderer_);
}

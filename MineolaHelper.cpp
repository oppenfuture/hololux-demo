#include "MineolaHelper.h"
#include <iostream>

bool MineolaHelper::Initialize() {
  if (!egl_.Initialize(EGL_OPENGL_ES_API)) {
    return false;
  }

  if (!gladLoadGLES2Loader((GLADloadproc)eglGetProcAddress)) {
    std::cerr << "gladLoadGLES2Loader failed" << std::endl;
    return false;
  }

  auto &en = mineola::Engine::Instance();
  en.Init();
  en.Start();

  return true;
}

MineolaHelper::~MineolaHelper() {
  mineola::Engine::Instance().Release();
}

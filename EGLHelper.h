#pragma once

#include <glad/egl.h>

class EGLHelper {
public:
  EGLHelper() = default;
  // Specifies the client API to bind, one of EGL_OPENGL_API, EGL_OPENGL_ES_API, or EGL_OPENVG_API.
  bool Initialize(EGLint api);
  ~EGLHelper();

  void SwapBuffers();

private:
  EGLDisplay display_{EGL_NO_DISPLAY};
  EGLSurface surface_{EGL_NO_SURFACE};
};

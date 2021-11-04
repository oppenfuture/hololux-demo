#include <iostream>
#include "EGLHelper.h"

bool EGLHelper::Initialize(EGLint api) {
  EGLint renderable_type = EGL_OPENGL_BIT;
  if (api == EGL_OPENGL_API) {
    renderable_type = EGL_OPENGL_BIT;
  } else if (api == EGL_OPENGL_ES_API) {
    renderable_type = EGL_OPENGL_ES2_BIT;
  } else if (api == EGL_OPENVG_API) {
    renderable_type = EGL_OPENVG_BIT;
  } else {
    std::cerr << "Unknown api " << api << std::endl;
    return false;
  }

  if (api == EGL_OPENGL_API) {
    std::cerr << "OpenGL API not supported" << std::endl;
    return false;
  }

  const EGLint config_attribs[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 24,
    EGL_RENDERABLE_TYPE, renderable_type,
    EGL_NONE
  };

  // We'll create fbos for rendering, so the surface size doesn't matter.
  static const EGLint pbuffer_attribs[] = {
    EGL_WIDTH, (EGLint)1,
    EGL_HEIGHT, (EGLint)1,
    EGL_NONE
  };

  EGLint api_major_version = 1;
  EGLint api_minor_version = 0;
  if (api == EGL_OPENGL_API) {
    api_major_version = 4;
    api_minor_version = 6;
  } else if (api == EGL_OPENGL_ES_API) {
    api_major_version = 3;
    api_minor_version = 2;
  }

  const EGLint context_attribs[] = {
    EGL_CONTEXT_MAJOR_VERSION, api_major_version,
    EGL_CONTEXT_MINOR_VERSION, api_minor_version,
    EGL_NONE,
  };

  int version = gladLoaderLoadEGL(EGL_NO_DISPLAY);
  if (version == 0) {
    std::cerr << "gladLoaderLoadEGL(NULL) failed" << std::endl;
    return false;
  }

  EGLDeviceEXT device;
  EGLint num_devices = 0;
  auto status = eglQueryDevicesEXT(1, &device, &num_devices);
  if (status == EGL_FALSE) {
    std::cerr << "eglQueryDevicesEXT failed" << std::endl;
    return false;
  }

  display_ = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, device, nullptr);
  if (display_ == EGL_NO_DISPLAY) {
    std::cerr << "eglGetDisplay failed" << std::endl;
    return false;
  }

  EGLint major = 0, minor = 0;
  status = eglInitialize(display_, &major, &minor);
  if (status == EGL_FALSE) {
    std::cerr << "eglInitialize failed" << std::endl;
    return false;
  }

  version = 0;
  version = gladLoaderLoadEGL(display_);
  if (version == 0) {
    std::cerr << "gladLoaderLoadEGL(display_) failed" << std::endl;
    return false;
  }

  EGLint num_configs = 0;
  EGLConfig config;
  status = eglChooseConfig(display_, config_attribs, &config, 1, &num_configs);
  if (status == EGL_FALSE || num_configs == 0) {
    std::cerr << "eglChooseConfig failed" << std::endl;
    return false;
  }

  surface_ = eglCreatePbufferSurface(display_, config, pbuffer_attribs);
  if (surface_ == EGL_NO_SURFACE) {
    std::cerr << "eglCreatePbufferSurface failed" << std::endl;
    return false;
  }

  status = eglBindAPI(api);
  if (status == EGL_FALSE) {
    std::cerr << "eglBindAPI failed" << std::endl;
    return false;
  }

  auto context = eglCreateContext(display_, config, EGL_NO_CONTEXT, context_attribs);
  if (context == EGL_NO_CONTEXT) {
    std::cerr << "eglCreateContext failed" << std::endl;
    return false;
  }

  status = eglMakeCurrent(display_, surface_, surface_, context);
  if (status == EGL_FALSE) {
    std::cerr << "eglMakeCurrent failed" << std::endl;
    return false;
  }

  return true;
}

EGLHelper::~EGLHelper() {
  if (display_ != EGL_NO_DISPLAY) {
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (surface_ != EGL_NO_SURFACE) {
      eglDestroySurface(display_, surface_);
    }
    eglTerminate(display_);
  }
}

void EGLHelper::SwapBuffers() {
  eglSwapBuffers(display_, surface_);
}

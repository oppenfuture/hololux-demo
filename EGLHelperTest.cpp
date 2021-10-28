#include <iostream>
#include "EGLHelper.h"

struct GLInfo {
  std::string vendor;
  std::string renderer;
  std::string version;
  std::string sl_version;
};

typedef const unsigned char* glGetStringT(unsigned int name);
GLInfo GetGLInfo() {
  auto glGetString = (glGetStringT*)eglGetProcAddress("glGetString");
  if (!glGetString) {
    std::cerr << "Failed to find glGetString" << std::endl;
    std::exit(1);
  }

  GLInfo info;
  info.vendor = (const char*)glGetString(0x1F00);
  info.renderer = (const char*)glGetString(0x1F01);
  info.version = (const char*)glGetString(0x1F02);
  info.sl_version = (const char*)glGetString(0x8B8C);
  return info;
}

std::ostream& operator<<(std::ostream& os, const GLInfo &info) {
  os << info.vendor << " @ " << info.renderer
    << ", Version " << info.version << " / " << info.sl_version;
  return os;
}

int main() {
  // Test OpenGL context.
  {
    EGLHelper helper;
    if (!helper.Initialize(EGL_OPENGL_API)) {
      return 1;
    } else {
      std::cerr << "Created OpenGL API: " << GetGLInfo() << std::endl;
    }
  }
  // Test OpenGL ES context.
  {
    EGLHelper helper;
    if (!helper.Initialize(EGL_OPENGL_ES_API)) {
      return 1;
    } else {
      std::cerr << "Created OpenGL ES API: " << GetGLInfo() << std::endl;
    }
  }
  return 0;
}

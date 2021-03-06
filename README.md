# `hololux` SDK示例工程

本工程用于展示如何使用`hololux`在`Linux`服务器环境下将光场渲染到纹理，并读回渲染结果。

## 运行环境

- `Linux x86_64`
- `libstdc++`
- 支持`EGL`和`OpenGL ES 3.2`的显卡驱动
- `libhololux.so`
- `libturbojpeg.so.0`

如果当前硬件和系统支持`EGL`和`OpenGL ES`，`/usr/lib/x86_64-linux-gnu/`下一般会有以下文件：
- `libEGL.so`
- `libGLESv2.so`

`Debian`系列安装EGL：
```bash
sudo apt install libegl1-mesa
```

`libhololux.so`和`libturbojpeg.so.0`位于`dependencies/lib/`下，需要能在运行时被搜索到。

`dependencies/`下的其他文件只用于构建测试程序，并不是运行时依赖。

## 构建和测试示例工程

### 开发依赖

- `gcc 8`
- `CMake >= 3.16`
- `make`

### 构建和测试

```bash
mkdir build && cd build
cmake ..
make -j
make test
```

## 工程结构

本工程主要提供了`IHololuxScreenshot`类，它封装了`EGL`、渲染引擎和`hololux`渲染器的创建和销毁，支持从本地加载光场模型文件，并将渲染结果读回内存。

`IHololuxScreenshot`对象在析构时会释放所有图形资源，包括`EGLContext`和`EGLDisplay`。因此，用户只应该创建一个`IHololuxScreenshot`对象，直到上一个对象被销毁之后才可以创建新的对象。

## 集成到现有工程

仓库根目录下的以下文件需要被加入`include`路径：

- `IHololuxScreenshot.h`

`artifact/`下的以下文件需要被链接到目标二进制文件：

- `libhololux_demo.so`

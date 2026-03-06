# qloongify
RK3588 Buildroot &amp; ffmpeg 4.4.4 &amp; qt5.15.11

```shell
Attribute Qt::AA_UseOpenGLES must be set before QCoreApplication is created.
=========================================
OpenGL ES Test on RK3588 - Mali-G610 MC4
=========================================
Vendor:  ARM
Renderer:  Mali-G610
OpenGL Version:  OpenGL ES 3.2 v1.g13p0-01eac0.9113fb3d6cbac82527a272992153edf9
GLSL Version:  OpenGL ES GLSL ES 3.20
Extensions available: Yes
 - GL_OES_EGL_image: Supported
Max Texture Size:  16383
Max Vertex Attributes:  32
=========================================
```

# 设置路径

export PATH="/com/wangyonglin/qloongify/usr/bin:${PATH}"
export LD_LIBRARY_PATH="/com/wangyonglin/qloongify/usr/lib:/com/wangyonglin/qloongify/usr/lib64:${LD_LIBRARY_PATH}"

# ./build.sh lunch
# Buildroot 6.1
``` shell
＃＃＃ ARM Mali-G610 MC4；OpenGL ES 1.1/2.0/3.1/3.2；Vulkan 1.1/1.2  
## System configuration
    (rockchip) Root password
## Text and terminal handling
 -*- pcre2 
    [ * ]   16-bit pcre2
    [ * ]   32-bit pcre2
FFmpeg
paho-mqtt-c                                                                                                          
 [*] paho-mqtt-cpp 
 
 ln -sf libmali-valhall-g610-g13p0-gbm.so libEGL.so.1
 ln -sf libmali-valhall-g610-g13p0-gbm.so libGLESv2.so.2
 ln -sf libmali-valhall-g610-g13p0-gbm.so libgbm.so.1
 ln -sf libmali-valhall-g610-g13p0-gbm.so libmali.so.1
```

## FQA&
    
    qt5.15 编译问题为  : libQt5SherpaOnnx.so: undefined reference to `std::ios_base_library_init()'
    原因：nm -D ./usr/lib/libstdc++.so | grep ios_base_library_init 没有找到 ios_base_library_init
    解决方法：
    Toolchain → C++ support 必须启用
    Toolchain → GCC compiler Version 选择较新的版本
    Toolchain → Enable C++ support 确保选中
    
    qt5.15 开发连接不上开发板 rsync 错误 
    buildroot 要开启rsync 和 librsync

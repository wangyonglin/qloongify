# 设置路径

export PATH="/com/wangyonglin/qloongify/usr/bin:${PATH}"
export LD_LIBRARY_PATH="/com/wangyonglin/qloongify/usr/lib:/com/wangyonglin/qloongify/usr/lib64:${LD_LIBRARY_PATH}"

# ./build.sh lunch
# Buildroot 6.1
``` shell
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
    W＠
    

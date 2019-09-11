FFmpeg README
=============

FFmpeg is a collection of libraries and tools to process multimedia content
such as audio, video, subtitles and related metadata.

## Build For Cronet
### Cronet
Cronet是Chromium的网络库，封装了HTTP/HTTP2/QUIC等协议。在我的博客文章[FFmpeg支持QUIC](https://blog.csdn.net/sonysuqin/article/details/89022250)中描述了在Chromium代码中增加一个bequic模块、编译、集成到FFmpeg中的过程。编译Cronet过程也类似，搭建好Chromium环境后运行以下命令即可。
```
ninja -c cronet_package
```
Cronet集成到FFmpeg中的方法与bequic模块类似，这里也采用了添加协议的办法，其前置步骤仍然可以参考[FFmpeg支持QUIC](https://blog.csdn.net/sonysuqin/article/details/89022250)中FFmpeg编译相关的章节。

### Windows
```
mkdir build
cd build
../configure --prefix=/INSTALL/PATH --disable-static --enable-shared \
             --enable-gpl --enable-version3 --enable-sdl --disable-mmx \
             --disable-stripping --arch=x86 --enable-libcronet \
             --extra-cflags=-I/CRONET/INCLUDE/PATH \
             --extra-ldflags=-L/CRONET/LIBRARY/PATH \
             --extra-libs=-lcronet.73.0.3683.75
make
make install
```

### Android
* Edit build_android.sh, modify cronet include and library search path.
* Then run command:
```
./build_android.sh
```

### iOS
* Follow [FFmpeg-iOS-build-script](https://github.com/kewlbear/FFmpeg-iOS-build-script), install gas-preprocessor and yasm;
* Copy build_ios.sh out from source directory:
```
copy build_ios.sh ..
```
* Edit build_ios.sh, modify cronet include and library search path.
* Then run command:
```
./build_ios.sh arm64
```

## Libraries

* `libavcodec` provides implementation of a wider range of codecs.
* `libavformat` implements streaming protocols, container formats and basic I/O access.
* `libavutil` includes hashers, decompressors and miscellaneous utility functions.
* `libavfilter` provides a mean to alter decoded Audio and Video through chain of filters.
* `libavdevice` provides an abstraction to access capture and playback devices.
* `libswresample` implements audio mixing and resampling routines.
* `libswscale` implements color conversion and scaling routines.

## Tools

* [ffmpeg](https://ffmpeg.org/ffmpeg.html) is a command line toolbox to
  manipulate, convert and stream multimedia content.
* [ffplay](https://ffmpeg.org/ffplay.html) is a minimalistic multimedia player.
* [ffprobe](https://ffmpeg.org/ffprobe.html) is a simple analysis tool to inspect
  multimedia content.
* Additional small tools such as `aviocat`, `ismindex` and `qt-faststart`.

## Documentation

The offline documentation is available in the **doc/** directory.

The online documentation is available in the main [website](https://ffmpeg.org)
and in the [wiki](https://trac.ffmpeg.org).

### Examples

Coding examples are available in the **doc/examples** directory.

## License

FFmpeg codebase is mainly LGPL-licensed with optional components licensed under
GPL. Please refer to the LICENSE file for detailed information.

## Contributing

Patches should be submitted to the ffmpeg-devel mailing list using
`git format-patch` or `git send-email`. Github pull requests should be
avoided because they are not part of our review process and will be ignored.

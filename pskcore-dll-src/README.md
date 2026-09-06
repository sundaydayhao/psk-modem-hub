# pskcore-dll-121-src

PSK31Core / PSKCore 的底层算法与 DLL 工程源码。

## 简介

本项目实现 PSK31/BPSK/QPSK 的收发核心逻辑，包含音频输入处理、FFT、解调、调制、AFC、波形处理和 DLL 导出接口等模块，可作为 WinPSK 等上层程序的底层通信库。

## 主要文件

- `PskDet.cpp` / `PskDet.h`：接收与解调核心
- `PSKMod.cpp` / `PSKMod.h`：发送与调制核心
- `fft.cpp` / `fft.h`：FFT 实现
- `Sound.cpp` / `Sound.h`：声卡输入输出处理
- `Wave.cpp` / `Wave.h`：波形文件处理
- `IOCntrl.cpp` / `IOCntrl.h`：流程控制和线程调度
- `PSKCore.cpp` / `PSKCore.h`：DLL 入口、导出接口和核心 API
- `PskCoreExplicit.h` / `PskCoreImplicit.h`：显式/隐式 DLL 链接接口头文件

## 工程文件

仓库保留了旧版 Visual Studio 工程文件：

- `PSK31Core.sln`
- `PSKCoreDLL/PSKCoreDLL.vcproj`
- `PSKCoreLib/PSKCoreLib.vcproj`
- `PSKCoreDLL/*.dsp` / `*.dsw`
- `PSKCoreLib/*.dsp`

## 构建

可优先尝试使用 Visual Studio 打开 `PSK31Core.sln` 进行构建。由于工程年代较早，现代 Visual Studio 可能需要升级工程文件或调整 Windows SDK / 编译器设置。

也可以参考 `BuildAll.bat` 中的原始构建流程。

## 版本历史

原始版本历史见 `ReadMe.txt`。

## 目录说明

更详细的中文目录说明见 `目录说明.md`。

# PSK Decode Hub

这个仓库用于集中同步和管理多个 PSK 调制、解调、解码相关项目，方便在同一个工作区中查看、构建和对比不同平台上的实现。

当前仓库作为汇总入口，嵌入式平台项目通过 Git 子模块维护，桌面端源码和测试脚本直接保存在本仓库中。

## 子模块

| 路径 | 仓库 | 说明 |
| --- | --- | --- |
| `esp32c3-psk-modem` | [esp32c3-psk-modem](https://github.com/sundaydayhao/esp32c3-psk-modem) | ESP32-C3 平台 PSK modem |
| `esp32p4-psk-modem` | [esp32p4-psk-modem](https://github.com/sundaydayhao/esp32p4-psk-modem) | ESP32-P4 平台 PSK modem |
| `esp32s3-psk-modem` | [esp32s3-psk-modem](https://github.com/sundaydayhao/esp32s3-psk-modem) | ESP32-S3 平台 PSK modem |
| `hpm6750-psk-modem` | [hpm6750-psk-modem](https://github.com/sundaydayhao/hpm6750-psk-modem) | HPM6750 平台 PSK modem |
| `stm32f7-psk-modem` | [stm32f7-psk-modem](https://github.com/sundaydayhao/stm32f7-psk-modem) | STM32F7 平台 PSK modem |
| `stm32h7-psk-modem` | [stm32h7-psk-modem](https://github.com/sundaydayhao/stm32h7-psk-modem) | STM32H7 平台 PSK modem |

## 本仓库内的源码和脚本

| 路径 | 说明 |
| --- | --- |
| [nuepsk_src](nuepsk_src/) | NUE-PSK 相关源码 |
| [pskcore-dll-src](pskcore-dll-src/) | PSK Core DLL 源码 |
| [win-psk-src](win-psk-src/) | Windows PSK 源码 |
| [psk-script](psk-script/) | PSK 测试音频生成脚本及示例文件 |

## 使用方式

克隆本仓库并同步所有子模块：

```bash
git clone --recurse-submodules https://github.com/sundaydayhao/psk-modem-hub.git
```

如果已经克隆了本仓库，在仓库目录中拉取更新并同步子模块地址（包括从 Gitee 迁移后的地址变更）：

```bash
git pull --ff-only
git submodule sync --recursive
git submodule update --init --recursive
```

以上命令检出本仓库记录的子模块提交。私有子模块需要相应的 GitHub 访问权限。

如果需要更新所有子模块到各自远端分支的最新提交，可以执行：

```bash
git submodule update --remote --recursive
```

## 项目目的

本项目用于把 PSK 相关的嵌入式、桌面端、核心库和 Python 实现放在一个统一入口中，便于后续进行：

- PSK 调制/解调算法研究
- 不同实现之间的行为对比
- 解码链路验证和测试
- 跨平台代码整理与迁移
- 文档、实验记录和工具脚本集中管理

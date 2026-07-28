# PSK Decode Hub

这个仓库用于集中同步和管理多个 PSK 调制、解调、解码相关项目，方便在同一个工作区中查看、构建和对比不同平台上的实现。

当前仓库本身作为汇总入口，具体源码通过 Git 子模块维护。

## 子模块

| 路径 | 仓库 | 说明 |
| --- | --- | --- |
| `esp32-psk-modem` | https://gitee.com/zhanghaogit/esp32-psk-modem | ESP32 平台上的 PSK modem 相关实现 |
| `nuepsk_source` | https://gitee.com/zhanghaogit/nuepsk_source | NUE-PSK 相关源码 |
| `pskcore-dll-121-src` | https://gitee.com/zhanghaogit/pskcore-dll-121-src | PSK Core DLL 1.21 源码 |
| `win-psk-src-213` | https://gitee.com/zhanghaogit/win-psk-src-213 | Windows PSK 2.13 源码 |
| `psk-python` | https://gitee.com/zhanghaogit/psk-python | Python 版 PSK 相关代码 |

## 使用方式

克隆本仓库并同步所有子模块：

```bash
git clone --recurse-submodules <this-repo-url>
```

如果已经克隆了本仓库，可以执行：

```bash
git submodule update --init --recursive
```

更新所有子模块到各自远端分支的最新提交：

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

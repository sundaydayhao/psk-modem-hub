# 源码引用说明

本次整理将源码副本替换为仓库引用，核对日期为 2026-11-12。

## WinPSK 与 PSKCore

[swharden/WinPSK](https://github.com/swharden/WinPSK) 收录了 Moe Wheatley（AE4JY）的 WinPSK 客户端和 PSKCore DLL，来源说明指向 [WinPSK 作者网站](https://www.moetronix.com/ae4jy/winpsk.htm)。两部分使用同一个 `winpsk` 子模块。

选定提交：`5309bc95e8467e8fdda55deac057273137c8579e`。

| 原目录 | 子模块内路径 | 比对结果 |
| --- | --- | --- |
| `win-psk-src/` | `winpsk/src/WinPSK/` | 55 个原有文件的 Git blob 哈希完全一致 |
| `pskcore-dll-src/` | `winpsk/src/PSKCore/` | 47 个原有文件的 Git blob 哈希完全一致 |

原目录另外添加的 `.gitignore`、`README.md` 和 `目录说明.md` 不属于上游源码。上游还包含少量 Visual Studio 工作区及构建记录文件。

核对时上游较新的提交修改过 `PskDet.cpp`，因此这里固定在上述匹配版本。WinPSK/PSKCore 的原始文件内容没有因本次整理而升级。

原来补充的说明仍可通过历史版本查阅：

- [WinPSK 构建说明](https://github.com/sundaydayhao/psk-modem-hub/blob/c1e62feea63664ce9a408377ffc23d24b25c6e39/win-psk-src/README.md)
- [WinPSK 目录说明](https://github.com/sundaydayhao/psk-modem-hub/blob/c1e62feea63664ce9a408377ffc23d24b25c6e39/win-psk-src/%E7%9B%AE%E5%BD%95%E8%AF%B4%E6%98%8E.md)
- [PSKCore 构建说明](https://github.com/sundaydayhao/psk-modem-hub/blob/c1e62feea63664ce9a408377ffc23d24b25c6e39/pskcore-dll-src/README.md)
- [PSKCore 目录说明](https://github.com/sundaydayhao/psk-modem-hub/blob/c1e62feea63664ce9a408377ffc23d24b25c6e39/pskcore-dll-src/%E7%9B%AE%E5%BD%95%E8%AF%B4%E6%98%8E.md)

## PSK 测试脚本

`psk-script` 引用已有的 [sundaydayhao/psk-script](https://github.com/sundaydayhao/psk-script)，固定提交为 `33bfa3c49be5dfd3b0be6afeb22984ff1cb1f8ed`。

原目录中的 31 个文件（包括脚本、说明和 WAV 文件）在该提交中均存在，Git blob 哈希完全一致。

## NUE-PSK

入口：[NUE-PSK 官方项目与源码下载](https://www.midnightdesignsolutions.com/NUE-PSK/index.html)。

本次通过 GitHub 仓库名及 `NUE_PSK_main_1.c`、`NUEPSK.mcw`、`NUE_PSK_Header.h` 等特征文件检索，未找到可核验的完整 GitHub 仓库。按仓库维护者要求，移除了 `nuepsk_src/` 源码副本，只保留官方链接，不使用包含零散移植代码的其他项目替代。

原有 NUE-PSK 2.33 源码仍保存在本仓库的 Git 历史中；官网提供的下载版本可能不同。

## 参考资料

`docs/` 中原有的 PDF 参考资料保留。本次未改写 Git 历史。

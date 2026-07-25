# NUE-PSK 移植到立创实战派 ESP32-S3 的可行性分析

## 1. 结论

NUE-PSK 可以移植到立创实战派 ESP32-S3 开发板，但这不是“改引脚即可运行”的移植，而是一次重构式移植。

更准确地说：

- 原项目是围绕 dsPIC33FJ128MC706、dsPIC DSP 库、片上 ADC、外置 DAC、PGA、PS/2 键盘、QEI 旋钮、128x64 LCD 等硬件写成的完整固件。
- 立创实战派 ESP32-S3 的硬件资源足够承载 PSK31/RTTY/CW 这类低速数字通信应用。
- 可复用的重点是 PSK/RTTY/CW 的协议状态机、调制/解调流程、字符处理、AFC/静噪等算法思路。
- 必须重写的是音频输入输出、显示、触摸/按键、存储、任务调度、硬件控制和驱动层。

建议目标不是“原样运行 NUE-PSK”，而是“基于 NUE-PSK 算法和交互逻辑，重新实现一个 ESP32-S3 版本的独立数字模式终端”。

## 2. 原 NUE-PSK 工程的关键依赖

根据本地源码和《基于 dsPIC 的无线通信系统设计.pdf》，原工程的关键结构如下：

- MCU：dsPIC33FJ128MC706。
- 采样链路：`Timer1` 以 8 kHz 节奏驱动 ADC/DAC 和收发处理。
- DSP 库：依赖 `dsp.h`、`fractional`、`FIRDecimate`、`FFTComplexIP`、`SquareMagnitudeCplx` 等 dsPIC DSP 类型和函数。
- 音频输入：dsPIC ADC 加模拟前端。
- 音频输出：MCP4922 DAC。
- 增益控制：MCP6S21 PGA。
- 显示：128x64 图形 LCD。
- 输入：PS/2 键盘、QEI 旋钮、按键。
- 存储：I2C EEPROM、Flash/Bootload 相关逻辑。
- 功能层：BPSK、QPSK、RTTY、CW、AFC、静噪、频谱显示、PTT 控制、宏文本、日志等。

这说明原代码的平台耦合很重，尤其是中断、ADC/DAC、LCD、键盘、QEI、EEPROM 和 dsPIC DSP 库。

## 3. 立创实战派 ESP32-S3 的适配判断

立创实战派 ESP32-S3 具备比较完整的音频和显示资源：

- ESP32-S3-WROOM-1-N16R8，双核 Xtensa LX7，最高 240 MHz。
- 8 MB PSRAM，16 MB Flash。
- ST7789 2.0 寸 320x240 SPI 屏。
- FT6336 电容触摸。
- ES7210 音频 ADC。
- ES8311 音频 DAC。
- NS4150B 功放。
- 双麦克风、喇叭、TF 卡、扩展接口。

这些资源对 NUE-PSK 这类 8 kHz 音频 DSP 项目是够用的。主要问题不在算力，而在以下几类适配工作：

- 把原来的 ADC/DAC 中断采样改成 ESP-IDF 的 I2S + DMA 音频流。
- 把 dsPIC DSP 库替换成 ESP-DSP、CMSIS-DSP 风格代码，或自写定点/浮点 FIR、FFT、NCO、相关检测模块。
- 把 128x64 LCD UI 重新映射到 ST7789 彩屏。
- 用触摸、实体按键或外接键盘替代 PS/2 键盘和 QEI 旋钮。
- 用 NVS、LittleFS 或 TF 卡替代 EEPROM 和原来的配置/日志存储方式。
- 用 FreeRTOS 任务、队列、环形缓冲区替代“重中断驱动”的 dsPIC 写法。

## 4. 可复用与必须重写的模块

| 类型 | 原工程内容 | 处理建议 |
|---|---|---|
| 可参考/部分复用 | `PSKMod.c`、`PskDet.c` 中的调制、解调、字符状态机、AFC/静噪思路 | 抽出平台无关算法，去掉 dsPIC 类型和寄存器依赖 |
| 可参考/需重写 | 频谱、水瀑、信号检测、收发状态控制 | 保留算法流程，重写缓冲、FFT、显示接口 |
| 基本重写 | ADC、DAC、Timer1 ISR | 改为 I2S + DMA + FreeRTOS 音频任务 |
| 基本重写 | LCD 显示层 | 改为 ST7789/LVGL 或轻量自绘 UI |
| 基本重写 | PS/2 键盘、QEI 旋钮、按键 | 改为触摸 UI、USB/BLE 键盘、GPIO 旋钮或按键 |
| 基本重写 | EEPROM、Flash、Bootloader 逻辑 | 改为 NVS、SD 卡、OTA 或 ESP-IDF 分区 |
| 需要重新设计 | 模拟音频前端、PTT、电平匹配、隔离 | 按实际电台接口重新做硬件适配 |

## 5. 主要风险点

### 5.1 实时性

PSK31 本身速率不高，但系统同时做音频采样、滤波、FFT、频谱显示、UI 刷新和收发控制时，仍然需要稳定的实时结构。

建议：

- 音频采样使用 I2S DMA，不要在高频中断中做重计算。
- DSP 处理使用固定大小 block，例如 64、128 或 256 sample。
- 显示刷新限频，不要让 UI 抢占音频任务。
- 音频任务优先级高于 UI、存储和网络任务。

### 5.2 音频前端

这是项目成败的核心风险之一。NUE-PSK 不是简单“麦克风听声音、喇叭放声音”。

需要明确：

- 电台音频输出到开发板 ADC 的电平范围。
- 开发板音频输出到电台 MIC/LINE 输入的电平范围。
- 是否需要隔离变压器或光耦。
- 是否需要硬件 PTT。
- 是否需要 VOX 模式。
- 是否需要抗 RF 干扰、接地隔离和限幅保护。

立创实战派板载音频更多是面向语音/多媒体应用，接无线电台时建议额外设计接口小板。

### 5.2.1 麦克风输入改 3.5mm 音频线输入

根据 ES8311 麦克风输入电路，`MIC1P/MIC1N` 才是音频输入端；右侧标注为 `PA_IN+`、`PA_IN-` 的网络是 ES8311 的差分输出去功放输入，不应作为 3.5mm 插孔输入使用。

如果要把原麦克风电路改为 3.5mm 音频线输入，推荐做成单声道 line-in：

```text
3.5mm Tip   -- 10k --+
                     +-- 4.7k -- C40 1uF -- MIC1P
3.5mm Ring  -- 10k --+

3.5mm Sleeve ------------------- AU_GND
AU_GND -------- C41 1uF -------- MIC1N
```

处理建议：

- 拆掉原 `MIC1` 麦克风模块。
- 断开或移除 `R34 220R`，不要把 `VMIC` 麦克风偏置电源送到 3.5mm 插孔。
- 保留 `C40/C41` 作为交流耦合电容。
- 保留 `C43/C44/C45` 等 ES8311 参考电压去耦。
- `R27/R28` 可以保留 0R，调试时也可改为 100R 到 1k 的串联保护电阻。
- 软件上把 ES8311 配置为 ADC/MIC 输入，并把 MIC Boost/PGA 增益调低，避免 line-level 信号过载。

如果输入来自电台耳机口、电脑声卡或手机耳机口，电平通常高于麦克风，建议加衰减和保护：

```text
L -- 10k --+
           +-- 10k -- C40 1uF -- MIC1P
R -- 10k --+
           |
          2.2k
           |
         AU_GND

Sleeve -- AU_GND
AU_GND -- C41 1uF -- MIC1N
```

用于真实电台时，建议在 3.5mm 输入前增加 600:600 音频隔离变压器、限幅保护和必要的 RF 滤波，避免地环路、过压和射频串扰。PSK31/NUE-PSK 只需要单声道音频输入，左右声道混合成一路即可。

### 5.3 DSP 兼容

原代码大量使用 dsPIC 的 fractional 定点 DSP 类型。ESP32-S3 可以使用浮点或定点实现，但不能直接调用原 dsPIC DSP 库。

建议优先策略：

- 第一阶段使用浮点实现，便于验证算法正确性。
- 性能不足时再局部改为定点或使用 ESP-DSP。
- 先保持 8 kHz 采样率，减少算法偏差。

### 5.4 UI 和交互

原项目是字符/图形 LCD + 键盘 + 旋钮风格。ESP32-S3 版本可以做成：

- 彩屏频谱/水瀑 + 触摸按钮。
- 外接 USB/BLE 键盘输入文本。
- GPIO 旋钮调整频点、阈值、模式。
- SD 卡保存宏文本和日志。

不要一开始就完整复刻原 UI，先实现最短收发闭环。

## 6. 推荐移植路线

建议按以下顺序推进：

1. 建立 ESP-IDF 工程，先跑通 ST7789、触摸、I2S 输入、I2S 输出和 TF 卡。
2. 固定 8 kHz 音频流，做环形缓冲和音频回环测试。
3. 移植 TX 路径，先让 ESP32-S3 产生 BPSK31 音频。
4. 移植 RX 路径，先用录音文件或已知测试音验证解调。
5. 添加最小 UI：模式、频点、TX/RX 状态、解码文本、输入文本。
6. 添加 PTT、电台接口、电平匹配和隔离。
7. 再补 RTTY、CW、宏文本、日志、配置保存、完整菜单。

最小可行目标建议设为：

- BPSK31 RX 能稳定解码。
- BPSK31 TX 能输出正确音频。
- 能通过一个简单 UI 切换 TX/RX。
- 能连接电台音频口并完成一次真实收发测试。

## 7. 如果不使用立创实战派，可选平台

下面是更适合移植 NUE-PSK 的几类平台。

| 平台 | 适合度 | 优点 | 主要问题 | 适合场景 |
|---|---:|---|---|---|
| Teensy 4.1 + Audio Adapter Board | 高 | 600 MHz Cortex-M7，实时 MCU 风格，I2S 音频和 Audio Library 成熟，开发节奏快 | 板载显示/触摸不如立创实战派完整，需要另接屏幕和输入设备 | 想最快做出 MCU 版本音频 DSP 原型 |
| STM32H747I-DISCO | 高 | Cortex-M7 + Cortex-M4，板载 LCD、触摸、音频 codec、麦克风、SDRAM、QSPI Flash、microSD，外设完整 | STM32Cube/HAL/双核工程复杂度较高 | 想做更接近产品形态的嵌入式独立终端 |
| STM32H7 自研板或其他 H7 Discovery/Eval | 高 | 实时性强，CMSIS-DSP 生态好，SAI/I2S/DMA 适合音频 DSP | 需要较强 STM32 驱动和硬件能力 | 后续准备做正式硬件 |
| Raspberry Pi Zero 2 W / Raspberry Pi 4 / Raspberry Pi 5 | 中高 | Linux 生态成熟，UI、文件、网络、USB 声卡、日志非常方便，算法验证快 | 非硬实时，音频延迟和调度抖动需要处理，功耗和启动时间更高 | 快速原型、桌面化/网络化版本 |
| 其他 ESP32-S3 音频屏幕开发板 | 中高 | 仍可使用 ESP-IDF，复用 ESP32-S3 移植成果 | 需要确认是否有真正可用的 line-in/line-out、PSRAM、屏幕和扩展口 | 想继续沿用 ESP32-S3 技术栈 |
| ESP32-P4 类平台 | 中 | 算力和显示能力更强，适合复杂 UI | 生态、板卡、音频外设和资料成熟度需要具体确认 | 后续新平台探索，不建议作为第一版 |
| RP2040/RP2350 类 MCU | 中低 | 成本低，PIO 灵活 | RAM、音频 codec、显示和 DSP 余量有限，完整 NUE-PSK 终端压力较大 | 只做简化实验，不适合完整移植 |

## 8. 平台选择建议

如果目标是最快跑通算法和音频 DSP：

- 优先选 Teensy 4.1 + Audio Adapter Board。
- 理由：实时 MCU 风格接近原 dsPIC，I2S 音频链路成熟，Audio Library 可减少底层音频工作量。

如果目标是做一个完整嵌入式产品原型：

- 优先选 STM32H747I-DISCO 或 STM32H7 自研板。
- 理由：音频、LCD、触摸、外部 RAM、SD 卡、调试工具链都更偏工业嵌入式产品。

如果目标是尽快做出功能演示：

- 优先选 Raspberry Pi Zero 2 W、Raspberry Pi 4 或 Raspberry Pi 5。
- 理由：Linux 下 UI、键盘、文件、网络、日志都简单，算法调试效率高。
- 代价：实时性不如 MCU，音频路径最好使用 USB 声卡或稳定的 I2S HAT。

如果目标是复用你手头的板子和 ESP-IDF 经验：

- 继续使用立创实战派 ESP32-S3 是合理选择。
- 理由：它已经有屏幕、触摸、音频 ADC/DAC、PSRAM 和 TF 卡，硬件完整度不错。
- 代价：需要重写较多底层驱动和任务框架。

## 9. 总体推荐

综合看：

- 最推荐的 MCU 原型平台：Teensy 4.1 + Audio Adapter Board。
- 最推荐的产品化嵌入式平台：STM32H747I-DISCO 或 STM32H7 自研板。
- 最推荐的快速验证平台：Raspberry Pi Zero 2 W / Raspberry Pi 4 / Raspberry Pi 5 + USB 声卡。
- 最推荐的就地开工平台：立创实战派 ESP32-S3。

如果你已经有立创实战派 ESP32-S3，可以先用它做第一版。只有在 ESP32-S3 的音频前端、电台接口或实时调度明显卡住时，再切到 Teensy 4.1 或 STM32H7。

## 10. 参考资料

- [立创实战派 ESP32-S3 文档](https://wiki.lckfb.com/zh-hans/szpi-esp32s3/)
- [NUE-PSK 项目页](https://www.midnightdesignsolutions.com/NUE-PSK/index.html)
- [Teensy 4.1 官方页面](https://www.pjrc.com/store/teensy41.html)
- [Teensy Audio Adapter Board 官方页面](https://www.pjrc.com/store/teensy3_audio.html)
- [STM32H747I-DISCO 官方页面](https://www.st.com/en/evaluation-tools/stm32h747i-disco.html)
- [Raspberry Pi Zero 2 W 官方页面](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/)
- [本地 PDF：基于 dsPIC 的无线通信系统设计](./基于dsPIC的无线通信系统设计.pdf)
- [本地源码目录：nuepsk_source](./nuepsk_source)

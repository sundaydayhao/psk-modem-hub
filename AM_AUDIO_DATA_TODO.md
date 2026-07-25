# AM Audio Data Decode TODO

目标：使用 AM 接收机的音频输出作为输入，在 AIM5101 上采集音频并参考 `nuepsk_source` 实现低速数据信号解码。

## 1. 硬件输入链路

- [ ] 确认 AM 接收机可用音频输出接口类型：耳机口、LINE OUT、鉴频/检波后音频输出。
- [ ] 设计 3.5mm 或排针音频输入接口，明确 Tip/Ring/Sleeve 接线。
- [ ] 增加输入衰减网络，避免耳机口电平过大导致 codec ADC 过载。
- [ ] 增加限幅保护，防止插拔、电台音量过大或异常尖峰损坏 codec 输入。
- [ ] 增加 600:600 音频隔离变压器，降低地环路、共模噪声和 RF 串扰风险。
- [ ] 增加简单低通滤波，目标带宽约 300 Hz 到 3 kHz，适合 PSK31/低速音频数据。
- [ ] 确认音频地 `AU_GND` 与系统地连接策略，优先单点连接。
- [ ] 使用屏蔽线连接 AM 接收机音频输出，外壳/屏蔽层接地方案需实测确认。

## 2. Codec 硬件

- [ ] 选择外部 audio codec，优先参考图中的 ES8311。
- [ ] ES8311 电源接入：`AVDD/DVDD/PVDD` 按数据手册供电并加去耦。
- [ ] ES8311 I2C 接入 AIM5101，用于寄存器配置。
- [ ] ES8311 I2S 接入 AIM5101，用于音频采样数据传输。
- [ ] ES8311 输入使用 `MIC1P/MIC1N`，不要把 AM 音频线接到 `PA_IN+/PA_IN-`。
- [ ] 处理 `VMIC` 偏置：接 line/audio 输入时不要把麦克风偏置电源直接送到 AM 接收机。
- [ ] 预留 codec `RESET`、`POWER_EN`、`MUTE` 等 GPIO 控制脚。
- [ ] 默认 GPIO 控制脚暂定：
  - `PA0` -> codec reset
  - `PA1` -> codec power enable
  - `PA2` -> codec mute

## 3. AIM5101 管脚规划

- [ ] I2S0 推荐接法：
  - `PD9` -> `I2S0_MCK`
  - `PC9` -> `I2S0_WS`
  - `PC10` -> `I2S0_CK`
  - `PC11` -> `I2S0_SDI0`
  - `PC12` -> `I2S0_SDO0`
- [ ] I2C1 推荐接法：
  - `PD10` -> `I2C1_SCL`
  - `PD11` -> `I2C1_SDA`
- [ ] 确认以上管脚没有和启动模式、调试口、已有外设冲突。
- [ ] 确认 IO 电平与 ES8311 数字 IO 电平一致。

## 4. CSI Driver 层

- [ ] 完善 AIM5101 GPIO driver，保证 examples 不直接操作寄存器。
- [ ] 增加 AIM5101 pinmux 对 I2S0/I2C1 复用功能的配置支持。
- [ ] 实现 AIM5101 I2C/IIC driver。
- [ ] 实现 AIM5101 I2S driver。
- [ ] 支持 I2S RX DMA 或等效环形缓冲机制。
- [ ] 暴露稳定的音频采样 API：启动、停止、读 block、查询溢出。
- [ ] 建立基础 driver 自测：GPIO、I2C scan、I2S dummy capture。

## 5. ES8311 Driver

- [ ] 新增 `es8311` codec driver 模块。
- [ ] I2C 探测 ES8311 地址，图中标注为 `0x18`。
- [ ] 实现 ES8311 reset/power/mute GPIO 控制。
- [ ] 配置 ES8311 为 ADC 输入模式。
- [ ] 配置 I2S 格式：建议先用 16-bit mono。
- [ ] 配置采样率：优先 `8 kHz`，不行则用 `16 kHz/48 kHz` 后软件重采样。
- [ ] 配置输入增益，先用低增益，防止 AM 接收机音频过载。
- [ ] 增加输入电平检测，输出 RMS/peak 供调试。

## 6. 音频采样与缓冲

- [ ] 建立 I2S RX DMA buffer。
- [ ] 建立应用层 ring buffer。
- [ ] 每次处理固定 block，例如 64/128/256 samples。
- [ ] 检测并统计 buffer underrun/overflow。
- [ ] 实现 DC offset 去除。
- [ ] 实现输入幅度归一化或 AGC，先做简单慢速增益控制。
- [ ] 增加测试 tone 输入验证：1 kHz 正弦、扫频、静音噪声。

## 7. PC 生成 BPSK31 测试音频

- [ ] 安装 `fldigi`。
- [ ] 在 `fldigi` 中选择 `Op Mode -> PSK -> BPSK31`。
- [ ] 在 `fldigi` 中配置声卡输出：`Configure -> Sound Card`。
- [ ] 设置音频中心频率，建议先用 `1000 Hz` 或 `1500 Hz`。
- [ ] 输入固定测试文本：

```text
CQ CQ CQ TEST DE AIM5101 AIM5101 K
```

- [ ] 点击 `TX`，让电脑声卡输出 BPSK31 音频。
- [ ] 电脑输出音量从 `10%` 开始，避免 codec 输入过载。
- [ ] 使用耳机或示波器确认电脑确实有音频输出。
- [ ] 将电脑耳机口或 USB 声卡 Line Out 接入 ES8311 输入前端。
- [ ] 接入前必须经过衰减、交流耦合、限幅保护；真实电台场景建议再加 600:600 音频隔离变压器。
- [ ] 推荐调试接法：

```text
PC Line Out L -- 10k --+
                       +-- 2.2k -- AU_GND
PC Line Out R -- 10k --+
                       |
                       +-- 1uF -- ES8311 MIC1P

PC GND --------------------------- AU_GND
ES8311 MIC1N ----------- 1uF ------ AU_GND
```

- [ ] 如果只使用单声道输出，只接 L 通道即可，R 通道悬空或通过 10k 混合。
- [ ] 调试阶段不要把 PC 声卡输出直接接到 `MIC1P/MIC1N`，防止电平过高。

## 8. NUE-PSK 解码移植

- [ ] 梳理 `nuepsk_source/C Source/PskDet.c` 的接收处理入口。
- [ ] 抽离平台无关的 PSK/RTTY/CW 解码状态机。
- [ ] 替换 dsPIC `fractional` 类型和 DSP 库依赖。
- [ ] 替换 `FIRDecimate`、`FFTComplexIP`、`SquareMagnitudeCplx` 等 dsPIC DSP 调用。
- [ ] 先实现 BPSK31 RX，其他模式后置。
- [ ] 保持音频采样率和原算法一致，优先 8 kHz。
- [ ] 建立 WAV 离线测试程序：WAV -> 解码文本。
- [ ] 使用已知 PSK31 测试音频验证解码正确率。
- [ ] 将离线解码核心接入 AIM5101 实时音频流。

## 9. 音频验证与调试

- [ ] 静音输入时，AIM5101 串口输出 RMS 应接近底噪，peak 不应跳满量程。
- [ ] 播放 `1 kHz` 正弦波时，串口输出 RMS/peak 应稳定。
- [ ] 播放 `1 kHz` 正弦波时，确认采样数据没有削顶：
  - 16-bit PCM 正常峰值建议先控制在 `3000` 到 `12000`。
  - 如果经常接近 `32767` 或 `-32768`，说明过载，需要降低 PC 音量或增加硬件衰减。
- [ ] 播放 `fldigi` BPSK31 时，确认频谱峰值在设置的中心频率附近，例如 `1000 Hz` 或 `1500 Hz`。
- [ ] 调整 PC 音量和 ES8311 PGA 增益，让 BPSK31 峰值不削顶，同时明显高于底噪。
- [ ] 记录一段 I2S 原始 PCM 到内存或串口导出，转换为 WAV 后在 PC 上用 fldigi 回放验证。
- [ ] 如果 AIM5101 采到的 WAV 在 PC fldigi 上可以解码，再接入本机 NUE-PSK 解码核心。
- [ ] 如果 PC fldigi 不能解码 AIM5101 录到的 WAV，优先检查：
  - 输入电平是否过载
  - MIC 偏置是否错误送入外部音频源
  - 左右声道/差分输入接法是否反相或悬空
  - I2S 位宽、左右声道、字节序是否配置错误
  - 采样率是否和 WAV/算法假设一致
  - 地线、隔离和 RF 干扰问题
- [ ] 先用 PC 声卡直连测试，通过后再换成真实 AM 接收机音频输出。

## 10. 调试输出与 UI

- [ ] 串口输出当前采样率、buffer 状态、RMS、peak。
- [ ] 串口输出检测到的中心频率/频偏。
- [ ] 串口输出解码文本。
- [ ] 定义统一的解码结果对象，至少包含：
  - `type`: `text` / `image` / `binary`
  - `mode`: `bpsk31` / 后续其他音频数据模式
  - `seq`: 分包序号
  - `timestamp_ms`: 接收时间戳
  - `rssi_or_snr`: 音频信号质量指标
  - `payload_len`: 有效载荷长度
  - `payload`: 文本 UTF-8 或图片分片数据
- [ ] 增加文本重组缓存：收到完整文本消息后，交给 UI 显示并保存最近 N 条。
- [ ] 增加图片重组缓存：按 `seq`/长度/CRC 重组完整图片，再交给 UI 显示。
- [ ] 首版图片格式建议只支持一种低复杂度格式：
  - 优先：发送端把图片预处理成小尺寸 RGB565/灰度 raw bitmap，板端直接显示。
  - 后续：再考虑 BMP；PNG/JPEG 需要解码库和更多 RAM，先不要作为首版目标。
- [ ] 选择本机屏幕接口。AIM5101HXX 手册未看到专用 LCD/RGB/8080 控制器，首版建议使用 SPI TFT。
- [ ] 推荐屏幕规格：
  - `ST7789` 240x240/240x320 SPI TFT，适合文本和小图。
  - 或 `ILI9341` 320x240 SPI TFT，资料多但刷新数据量更大。
- [ ] 屏幕硬件连接建议：
  - `SPI1` -> TFT `SCK/MOSI/CS`
  - GPIO -> TFT `DC/RESET/BL`
  - `MISO` 可不接，除非需要读屏幕 ID。
- [ ] 在 `csi_driver` 层实现 SPI/pinmux/GPIO 控制，不在 example 中直接写寄存器。
- [ ] 增加 display driver：
  - SPI 发送命令/数据
  - 屏幕初始化
  - 设置窗口
  - 填充背景
  - 绘制 ASCII/UTF-8 文本
  - 绘制 RGB565/灰度图片
- [ ] 增加应用 UI 页面：
  - 当前音频状态：采样率、RMS、peak、SNR/锁定状态。
  - 当前解码模式：例如 BPSK31。
  - 最近文本消息列表。
  - 图片消息预览页。
  - 错误/丢包/CRC 统计页。
- [ ] 增加频谱/瀑布图 UI，作为调试页而不是首版必需功能。
- [ ] 增加模式选择、频点调整、阈值调整等交互。

## 10.1 手机端显示与无线转发

- [ ] 首版无线方案建议使用 ESP32/ESP8266 AT 模组，通过 UART 连接 AIM5101。
- [ ] AIM5101 与 ESP-AT 硬件连接：
  - AIM5101 `UARTx_TX` -> ESP `RX`
  - AIM5101 `UARTx_RX` -> ESP `TX`
  - GPIO -> ESP `EN/RESET`
  - 可选 GPIO -> ESP `BOOT`
  - 共地，确认都是 3.3 V IO；ESP 供电必须能承受 WiFi 发射峰值电流。
- [ ] 软件上新增 `wifi_transport` 层，封装 AT 指令、重连、发送队列和超时。
- [ ] 首版手机显示方式建议二选一：
  - ESP 开 AP + HTTP/WebSocket 服务，手机浏览器访问网页显示文本/图片。
  - ESP 连接路由器，手机访问 ESP IP 或通过 UDP/mDNS 发现。
- [ ] 推荐首版协议：
  - AIM5101 解码后通过 UART 发 JSON header + binary payload 给 ESP。
  - ESP 转成 WebSocket message 推送到手机网页。
  - 文本直接用 UTF-8。
  - 图片用 `image/rgb565` 自定义块或转成浏览器可显示的 BMP/PNG 后再发。
- [ ] 如果希望用蓝牙，优先让 ESP32 做 BLE GATT 或 Bluetooth SPP，AIM5101 仍然只通过 UART 发送结果。
- [ ] 不建议首版让 AIM5101 直接实现完整 TCP/IP/BLE 协议栈，工作量和风险比 UART + ESP 协处理器高。
- [ ] 手机网页功能：
  - 显示当前连接状态。
  - 显示实时音频质量。
  - 显示最新文本消息。
  - 图片消息自动拼接后展示。
  - 提供清空、保存、下载原始 payload 的按钮。
- [ ] 增加端到端验证：
  - PC 生成 BPSK31 文本 -> AIM5101 解码 -> 本机屏幕显示 -> ESP 转发 -> 手机显示。
  - PC 发送小尺寸图片 payload -> AIM5101 重组 -> 本机屏幕显示 -> 手机显示。
  - 人为断 WiFi/BT 后恢复，确认消息队列不会导致系统死锁。

## 11. 验证步骤

- [ ] ES8311 I2C scan 能发现 `0x18`。
- [ ] I2S 能采到非零音频样本。
- [ ] 输入 1 kHz tone，RMS 稳定且无削顶。
- [ ] 输入 PC 播放的 PSK31 音频，离线解码通过。
- [ ] AM 接收机本地短链路测试，确认音频解码可用。
- [ ] 真实 AM 信道测试，记录信噪比、误码、音量设置。

## 12. 当前已完成

- [x] 将 `smartl_rv32` 重命名为 `aim5101_rv32`。
- [x] GPIO 示例不再直接写寄存器。
- [x] 新增 codec reset/power/mute 的 GPIO 控制封装。
- [x] 默认 codec 控制脚通过 CSI GPIO API 操作。

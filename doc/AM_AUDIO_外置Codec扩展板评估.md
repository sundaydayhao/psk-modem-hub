# AM 音频前端 / Codec 扩展板评估

目标：评估一块 AIM5101HXX 外接音频/显示扩展板的实现方式，把 AM 接收机已经解调出来的音频基带信号转换成 AIM5101 可以采集的数字音频流，并通过 SPI 屏幕显示文本/图像信息。语音/音频播放可以由 ES8311 的模拟输出送入外部功放，也可以在后续改成其他音频实现。

本文只处理 AM 接收机的音频输出，不处理 AM 射频信号。它是一个候选方案文档，不代表项目最终必须固定采用 ES8311 或任何外置 codec。

## 1. 方案判断

### 1.1 片内 AD/DA

优点：
- 结构最简单。
- 不需要额外音频芯片。
- 适合快速做最小闭环验证。

缺点：
- 音频性能取决于 MCU 片内模拟能力。
- 输入输出电平、偏置、抗干扰和驱动能力通常较弱。
- 如果要接电台或做较稳的产品，后续还会补很多外围。

### 1.2 外置 Codec

优点：
- 音频链路更规范，ADC/DAC、偏置、增益更完整。
- 对接电台时更容易做出稳定输入输出。
- 更像可产品化方案。

缺点：
- 芯片、供电、I2S/I2C、复位和布线都更复杂。
- 成本和调试时间高。

### 1.3 板载音频

优点：
- 如果开发板已经带好 codec、功放、麦克风和屏幕，最省时间。
- 适合先跑通功能。

缺点：
- 强依赖开发板，不适合把方案直接当成最终产品架构。
- 很多板载音频更偏语音/多媒体，不一定适合电台音频前端。

结论：
- 原型最快：板载音频或片内 AD/DA。
- 更像产品：外置 codec。

## 2. 系统框图

```text
AM 接收机耳机口 / Line Out
        |
        v
3.5 mm 音频座
        |
        v
保护 + 衰减 + 交流耦合
        |
        v
ES8311 MIC1P/MIC1N ADC 输入
        |
        v
I2S0 数字音频
        |
        v
AIM5101HXX
        |
        +--> SPI0 屏幕显示文本/图像
        |
        +--> I2S0 音频播放到 ES8311 DAC
```

控制链路：

```text
AIM5101HXX I2C1 ----> ES8311 寄存器配置
AIM5101HXX GPIO ----> LCD/触摸/SD 片选、LCD 复位、D/C、背光
```

## 3. 外设占用总表

| 功能 | AIM5101HXX 外设 | 引脚 | 说明 |
|---|---|---|---|
| 外置 codec 音频 | `I2S0` | `PD9`, `PC9`, `PC10`, `PC11`, `PC12` | 仅在采用外置 codec 方案时占用 |
| 外置 codec 控制 | `I2C1` | `PD10`, `PD11` | 仅在采用外置 codec 方案时占用 |
| 14pin LCD/SD | `SPI0` | `PB6`, `PB7`, `PB8` | 与 codec 不冲突 |
| 14pin 电容触摸 | `I2C0` | `PE0`, `PE1` | 与 codec 的 `I2C1` 不冲突 |
| LCD/触摸/SD 控制 | GPIO | `PB5`, `PB9`-`PB11`, `PB14`, `PE2`, `PE3` | 片选、复位、D/C、背光、触摸复位/中断 |

如果采用外置 codec，就不要把屏幕接到 `SPI1` 的 `PC9-PC12`，因为这些管脚会被音频链路占用。

## 4. AIM5101HXX 到外置 Codec

| AIM5101HXX 引脚 | AIM5101HXX 功能 | ES8311 信号 | 方向 |
|---|---|---|---|
| `PD9` | `I2S0_MCK` | `MCLK` | AIM5101HXX -> 外置 codec |
| `PC10` | `I2S0_CK` | `SCLK/BCLK` | AIM5101HXX -> 外置 codec |
| `PC9` | `I2S0_WS` | `LRCK/WS` | AIM5101HXX -> 外置 codec |
| `PC11` | `I2S0_SDI0` | `ASDOUT/ADCOUT` | 外置 codec -> AIM5101HXX |
| `PC12` | `I2S0_SDO0` | `DSDIN/DACIN` | AIM5101HXX -> 外置 codec |
| `PD10` | `I2C1_SCL` | `CCLK/SCL` | AIM5101HXX -> 外置 codec |
| `PD11` | `I2C1_SDA` | `CDATA/SDA` | 双向 |

I2C 需要上拉：

```text
SCL -- 4.7k -- 3V3
SDA -- 4.7k -- 3V3
```

如果 AIM5101HXX 主板上已经有同一条 I2C1 总线的上拉，小板可以预留电阻位不上件。

## 5. 14pin SPI 触摸屏硬件连线

14pin 屏幕模块的 LCD 和 SD 卡共用 SPI，总线推荐使用 AIM5101HXX `SPI0`：

| AIM5101HXX 引脚 | 复用功能 | 用途 |
|---|---|---|
| `PB6` | `SPI0_SCK` | SPI 时钟 |
| `PB8` | `SPI0_MOSI` | MCU 到 LCD/SD 数据 |
| `PB7` | `SPI0_MISO` | SD 到 MCU 数据，LCD 读回可选 |
| `PB5` | `SPI0_NSS0` 或 GPIO | 推荐作 `LCD_CS` |

14pin 屏的电容触摸是 I2C 接口。若采用外置 codec，codec 已经使用 `I2C1`，所以触摸推荐使用另一组 `I2C0`：

| AIM5101HXX 引脚 | 复用功能 | 用途 |
|---|---|---|
| `PE0` | `I2C0_SDA` | 触摸 I2C 数据 |
| `PE1` | `I2C0_SCL` | 触摸 I2C 时钟 |

`PB9`-`PB11`、`PB14`、`PE2`、`PE3` 在本设计中作为普通 GPIO 使用。

### 4.1 LCD 部分

| 屏幕引脚 | 模块信号 | AIM5101HXX 连接 | 说明 |
|---|---|---|---|
| `1` | `VCC` | `3V3` | 模块电源 |
| `2` | `GND` | `GND` | 地 |
| `3` | `LCD_CS` | `PB5 / LCD_CS` | LCD 片选，低有效，建议 10k 上拉 |
| `4` | `LCD_RST` | `PB9 / LCD_RST` | LCD 复位，低有效 |
| `5` | `LCD_RS` | `PB10 / LCD_DC` | 命令/数据选择，高为数据、低为命令 |
| `6` | `SDI(MOSI)` | `PB8 / SPI0_MOSI` | LCD/SD 共用 SPI MOSI |
| `7` | `SCK` | `PB6 / SPI0_SCK` | LCD/SD 共用 SPI SCK |
| `8` | `LED` | `PB11 / LCD_BL_EN` 经 MOS 管 | 背光开关控制 |
| `9` | `SDO(MISO)` | `PB7 / SPI0_MISO`，可选 | LCD/SD 共用 SPI MISO；不读 LCD 且不用 SD 时可不接 |

背光 `LED` 不建议直接由 MCU GPIO 驱动，建议用 NMOS 或三极管做开关/PWM：

```text
3V3/5V -> 屏幕背光 LED 供电端
LED 回路 -> NMOS -> GND
PB11 -> 100R~1k -> NMOS gate
gate -- 100k -- GND
```

如果模块内部已经带背光限流，按模块手册接；不确定时不要直接把 `LED` 接到 MCU IO。
如果需要背光亮度调节，应改接到已确认支持硬件 PWM 的 AIM5101HXX 管脚，或用软件 PWM 控制 `PB11`。

### 4.2 电容触摸部分

如果需要触摸：

| 屏幕引脚 | 模块信号 | AIM5101HXX 连接 | 说明 |
|---|---|---|---|
| `10` | `CTP_SCL` | `PE1 / I2C0_SCL` | 触摸 I2C 时钟 |
| `11` | `CTP_RST` | `PE2 / CTP_RST` | 触摸复位，低有效 |
| `12` | `CTP_SDA` | `PE0 / I2C0_SDA` | 触摸 I2C 数据 |
| `13` | `CTP_INT` | `PE3 / CTP_INT` | 触摸中断，触摸时通常拉低 |

触摸 I2C 需要上拉：

```text
CTP_SCL -- 4.7k -- 3V3
CTP_SDA -- 4.7k -- 3V3
```

如果屏幕模块内部已经有上拉，扩展板可预留不上件。

### 4.3 TF/SD 卡部分

如果需要 TF/SD 卡：

| 屏幕引脚 | 模块信号 | AIM5101HXX 连接 | 说明 |
|---|---|---|---|
| `14` | `SD_CS` | `PB14 / SD_CS` | SD 片选，低有效，建议 10k 上拉 |

14pin 模块里 SD 卡的数据和时钟复用 `SDI(MOSI)`、`SDO(MISO)`、`SCK`。

### 4.4 SPI 总线注意事项

所有片选默认拉高：

```text
LCD_CS -- 10k -- 3V3
SD_CS  -- 10k -- 3V3
```

共用总线关系：

```text
PB6/SPI0_SCK  -> LCD SCK + SD-SCK
PB8/SPI0_MOSI -> LCD SDI + SD-MOSI
PB7/SPI0_MISO <- LCD SDO(可选) + SD-MISO
```

同一时刻只能拉低一个 SPI 片选。LCD 刷屏速度较高，`SCK/MOSI/CS/DC` 走线要短，并有完整地参考。所有 SPI/I2C IO 使用 3.3V 逻辑，不接 5V 逻辑。

## 6. 3.5 mm 四段耳机座

按常见 CTIA 四段式耳机定义：

| PJ-320D 脚号 | 插头段位 | CTIA 功能 |
|---|---|---|
| `1` | Tip | 左声道 `AU_L` |
| `4` | Ring1 | 右声道 `AU_R` |
| `3` | Ring2 | `GND` |
| `2` | Sleeve | `MIC` |

注意：OMTP 标准会把 `GND` 和 `MIC` 对调。现在常见手机耳机多为 CTIA，但最终仍应以所选 PJ-320D 数据手册和封装焊盘编号为准。

## 7. 推荐音频输入前端

AM 接收机或电脑声卡输出通常在 `AU_L/AU_R` 上，不在四段耳机的 `MIC` 脚上。若采用外置 codec，推荐把左右声道通过电阻混成单声道，再衰减、交流耦合到 codec 的 `MIC1P`，`MIC1N` 作为交流参考接地。

```text
PJ-320D pin 1 / AU_L -- 10k --+
                              +-- 2.2k -- GND
PJ-320D pin 4 / AU_R -- 10k --+
                              |
                              +-- 1uF -- 0R/100R -- ES8311 MIC1P

PJ-320D pin 3 / GND ---------------------- GND
ES8311 MIC1N -------- 1uF ---------------- GND
```

`0R/100R` 是预留串联保护电阻位，默认可贴 `0R`，需要抑制插拔尖峰时再改 `100R`。

真实电台或长线连接建议增加 600:600 音频隔离变压器，并在连接器附近增加 ESD 和 RF 滤波。

## 8. 外置 Codec 关键电路

| 外置 codec 引脚 | 要求 |
|---|---|
| `PVDD` | 接 `3V3`，靠近引脚放 `100nF` |
| `DVDD` | 接 `3V3` 或数据手册允许的数字电源，靠近引脚放 `100nF` |
| `AVDD` | 必须接模拟 `3V3`，靠近引脚放 `100nF + 1uF` |
| `AGND/DGND/EP` | 接地，EP 建议大面积接地并打过孔 |
| `VMID` | 按数据手册接 `1uF` 到地 |
| `ADCVREF` | 按数据手册接 `1uF` 到地 |
| `DACVREF` | 按数据手册接 `1uF` 到地 |

若采用 ES8311，`CE` 是 I2C 地址选择脚：

| `CE` 电平 | 7-bit I2C 地址 |
|---|---|
| `0` | `0x18` |
| `1` | `0x19` |

如果把 `CE` 接地，软件 I2C 扫描应看到 `0x18`。

## 9. 音频输出和功放

若采用 ES8311，`OUTP/OUTN` 是一对模拟音频输出，可送入模拟输入功放，例如 NS4150B，再驱动 3W 扬声器。

推荐链路：

```text
AIM5101HXX PC12/I2S0_SDO0 -> ES8311 DSDIN
ES8311 DAC -> OUTP/OUTN
OUTP/OUTN -> 交流耦合 -> NS4150B INP/INN
NS4150B VoP/VoN -> 扬声器
```

注意：

- NS4150B 的扬声器输出是 BTL，扬声器两端接 `VoP/VoN`，任意一端都不要接地。
- 若要接 3W 扬声器，功放电源建议用 5V，并预留足够电流。
- NS4150B `CTRL` 不要默认拉低导致关断；可上拉到工作电平，或接 MCU GPIO 控制。

如果改用 MAX98357A，它是数字 I2S 输入功放，应直接接 AIM5101HXX 的 I2S 输出，不接外置 codec 的模拟输出。

## 10. 当前引脚分配（外置 codec 方案）

| 模块 | 信号 | AIM5101HXX 引脚 | 是否与 codec 冲突 |
|---|---|---|---|
| ES8311 | `MCLK` | `PD9 / I2S0_MCK` | 否，codec 专用 |
| ES8311 | `BCLK` | `PC10 / I2S0_CK` | 否，codec 专用 |
| ES8311 | `LRCK` | `PC9 / I2S0_WS` | 否，codec 专用 |
| ES8311 | `ASDOUT` | `PC11 / I2S0_SDI0` | 否，codec 专用 |
| ES8311 | `DSDIN` | `PC12 / I2S0_SDO0` | 否，codec 专用 |
| ES8311 | `SCL` | `PD10 / I2C1_SCL` | 否，codec 专用 |
| ES8311 | `SDA` | `PD11 / I2C1_SDA` | 否，codec 专用 |
| LCD/SD | `SPI_SCK` | `PB6 / SPI0_SCK` | 否 |
| LCD/SD | `SPI_MOSI` | `PB8 / SPI0_MOSI` | 否 |
| LCD/SD | `SPI_MISO` | `PB7 / SPI0_MISO` | 否 |
| LCD | `LCD_CS` | `PB5 / GPIO 或 SPI0_NSS0` | 否 |
| LCD | `LCD_RST` | `PB9 / GPIO` | 否 |
| LCD | `LCD_DC` | `PB10 / GPIO` | 否 |
| LCD | `LCD_BL_EN` | `PB11 / GPIO` | 否 |
| Touch | `CTP_SDA` | `PE0 / I2C0_SDA` | 否，codec 用的是 I2C1 |
| Touch | `CTP_SCL` | `PE1 / I2C0_SCL` | 否，codec 用的是 I2C1 |
| Touch | `CTP_RST` | `PE2 / GPIO` | 否 |
| Touch | `CTP_INT` | `PE3 / GPIO input` | 否 |
| SD | `SD_CS` | `PB14 / GPIO` | 否 |

## 11. PCB 布局规则

- ES8311 靠近输入前端和音频连接器。
- 所有去耦电容紧贴 ES8311 对应电源/参考引脚。
- `MIC1P/MIC1N` 走线短、靠近、远离 `MCLK/BCLK/LRCK` 和 SPI 屏幕时钟。
- I2S/SPI 时钟不要从模拟输入网络下方穿过。
- ESD/RF 保护放在音频连接器附近。
- SPI 屏幕排线附近保留完整地参考，`SCK` 旁边避免长距离平行模拟音频线。
- 预留测试点：`MCLK`、`BCLK`、`LRCK`、`ASDOUT`、`SCL`、`SDA`、`SPI0_SCK`、`SPI0_MOSI`、`SPI0_MISO`、ADC 输入节点、`3V3`、`GND`。

## 12. 调试清单

- [ ] 确认 `3V3` 到 `GND` 没有短路。
- [ ] 确认 `PVDD/DVDD/AVDD` 都有正确电压。
- [ ] 确认 I2C 上拉存在，空闲时 `SCL/SDA` 为高电平。
- [ ] I2C 扫描能发现 ES8311：`CE=0` 时地址应为 `0x18`。
- [ ] 示波器或逻辑分析仪确认 `MCLK/BCLK/LRCK`。
- [ ] 配置 ES8311 为 ADC/MIC 输入模式。
- [ ] 输入 1 kHz 小信号，确认 I2S 采样非零且不削顶。
- [ ] 初始化 SPI0，确认 LCD 可以点亮和刷色。
- [ ] 若接触摸，确认 `CTP_INT` 和触摸坐标读数。
- [ ] 若接 SD，确认所有非目标片选保持高电平后再初始化 SD。

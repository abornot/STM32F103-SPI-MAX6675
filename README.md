# 基于 STM32F103 的采集 K 型热电偶温度示例

MCU 型号为 STM32F103RCT6，采用硬件 SPI 采集 MAX6675 转换后的热电偶温度。

### 硬件

![1](https://user-images.githubusercontent.com/117444566/229395000-9151f18d-b395-4e2a-a816-00840ec0dbcc.png)

![2](https://user-images.githubusercontent.com/117444566/229395037-2150f79b-ba96-4af4-af0c-ce33a2fcef6b.png)

### 注意

- 如果出现中文乱码，请修改文件编码为 GB18030
- 工程使用 IDE 为 ARM Keil 5.14
- 只能用于 K 型热电偶，其他型号自测
- 温度值在数组 GssThermocoupleTemperature[i] 中，共 3 路
- 参考正点原子，修改初始化中 SPI_CPOL_Low、SPI_CPHA_1Edge 和 SPI 速度设置 SPI_BaudRatePrescaler_8

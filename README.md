# 参考

[STM32系列(HAL库)——F103C8T6驱动WS2812全彩RGB模块（PWM+DMA方式）_stm32f1 ws2812彩虹渐变程序-CSDN博客](https://blog.csdn.net/lwb450921/article/details/124046605)

[RGB颜色查询对照表|RGB颜色|三原色配色表](https://www.sojson.com/rgb.html)

# 硬件V0.1 版本 软件首次基础代码

## 已完成

1. GPIO LED驱动正常；
2. 按键正常
3. 低功耗唤醒正常
4. 充放电正常

## 待解决
1. 修改单片机型号为STM32F103C8T6 ，原来使用的L系列芯片主频比较低，输出波形比较难达到
2. 为单片机添加外部晶振
3. 给w2812输出IO口添加上拉电阻和0R跳线电阻方便调试
4. 将工程移动到AD工程，方便后续维护；
5. 需要重新设计灯片板子和主板的挖空方式，现在的挖空不适合接线，没有注释；


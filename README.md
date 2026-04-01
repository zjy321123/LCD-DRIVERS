# LCD-DRIVERS
简介：驱动LCD屏幕
V1.0.0  初始版本 使用SPI加DMA传输加DMA搬运非堵塞模式，回调驱动，避免堵塞。 
V2.0.0  增加LVGL9.2.1支持，非堵塞运行，使用PACK包，全屏刷新越80Ms，LVGL全屏渲染时间在4M主频下就需要30ms左右了，再加缓冲区效果不大。且浪费RAM差不 多极限了。
https://blog.csdn.net/weixin_74724281/article/details/144062721 lvgl pack  参考链接
https://www.keil.arm.com/packs/?q=LVGL&pack-search=             arm官网LVGL pack参考链接
https://blog.csdn.net/qq_59953808/article/details/126445608     LVGL 源代码 参考链接
lvgl\env_support\cmsis-pack\LVGL.lvgl.9.2.0.pack                LVGL 源代码 Pack目录
V2.0.1  修改了LVGL的刷新率。刷新速度从80ms左右变成了60ms左右,修改了lvgl的源代码编译器类型报警提示。
V3.0.0  增加了GUI-GUIDER支持，增加lVGL设备输入组件支持

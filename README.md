# 2048程序员版
## 程序列表
| 程序名称 | 版本 | 本次上传时间 | 发起时间 |
|:--------:|:-------:|:--------:|:------:|
| 2048程序员版 | Version 1.3 | 2021-8-14 | 2021-7-31 |
| ggcc_graphics 2D图形引擎 | Version 1.6 | 2021-8-14 | 2021-7-9 17:00 |
## 编译系统
本程序使用**Dev-C++**编写，在**Dev-C++ 5.11**中以**TDM-GCC 4.9.2 64-bit Debug**的编译器编译通过。
## 程序介绍
### 游戏介绍
这是一个众所周知的2048小游戏

游戏包含3个模式，可以按空格键进行更改
#### 游戏截图
模式1：程序员版
![image](https://user-images.githubusercontent.com/88886127/129445816-2e8a7404-78db-4401-8c5a-6bc3c5df3ae0.png)

模式2：朝代版
![image](https://user-images.githubusercontent.com/88886127/129445829-49d8b1a7-a416-4103-ac6f-25d5b241ac1d.png)

模式3：数字版
![image](https://user-images.githubusercontent.com/88886127/129445842-d1608bbf-f835-4a3c-a2e7-3e7d78bb483c.png)

### 引擎介绍
本2D引擎是模仿**EasyX**制作的，用于**Dev-C++**，重写了大部分**EasyX**中的函数。

由于是控制台，绘制效率较低，所以不推荐制作游戏，只能做一些轻量级的程序

于**EasyX**不同的是，**ggcc_graphics**可以设置像素大小(默认16\*16)：
```cpp
settextsize(像素宽,像素高)
```
注意，这里用的是控制台，所以要将像素宽/2

例如，设置16\*16的像素要这样写:
```cpp
settextsize(8.0,16.0)
```
而设置1\*1的像素要这样写:
```cpp
settextsize(0.5,1.0)
```

更多内容可以参考**EasyX**官网

-------

# 2048 programmer Edition
## Program list
| Program name | version | upload time | launch time|
|:--------:|:-------:|:--------:|:------:|
| 2048 programmer | version 1.3 | 2021-8-14 | 2021-7-31|
| ggcc_ Graphics 2D graphics engine | version 1.6 | 2021-8-14 | 2021-7-9 17:00|
## Compiling system
This program is written in **DEV-C++** and compiled with **tdm-gcc 4.9.2 64 bit debug** compiler in **DEV-C++ 5.11**.
## Program introduction
### Game introduction
This is a well-known 2048 game

The game contains 3 modes, which can be changed by pressing the spacebar
#### Game screenshot
See above
### Engine introduction
This 2D engine is made by imitating **EasyX**. It is used for **DEV-C++**, and rewrites most functions in **EasyX**.

Because it is a console with low rendering efficiency, it is not recommended to make games. Only some lightweight programs can be made

Unlike **EasyX**, **ggcc_graphics** you can set the pixel size (default 16 \* 16):
```cpp
settextsize (pixel width, pixel height)
```
Note that the console is used here, so the pixel width should be / 2

For example, to set a pixel of 16 \* 16, write:
```cpp
settextsize(8.0,16.0)
```
The pixel setting 1 \* 1 should be written as follows:
```cpp
settextsize(0.5,1.0)
```
For more information, please refer to **EasyX** official website

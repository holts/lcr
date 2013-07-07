各种LCD控制IC的接口函数及LCD图形文本功能的C语言实现


就目前所接触到的LCD控制器来说，其接口主要有以下几种：

    1.8/16-bit 8080

    2.8/16-bit 6800

    3.4-bit 6800（主要出现在字符型LCD上）

    4.4-wire SPI

    5.3-wire SPI

    6.I2C Interface

lcdiof中除了16位的接口，其余的都已经完成并久经测试。


6800和8080的区别主要是总线的控制方式上
对于内存的存储，需要数据总线和地址总线，这都是一样的
但对于存取的控制，它们则采用了不同的方式——
8080是通过“读使能（RE）”和“写使能（WE）”两条控制线进行读写操作
6800是通过“总使能（E）”和“读写选择（W/R）”两条控制线进行
LCD显示模块的外部接口一般采用并行方式,并行接口接口线的读写时序常见以下两种模式: 
(1)8080模式。
这类模式通常有下列接口信号:
Vcc(工作主电源)
Vss(公共端)Vee(偏置负电源,常用于调整显示对比度)
/RES,复位线。
DB0~DB7,双向数据线。
Ｄ/Ｉ,数据/指令选择线(1:数据读写,0:命令读写)。
/CS,片选信号线(如果有多片组合,可有多条片选信号线)。
/WR, MPU向LCD写入数据控制线。
/RD, MPU从LCD读入数据控制线。
(2)6800模式。在这种模式下,
Ｖcc、Ｖss、Ｖee、/RES、DB0~DB7、D/I的功能同模式(1),
其他信号线为:R/W,读写控制(1:MPU读, 0:MPU写)。
E,允许信号(多片组合时,可有多条允许信号线)。 




    LCD控制IC显示内存的寻址方式有垂直寻址好水平寻址之分，如T6963，RA8816等用是水平寻址，

而ST7565，NT7534，S1D15605(SED1565)用的是垂直寻址。所以绘图和显示文本的函数就会有所不同，

为此，我写了两种模式下的图形文本函数，分别放在了vertical和horizontal两个目录下。

    图形函数的画圆部分参考了很多大侠的聪明智慧，如国内的XiaoQi（晓奇），国外的Bresenham和Pascal Stang等，在这里要对他们的高超智慧表示由衷的敬佩！

    文本显示功能要感谢晓奇工作室发布的XiaoQiZimo软件，使得文字数据的生成易如反掌。

    函数都是最初在51单片机上实现的，所以要稍作修改才能在AVR上运行。主要是改一些端口定义

（lcdiof），graphics和txt基本不用动！


在ccv_function_lib\graphics\这个目录下都是与图形相关的函数，

其下的bmp是一些bmp格式的位图；

vertical是垂直寻址模式下的图形函数；

horizontal是水平寻址模式下的图形函数。

画圆的函数原型如下

// Draw a circle at the center(Ox, Oy) with Radius(Rx)

void Circle(uint8 Ox, uint8 Oy, uint8 Rx);

下面是三种画圆算法中的一种，简洁明了，效果很棒！

                  // GNU's Algorithm for drawing circle

                // Pascal Stang - Copyright (C) 2002

                int16 tswitch, y, x = 0;

                uint8 d;

               

                d = Oy - Ox;

                y = Rx;

                tswitch = 3 - 2 * Rx;

                while (x <= y)

                {

                        Point(Ox + x, Oy + y);     Point(Ox + x, Oy - y);

                        Point(Ox - x, Oy + y);     Point(Ox - x, Oy - y);

                        Point(Oy + y - d, Oy + x); Point(Oy + y - d, Oy - x);

                        Point(Oy - y - d, Oy + x); Point(Oy - y - d, Oy - x);

                       

                        if (tswitch < 0) tswitch += (4 * x + 6);

                        else

                        {

                                tswitch += (4 * (x - y) + 10);

                                y--;

                        }

                        x++;

                }


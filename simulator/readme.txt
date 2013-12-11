
.. LCD模拟器使用说明

========================================================================
LCD模拟器
========================================================================

.. 在文档标题与副标之间的注释在章节标题后解析
    Above is the document title, and below is the subtitle.
   They are transformed from section titles after parsing.

------------------------------------------------------------------------
 文档副标题 Examples of Subtitle
------------------------------------------------------------------------

.. 文档信息栏 bibliographic fields (which also require a transform):

:作者: holts 
:Email: holts.he@gmail.com
:版本: $Revision: 1.00 $ 
:日期: $Date: 2013/08/08 11:27:46 $
:声明: 不承担由此产生的一切不良后果 
:版权: 此文档遵从开放原则，你可以自由的复制，修改，发布,发售，购买, 租借，销毁，改进；或是引用，摘录，合并，比较，分类等等, 总之你可以自由使用此文档.
 
.. contents:: 内容索引 Table of Contents
.. section-numbering::


建立驱动文件夹
------------------------------------------------------------------------

把自己的lcd驱动全部放到一个文件夹，如lcddr，把驱动的c源文件改成成cpp后缀文件，同时在VC6项目管理器中，加入这个文件夹下的所有cpp和h文件。

也可以直接编辑lcd.dsp, 将文件加入项目。

当然，如果你只是想看下显示效果，什么都不用改，直接使用现有的128X64驱动就可以了。

更改文件
------------------------------------------------------------------------

在源文件前面加（每个都加）

#include "stdafx.h"
#include "..\LCD.h"
#include "..\LCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

定义屏规规
------------------------------------------------------------------------

在读写缓冲区的lcddlg.cpp文件对应的头文件lcddlg.h 定义屏规格

#define     X_MAX       128
#define     Y_MAX       64


编程
------------------------------------------------------------------------

可以了，，，直接对DDRAM[] 操作就可以显示出来了。。

例子可以看 源代码，，，
	

添加 键盘控制的 方法，，，很简单的，，

在lcddlg.cpp里面找到函数

BOOL CLCDDlg::PreTranslateMessage(MSG* pMsg)

在里面添加
if( pMsg->message == WM_KEYDOWN)
{
    switch(pMsg->wParam)
   {
      case  VK_LEFT:
          Key = 1;break;
      case  VK_DOWN:
          Key = 2;break;       
      case  VK_UP:
          Key = 3;break;
      case  VK_RIGHT:
          Key = 4;break;       
    }
}
就可以了。。。


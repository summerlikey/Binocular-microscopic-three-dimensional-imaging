# Binocular-microscopic-three-dimensional-imaging
My project  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/api.png)  
GenICam  
GigE TL camera mako G-192B PoE
Vimba Manual.pdf  
Vimba CPP Manual.pdf  
Vimba ImageTransform Manual.pdf  
# 工作流程
相机采集时，连续的采集处理采集处理采集处理，采用队列保证每一帧都能处理，DMA队列->准备队列->处理队列  
# 数据传输和显示流程
不同相机有不同的流采集器，如果一个结构安装了多个相机，那么就会有多个流采集器，对应多个通道。对于每个通道，实际采集数据时拆分后的数据包RawData形式，在内存中是一维数组形式，每一帧有不同的标识表明一帧的开始和结束，每个数据包也有标识显示当前的通道，想要显示图像，用户程序需要将一维数据拼装回图像形式，可借助OpenCV或者MIL等图像处理包完成该工作。图片来源于网络。  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/shuju.png)
# Buffer management
User:  
1.Allocate memory for the frame buffers on the host PC.  
2.Announce the buffer (this hands the frame buffer over to the API ).  
3.Queue a frame (prepare buffer to be filled).  
Vimba:  
4.Vimba fills the buffer with an image from the camera  
5.Vimba returns the filled buffer (and hands it over to the user).  
User:  
6.Work with the image.  
7.Requeue the frame to hand it over to the API.  
# SynchronousGrab 同步
# asynchronousGrab 异步
# Image Capture and Image AcquisiƟon  
 ![image](https://github.com/summerlikey/Binocular-microscopic-three-dimensional-imaging/raw/master/image/acquistion.png)   
# Qt .pro
在.pro文件中设置    
INCLUDEPATH += $$(VIMBA_HOME)  
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaCPP\include"
INCLUDEPATH += "C:\Program Files\Allied Vision\Vimba_2.1\VimbaImageTransform\include"

LIBS += I:/qtcode/VimbaC/Lib/Win64/VimbaC.lib  
LIBS += I:/qtcode/VimbaCPP/Lib/Win64/VimbaCPP.lib  
LIBS += I:/qtcode/VimbaImageTransform/Lib/Win64/VimbaImageTransform.lib  
设置完成后，将下面4个文件放入debug 或者release中  
VimbaC.dll  
VimbaCPP.dll  
VimbaCPPd.dll  
VimbaImageTransform.dll  

# Qt 第三方库配置
QT配置一直失败，无法链接，连接后需要run qmake一下   
当多继承含有QObjiect时，需要将QObjiect放在前面，例如class FrameObserver : public QObject,public IFrameObserver，如何反过来将会出现中间编译错误  
# 处理流程
1.相机打开  
2.准备工作  
A.定义帧大小  
B.开始引擎   
C.帧放入队列    
3.开始采集  
  采集过程  这过程中，可以利用类的初始化进行参数传递，重要的是哪一个相机的传递，构造函数能够实现传递，（重要）
5.停止采集  
6.停止采集引擎  
7.释放队列  
8.释放所有帧  
9.释放所有观察器  
10.关闭当前相机  
# 官方例子在vs2017中打开
将qt 的version 调整到自己版本的，qt project option version  
将qt连接库lib调整到qt五的版本  
附加库目录选择本机qt的bin目录，本机新建一个环境变量QTDIR 值就是Qt的目录，因为include 和bin 目录都继承了环境变量，下次配置三方库时观察是否使用变量，变量可以在环境变量中配置（全局变量）  
# SDK特点
整体编程风格（C++)  
充分体现C++语言的优点，类的继承和面向对象，采用相机观察器、功能观察器和帧观察器，三种观察器能够表现AVT相机的大部分主要特点
